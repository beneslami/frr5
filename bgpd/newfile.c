/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

int bgp_update(struct peer *peer, struct prefix *p, uint32_t addpath_id,
	       struct attr *attr, afi_t afi, safi_t safi, int type,
	       int sub_type, struct prefix_rd *prd, mpls_label_t *label,
	       uint32_t num_labels, int soft_reconfig,
	       struct bgp_route_evpn *evpn)
{
	int ret;
	int aspath_loop_count = 0;
	struct bgp_node *rn;
	struct bgp *bgp;
	struct attr new_attr;
	struct attr *attr_new;
	struct bgp_info *ri;
	struct bgp_info *new;
	struct bgp_info_extra *extra;
	const char *reason;
	char pfx_buf[BGP_PRD_PATH_STRLEN];
	int connected = 0;
	int do_loop_check = 1;
	int has_valid_label = 0;
#if ENABLE_BGP_VNC
	int vnc_implicit_withdraw = 0;
#endif
	int same_attr = 0;

	memset(&new_attr, 0, sizeof(struct attr));
	new_attr.label_index = BGP_INVALID_LABEL_INDEX;
	new_attr.label = MPLS_INVALID_LABEL;

	bgp = peer->bgp;
	rn = bgp_afi_node_get(bgp->rib[afi][safi], afi, safi, p, prd);
	/* TODO: Check to see if we can get rid of "is_valid_label" */
	if (afi == AFI_L2VPN && safi == SAFI_EVPN)
		has_valid_label = (num_labels > 0) ? 1 : 0;
	else
		has_valid_label = bgp_is_valid_label(label);

	/* When peer's soft reconfiguration enabled.  Record input packet in
	   Adj-RIBs-In.  */
	if (!soft_reconfig
	    && CHECK_FLAG(peer->af_flags[afi][safi], PEER_FLAG_SOFT_RECONFIG)
	    && peer != bgp->peer_self)
		bgp_adj_in_set(rn, peer, attr, addpath_id);

	/* Check previously received route. */
	for (ri = rn->info; ri; ri = ri->next)
		if (ri->peer == peer && ri->type == type
		    && ri->sub_type == sub_type
		    && ri->addpath_rx_id == addpath_id)
			break;

	/* AS path local-as loop check. */
	if (peer->change_local_as) {
		if (peer->allowas_in[afi][safi])
			aspath_loop_count = peer->allowas_in[afi][safi];
		else if (!CHECK_FLAG(peer->flags,
				     PEER_FLAG_LOCAL_AS_NO_PREPEND))
			aspath_loop_count = 1;

		if (aspath_loop_check(attr->aspath, peer->change_local_as)
		    > aspath_loop_count) {
			reason = "as-path contains our own AS;";
			goto filtered;
		}
	}

	/* If the peer is configured for "allowas-in origin" and the last ASN in
	 * the
	 * as-path is our ASN then we do not need to call aspath_loop_check
	 */
	if (CHECK_FLAG(peer->af_flags[afi][safi], PEER_FLAG_ALLOWAS_IN_ORIGIN))
		if (aspath_get_last_as(attr->aspath) == bgp->as)
			do_loop_check = 0;

	/* AS path loop check. */
	if (do_loop_check) {
		if (aspath_loop_check(attr->aspath, bgp->as)
			    > peer->allowas_in[afi][safi]
		    || (CHECK_FLAG(bgp->config, BGP_CONFIG_CONFEDERATION)
			&& aspath_loop_check(attr->aspath, bgp->confed_id)
				   > peer->allowas_in[afi][safi])) {
			reason = "as-path contains our own AS;";
			goto filtered;
		}
	}

	/* Route reflector originator ID check.  */
	if (attr->flag & ATTR_FLAG_BIT(BGP_ATTR_ORIGINATOR_ID)
	    && IPV4_ADDR_SAME(&bgp->router_id, &attr->originator_id)) {
		reason = "originator is us;";
		goto filtered;
	}

	/* Route reflector cluster ID check.  */
	if (bgp_cluster_filter(peer, attr)) {
		reason = "reflected from the same cluster;";
		goto filtered;
	}

	/* Apply incoming filter.  */
	if (bgp_input_filter(peer, p, attr, afi, safi) == FILTER_DENY) {
		reason = "filter;";
		goto filtered;
	}

	bgp_attr_dup(&new_attr, attr);

	/* Apply incoming route-map.
	 * NB: new_attr may now contain newly allocated values from route-map
	 * "set"
	 * commands, so we need bgp_attr_flush in the error paths, until we
	 * intern
	 * the attr (which takes over the memory references) */
	if (bgp_input_modifier(peer, p, &new_attr, afi, safi, NULL)
	    == RMAP_DENY) {
		reason = "route-map;";
		bgp_attr_flush(&new_attr);
		goto filtered;
	}

	if (peer->sort == BGP_PEER_EBGP) {

		/* If we receive the graceful-shutdown community from an eBGP
		 * peer we must lower local-preference */
		if (new_attr.community
		    && community_include(new_attr.community, COMMUNITY_GSHUT)) {
			new_attr.flag |= ATTR_FLAG_BIT(BGP_ATTR_LOCAL_PREF);
			new_attr.local_pref = BGP_GSHUT_LOCAL_PREF;

			/* If graceful-shutdown is configured then add the GSHUT
			 * community to all paths received from eBGP peers */
		} else if (bgp_flag_check(peer->bgp,
					  BGP_FLAG_GRACEFUL_SHUTDOWN)) {
			bgp_attr_add_gshut_community(&new_attr);
		}
	}

	/* next hop check.  */
	if (!CHECK_FLAG(peer->flags, PEER_FLAG_IS_RFAPI_HD)
	    && bgp_update_martian_nexthop(bgp, afi, safi, &new_attr)) {
		reason = "martian or self next-hop;";
		bgp_attr_flush(&new_attr);
		goto filtered;
	}

	attr_new = bgp_attr_intern(&new_attr);

	/* If the update is implicit withdraw. */
	if (ri) {
		ri->uptime = bgp_clock();
		same_attr = attrhash_cmp(ri->attr, attr_new);

		/* Same attribute comes in. */
		if (!CHECK_FLAG(ri->flags, BGP_INFO_REMOVED)
		    && attrhash_cmp(ri->attr, attr_new)
		    && (!has_valid_label
			|| memcmp(&(bgp_info_extra_get(ri))->label, label,
				  num_labels * sizeof(mpls_label_t))
				   == 0)
		    && (overlay_index_equal(
			       afi, ri, evpn == NULL ? NULL : &evpn->eth_s_id,
			       evpn == NULL ? NULL : &evpn->gw_ip))) {
			if (CHECK_FLAG(bgp->af_flags[afi][safi],
				       BGP_CONFIG_DAMPENING)
			    && peer->sort == BGP_PEER_EBGP
			    && CHECK_FLAG(ri->flags, BGP_INFO_HISTORY)) {
				if (bgp_debug_update(peer, p, NULL, 1)) {
					bgp_debug_rdpfxpath2str(
						afi, safi, prd, p, label,
						num_labels, addpath_id ? 1 : 0,
						addpath_id, pfx_buf,
						sizeof(pfx_buf));
					zlog_debug("%s rcvd %s", peer->host,
						   pfx_buf);
				}

				if (bgp_damp_update(ri, rn, afi, safi)
				    != BGP_DAMP_SUPPRESSED) {
					bgp_aggregate_increment(bgp, p, ri, afi,
								safi);
					bgp_process(bgp, rn, afi, safi);
				}
			} else /* Duplicate - odd */
			{
				if (bgp_debug_update(peer, p, NULL, 1)) {
					if (!peer->rcvd_attr_printed) {
						zlog_debug(
							"%s rcvd UPDATE w/ attr: %s",
							peer->host,
							peer->rcvd_attr_str);
						peer->rcvd_attr_printed = 1;
					}

					bgp_debug_rdpfxpath2str(
						afi, safi, prd, p, label,
						num_labels, addpath_id ? 1 : 0,
						addpath_id, pfx_buf,
						sizeof(pfx_buf));
					zlog_debug(
						"%s rcvd %s...duplicate ignored",
						peer->host, pfx_buf);
				}

				/* graceful restart STALE flag unset. */
				if (CHECK_FLAG(ri->flags, BGP_INFO_STALE)) {
					bgp_info_unset_flag(rn, ri,
							    BGP_INFO_STALE);
					bgp_process(bgp, rn, afi, safi);
				}
			}

			bgp_unlock_node(rn);
			bgp_attr_unintern(&attr_new);

			return 0;
		}

		/* Withdraw/Announce before we fully processed the withdraw */
		if (CHECK_FLAG(ri->flags, BGP_INFO_REMOVED)) {
			if (bgp_debug_update(peer, p, NULL, 1)) {
				bgp_debug_rdpfxpath2str(
					afi, safi, prd, p, label, num_labels,
					addpath_id ? 1 : 0, addpath_id, pfx_buf,
					sizeof(pfx_buf));
				zlog_debug(
					"%s rcvd %s, flapped quicker than processing",
					peer->host, pfx_buf);
			}

			bgp_info_restore(rn, ri);
		}

		/* Received Logging. */
		if (bgp_debug_update(peer, p, NULL, 1)) {
			bgp_debug_rdpfxpath2str(afi, safi, prd, p, label,
						num_labels, addpath_id ? 1 : 0,
						addpath_id, pfx_buf,
						sizeof(pfx_buf));
			zlog_debug("%s rcvd %s", peer->host, pfx_buf);
		}

		/* graceful restart STALE flag unset. */
		if (CHECK_FLAG(ri->flags, BGP_INFO_STALE))
			bgp_info_unset_flag(rn, ri, BGP_INFO_STALE);

		/* The attribute is changed. */
		bgp_info_set_flag(rn, ri, BGP_INFO_ATTR_CHANGED);

		/* implicit withdraw, decrement aggregate and pcount here.
		 * only if update is accepted, they'll increment below.
		 */
		bgp_aggregate_decrement(bgp, p, ri, afi, safi);

		/* Update bgp route dampening information.  */
		if (CHECK_FLAG(bgp->af_flags[afi][safi], BGP_CONFIG_DAMPENING)
		    && peer->sort == BGP_PEER_EBGP) {
			/* This is implicit withdraw so we should update
			   dampening
			   information.  */
			if (!CHECK_FLAG(ri->flags, BGP_INFO_HISTORY))
				bgp_damp_withdraw(ri, rn, afi, safi, 1);
		}
#if ENABLE_BGP_VNC
		if (safi == SAFI_MPLS_VPN) {
			struct bgp_node *prn = NULL;
			struct bgp_table *table = NULL;

			prn = bgp_node_get(bgp->rib[afi][safi],
					   (struct prefix *)prd);
			if (prn->info) {
				table = (struct bgp_table *)(prn->info);

				vnc_import_bgp_del_vnc_host_route_mode_resolve_nve(
					bgp, prd, table, p, ri);
			}
			bgp_unlock_node(prn);
		}
		if ((afi == AFI_IP || afi == AFI_IP6)
		    && (safi == SAFI_UNICAST)) {
			if (CHECK_FLAG(ri->flags, BGP_INFO_SELECTED)) {
				/*
				 * Implicit withdraw case.
				 */
				++vnc_implicit_withdraw;
				vnc_import_bgp_del_route(bgp, p, ri);
				vnc_import_bgp_exterior_del_route(bgp, p, ri);
			}
		}
#endif

		/* Special handling for EVPN update of an existing route. If the
		 * extended community attribute has changed, we need to
		 * un-import
		 * the route using its existing extended community. It will be
		 * subsequently processed for import with the new extended
		 * community.
		 */
		if (safi == SAFI_EVPN && !same_attr) {
			if ((ri->attr->flag
			     & ATTR_FLAG_BIT(BGP_ATTR_EXT_COMMUNITIES))
			    && (attr_new->flag
				& ATTR_FLAG_BIT(BGP_ATTR_EXT_COMMUNITIES))) {
				int cmp;

				cmp = ecommunity_cmp(ri->attr->ecommunity,
						     attr_new->ecommunity);
				if (!cmp) {
					if (bgp_debug_update(peer, p, NULL, 1))
						zlog_debug(
							"Change in EXT-COMM, existing %s new %s",
							ecommunity_str(
								ri->attr->ecommunity),
							ecommunity_str(
								attr_new->ecommunity));
					bgp_evpn_unimport_route(bgp, afi, safi,
								p, ri);
				}
			}
		}

		/* Update to new attribute.  */
		bgp_attr_unintern(&ri->attr);
		ri->attr = attr_new;

		/* Update MPLS label */
		if (has_valid_label) {
			extra = bgp_info_extra_get(ri);
			memcpy(&extra->label, label,
			       num_labels * sizeof(mpls_label_t));
			extra->num_labels = num_labels;
			if (!(afi == AFI_L2VPN && safi == SAFI_EVPN))
				bgp_set_valid_label(&extra->label[0]);
		}

#if ENABLE_BGP_VNC
		if ((afi == AFI_IP || afi == AFI_IP6)
		    && (safi == SAFI_UNICAST)) {
			if (vnc_implicit_withdraw) {
				/*
				 * Add back the route with its new attributes
				 * (e.g., nexthop).
				 * The route is still selected, until the route
				 * selection
				 * queued by bgp_process actually runs. We have
				 * to make this
				 * update to the VNC side immediately to avoid
				 * racing against
				 * configuration changes (e.g., route-map
				 * changes) which
				 * trigger re-importation of the entire RIB.
				 */
				vnc_import_bgp_add_route(bgp, p, ri);
				vnc_import_bgp_exterior_add_route(bgp, p, ri);
			}
		}
#endif
		/* Update Overlay Index */
		if (afi == AFI_L2VPN) {
			overlay_index_update(
				ri->attr, evpn == NULL ? NULL : &evpn->eth_s_id,
				evpn == NULL ? NULL : &evpn->gw_ip);
		}

		/* Update bgp route dampening information.  */
		if (CHECK_FLAG(bgp->af_flags[afi][safi], BGP_CONFIG_DAMPENING)
		    && peer->sort == BGP_PEER_EBGP) {
			/* Now we do normal update dampening.  */
			ret = bgp_damp_update(ri, rn, afi, safi);
			if (ret == BGP_DAMP_SUPPRESSED) {
				bgp_unlock_node(rn);
				return 0;
			}
		}

		/* Nexthop reachability check - for unicast and
		 * labeled-unicast.. */
		if ((afi == AFI_IP || afi == AFI_IP6)
		    && (safi == SAFI_UNICAST || safi == SAFI_LABELED_UNICAST)) {
			if (peer->sort == BGP_PEER_EBGP && peer->ttl == 1
			    && !CHECK_FLAG(peer->flags,
					   PEER_FLAG_DISABLE_CONNECTED_CHECK)
			    && !bgp_flag_check(
				       bgp, BGP_FLAG_DISABLE_NH_CONNECTED_CHK))
				connected = 1;
			else
				connected = 0;

			struct bgp *bgp_nexthop = bgp;

			if (ri->extra && ri->extra->bgp_orig)
				bgp_nexthop = ri->extra->bgp_orig;

			if (bgp_find_or_add_nexthop(bgp, bgp_nexthop, afi,
							ri, NULL, connected)
			    || CHECK_FLAG(peer->flags, PEER_FLAG_IS_RFAPI_HD))
				bgp_info_set_flag(rn, ri, BGP_INFO_VALID);
			else {
				if (BGP_DEBUG(nht, NHT)) {
					char buf1[INET6_ADDRSTRLEN];
					inet_ntop(AF_INET,
						  (const void *)&attr_new
							  ->nexthop,
						  buf1, INET6_ADDRSTRLEN);
					zlog_debug("%s(%s): NH unresolved",
						   __FUNCTION__, buf1);
				}
				bgp_info_unset_flag(rn, ri, BGP_INFO_VALID);
			}
		} else
			bgp_info_set_flag(rn, ri, BGP_INFO_VALID);

#if ENABLE_BGP_VNC
		if (safi == SAFI_MPLS_VPN) {
			struct bgp_node *prn = NULL;
			struct bgp_table *table = NULL;

			prn = bgp_node_get(bgp->rib[afi][safi],
					   (struct prefix *)prd);
			if (prn->info) {
				table = (struct bgp_table *)(prn->info);

				vnc_import_bgp_add_vnc_host_route_mode_resolve_nve(
					bgp, prd, table, p, ri);
			}
			bgp_unlock_node(prn);
		}
#endif

		/* If this is an EVPN route and some attribute has changed,
		 * process
		 * route for import. If the extended community has changed, we
		 * would
		 * have done the un-import earlier and the import would result
		 * in the
		 * route getting injected into appropriate L2 VNIs. If it is
		 * just
		 * some other attribute change, the import will result in
		 * updating
		 * the attributes for the route in the VNI(s).
		 */
		if (safi == SAFI_EVPN && !same_attr)
			bgp_evpn_import_route(bgp, afi, safi, p, ri);

		/* Process change. */
		bgp_aggregate_increment(bgp, p, ri, afi, safi);

		bgp_process(bgp, rn, afi, safi);
		bgp_unlock_node(rn);

		if (SAFI_UNICAST == safi
		    && (bgp->inst_type == BGP_INSTANCE_TYPE_VRF
			|| bgp->inst_type == BGP_INSTANCE_TYPE_DEFAULT)) {

			vpn_leak_from_vrf_update(bgp_get_default(), bgp, ri);
		}
		if ((SAFI_MPLS_VPN == safi)
		    && (bgp->inst_type == BGP_INSTANCE_TYPE_DEFAULT)) {

			vpn_leak_to_vrf_update(bgp, ri);
		}

#if ENABLE_BGP_VNC
		if (SAFI_MPLS_VPN == safi) {
			mpls_label_t label_decoded = decode_label(label);

			rfapiProcessUpdate(peer, NULL, p, prd, attr, afi, safi,
					   type, sub_type, &label_decoded);
		}
		if (SAFI_ENCAP == safi) {
			rfapiProcessUpdate(peer, NULL, p, prd, attr, afi, safi,
					   type, sub_type, NULL);
		}
#endif

		return 0;
	} // End of implicit withdraw

	/* Received Logging. */
	if (bgp_debug_update(peer, p, NULL, 1)) {
		if (!peer->rcvd_attr_printed) {
			zlog_debug("%s rcvd UPDATE w/ attr: %s", peer->host,
				   peer->rcvd_attr_str);
			peer->rcvd_attr_printed = 1;
		}

		bgp_debug_rdpfxpath2str(afi, safi, prd, p, label, num_labels,
					addpath_id ? 1 : 0, addpath_id, pfx_buf,
					sizeof(pfx_buf));
		zlog_debug("%s rcvd %s", peer->host, pfx_buf);
	}

	/* Make new BGP info. */
	new = info_make(type, sub_type, 0, peer, attr_new, rn);

	/* Update MPLS label */
	if (has_valid_label) {
		extra = bgp_info_extra_get(new);
		memcpy(&extra->label, label, num_labels * sizeof(mpls_label_t));
		extra->num_labels = num_labels;
		if (!(afi == AFI_L2VPN && safi == SAFI_EVPN))
			bgp_set_valid_label(&extra->label[0]);
	}

	/* Update Overlay Index */
	if (afi == AFI_L2VPN) {
		overlay_index_update(new->attr,
				     evpn == NULL ? NULL : &evpn->eth_s_id,
				     evpn == NULL ? NULL : &evpn->gw_ip);
	}
	/* Nexthop reachability check. */
	if ((afi == AFI_IP || afi == AFI_IP6)
	    && (safi == SAFI_UNICAST || safi == SAFI_LABELED_UNICAST)) {
		if (peer->sort == BGP_PEER_EBGP && peer->ttl == 1
		    && !CHECK_FLAG(peer->flags,
				   PEER_FLAG_DISABLE_CONNECTED_CHECK)
		    && !bgp_flag_check(bgp, BGP_FLAG_DISABLE_NH_CONNECTED_CHK))
			connected = 1;
		else
			connected = 0;

		if (bgp_find_or_add_nexthop(bgp, bgp, afi, new, NULL, connected)
		    || CHECK_FLAG(peer->flags, PEER_FLAG_IS_RFAPI_HD))
			bgp_info_set_flag(rn, new, BGP_INFO_VALID);
		else {
			if (BGP_DEBUG(nht, NHT)) {
				char buf1[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET,
					  (const void *)&attr_new->nexthop,
					  buf1, INET6_ADDRSTRLEN);
				zlog_debug("%s(%s): NH unresolved",
					   __FUNCTION__, buf1);
			}
			bgp_info_unset_flag(rn, new, BGP_INFO_VALID);
		}
	} else
		bgp_info_set_flag(rn, new, BGP_INFO_VALID);

	/* Addpath ID */
	new->addpath_rx_id = addpath_id;

	/* Increment prefix */
	bgp_aggregate_increment(bgp, p, new, afi, safi);

	/* Register new BGP information. */
	bgp_info_add(rn, new);

	/* route_node_get lock */
	bgp_unlock_node(rn);

#if ENABLE_BGP_VNC
	if (safi == SAFI_MPLS_VPN) {
		struct bgp_node *prn = NULL;
		struct bgp_table *table = NULL;

		prn = bgp_node_get(bgp->rib[afi][safi], (struct prefix *)prd);
		if (prn->info) {
			table = (struct bgp_table *)(prn->info);

			vnc_import_bgp_add_vnc_host_route_mode_resolve_nve(
				bgp, prd, table, p, new);
		}
		bgp_unlock_node(prn);
	}
#endif

	/* If maximum prefix count is configured and current prefix
	   count exeed it. */
	if (bgp_maximum_prefix_overflow(peer, afi, safi, 0))
		return -1;

	/* If this is an EVPN route, process for import. */
	if (safi == SAFI_EVPN)
		bgp_evpn_import_route(bgp, afi, safi, p, new);

	/* Process change. */
	bgp_process(bgp, rn, afi, safi);

	if (SAFI_UNICAST == safi
	    && (bgp->inst_type == BGP_INSTANCE_TYPE_VRF
		|| bgp->inst_type == BGP_INSTANCE_TYPE_DEFAULT)) {
		vpn_leak_from_vrf_update(bgp_get_default(), bgp, new);
	}
	if ((SAFI_MPLS_VPN == safi)
	    && (bgp->inst_type == BGP_INSTANCE_TYPE_DEFAULT)) {

		vpn_leak_to_vrf_update(bgp, new);
	}
#if ENABLE_BGP_VNC
	if (SAFI_MPLS_VPN == safi) {
		mpls_label_t label_decoded = decode_label(label);

		rfapiProcessUpdate(peer, NULL, p, prd, attr, afi, safi, type,
				   sub_type, &label_decoded);
	}
	if (SAFI_ENCAP == safi) {
		rfapiProcessUpdate(peer, NULL, p, prd, attr, afi, safi, type,
				   sub_type, NULL);
	}
#endif

	return 0;

/* This BGP update is filtered.  Log the reason then update BGP
   entry.  */
filtered:
	if (bgp_debug_update(peer, p, NULL, 1)) {
		if (!peer->rcvd_attr_printed) {
			zlog_debug("%s rcvd UPDATE w/ attr: %s", peer->host,
				   peer->rcvd_attr_str);
			peer->rcvd_attr_printed = 1;
		}

		bgp_debug_rdpfxpath2str(afi, safi, prd, p, label, num_labels,
					addpath_id ? 1 : 0, addpath_id, pfx_buf,
					sizeof(pfx_buf));
		zlog_debug("%s rcvd UPDATE about %s -- DENIED due to: %s",
			   peer->host, pfx_buf, reason);
	}

	if (ri) {
		/* If this is an EVPN route, un-import it as it is now filtered.
		 */
		if (safi == SAFI_EVPN)
			bgp_evpn_unimport_route(bgp, afi, safi, p, ri);

		if (SAFI_UNICAST == safi
		    && (bgp->inst_type == BGP_INSTANCE_TYPE_VRF
			|| bgp->inst_type == BGP_INSTANCE_TYPE_DEFAULT)) {

			vpn_leak_from_vrf_withdraw(bgp_get_default(), bgp, ri);
		}
		if ((SAFI_MPLS_VPN == safi)
		    && (bgp->inst_type == BGP_INSTANCE_TYPE_DEFAULT)) {

			vpn_leak_to_vrf_withdraw(bgp, ri);
		}

		bgp_rib_remove(rn, ri, peer, afi, safi);
	}

	bgp_unlock_node(rn);

#if ENABLE_BGP_VNC
	/*
	 * Filtered update is treated as an implicit withdrawal (see
	 * bgp_rib_remove()
	 * a few lines above)
	 */
	if ((SAFI_MPLS_VPN == safi) || (SAFI_ENCAP == safi)) {
		rfapiProcessWithdraw(peer, NULL, p, prd, NULL, afi, safi, type,
				     0);
	}
#endif

	return 0;
}