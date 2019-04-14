static void _netlink_route_build_singlepath(const char *routedesc, int bytelen,
					    struct nexthop *nexthop,
					    struct nlmsghdr *nlmsg,
					    struct rtmsg *rtmsg,
					    size_t req_size, int cmd)
{
//    printf("\tM.SALARIDEBUG: Calling %s\n", __func__); // Delete me
	struct mpls_label_stack *nh_label;
	mpls_lse_t out_lse[MPLS_MAX_LABELS];
	int num_labels = 0;
	char label_buf[256];

	/*
	 * label_buf is *only* currently used within debugging.
	 * As such when we assign it we are guarding it inside
	 * a debug test.  If you want to change this make sure
	 * you fix this assumption
	 */
	label_buf[0] = '\0';
        
        //Mehran Memarnejad
        //Here we are building single path route
        vpp_data.is_multipath = VPP_SINGLE_PATH;
    
	assert(nexthop);
        


        
        //m.salari: l3vpn
        int table_id = vpp_vrf_id_to_table_id(nexthop->vrf_id);
        vpp_data.vrf = vpp_vrf_list[table_id];
        //\m.salari
        
        
        //m.salari debug deleteme
        int depth = -1; 
        printf("(((((( ");
        printf("desc:%s ", routedesc);
        printf("rtmtbl:%u ", rtmsg->rtm_table);
        //\m.salari debug deleteme

	for (struct nexthop *nh = nexthop; nh; nh = nh->rparent) {
                //m.salari: debug, deleteme
                depth++; 
                printf(" [ DPT %d, ", depth);
                printf("gat=%s, ", inet_ntoa(nh->gate.ipv4));
                printf("src=%s, ", inet_ntoa(nh->src.ipv4));
                printf("ifidx:%d ", nh->ifindex);
                printf("vrf=%u, ", nh->vrf_id);
                if (depth>0) {
                    printf("resgat=%s, ", inet_ntoa(nh->resolved->gate.ipv4));
                    printf("ressrc=%s, ", inet_ntoa(nh->resolved->src.ipv4));
//                  if (nh->resolved->vrf_id) {
                    printf("resvrf=%u, ", nh->resolved->vrf_id);
//                  }
                    


                }
		//\m.salari: debug, deleteme
                
                char label_buf1[20];

		nh_label = nh->nh_label;
		if (!nh_label || !nh_label->num_labels)
			continue;
                
                //Mehran Memarnejad
                vpp_data.num_labels = 0;
        
		for (int i = 0; i < nh_label->num_labels; i++) {
                    //m.salari: dbug, deleteme
                    printf("Lbl[%d]=%u ", i, nh->nh_label->label[i]);
                    //\m.salari: dbug, deleteme

                        //Mehran Memarnejad
                        //Add here so that we can count all labels including implicit null
                        vpp_data.label[i] = nh_label->label[i];
                        vpp_data.num_labels++;
                        
                        
			if (nh_label->label[i] == MPLS_LABEL_IMPLICIT_NULL)
				continue;

			if (IS_ZEBRA_DEBUG_KERNEL) {
				if (!num_labels)
					sprintf(label_buf, "label %u",
						nh_label->label[i]);
				else {
					sprintf(label_buf1, "/%u",
						nh_label->label[i]);
					strlcat(label_buf, label_buf1,
						sizeof(label_buf));
				}
			}

			out_lse[num_labels] =
				mpls_lse_encode(nh_label->label[i], 0, 0, 0);
			num_labels++;
		}
            printf(" ))))))");

	}//eo for (struct nexthop *nh = nexthop; nh; nh = nh->rparent)

	if (num_labels) {
		/* Set the BoS bit */
		out_lse[num_labels - 1] |= htonl(1 << MPLS_LS_S_SHIFT);

		if (rtmsg->rtm_family == AF_MPLS)
			addattr_l(nlmsg, req_size, RTA_NEWDST, &out_lse,
				  num_labels * sizeof(mpls_lse_t));
		else {
			struct rtattr *nest;
			uint16_t encap = LWTUNNEL_ENCAP_MPLS;

			addattr_l(nlmsg, req_size, RTA_ENCAP_TYPE, &encap,
				  sizeof(uint16_t));
			nest = addattr_nest(nlmsg, req_size, RTA_ENCAP);
			addattr_l(nlmsg, req_size, MPLS_IPTUNNEL_DST, &out_lse,
				  num_labels * sizeof(mpls_lse_t));
			addattr_nest_end(nlmsg, nest);
		}
	}

	if (CHECK_FLAG(nexthop->flags, NEXTHOP_FLAG_ONLINK))
		rtmsg->rtm_flags |= RTNH_F_ONLINK;

	if (rtmsg->rtm_family == AF_INET
	    && (nexthop->type == NEXTHOP_TYPE_IPV6
		|| nexthop->type == NEXTHOP_TYPE_IPV6_IFINDEX)) {
		rtmsg->rtm_flags |= RTNH_F_ONLINK;
		addattr_l(nlmsg, req_size, RTA_GATEWAY, &ipv4_ll, 4);
		addattr32(nlmsg, req_size, RTA_OIF, nexthop->ifindex);

		if (nexthop->rmap_src.ipv4.s_addr && (cmd == RTM_NEWROUTE))
			addattr_l(nlmsg, req_size, RTA_PREFSRC,
				  &nexthop->rmap_src.ipv4, bytelen);
		else if (nexthop->src.ipv4.s_addr && (cmd == RTM_NEWROUTE))
			addattr_l(nlmsg, req_size, RTA_PREFSRC,
				  &nexthop->src.ipv4, bytelen);

		if (IS_ZEBRA_DEBUG_KERNEL)
			zlog_debug(
				" 5549: _netlink_route_build_singlepath() (%s): "
				"nexthop via %s %s if %u(%u)",
				routedesc, ipv4_ll_buf, label_buf,
				nexthop->ifindex, nexthop->vrf_id);
		return;
	}

	if (nexthop->type == NEXTHOP_TYPE_IPV4
	    || nexthop->type == NEXTHOP_TYPE_IPV4_IFINDEX) {
		/* Send deletes to the kernel without specifying the next-hop */
		if (cmd != RTM_DELROUTE)
			_netlink_route_nl_add_gateway_info(
				rtmsg->rtm_family, AF_INET, nlmsg, req_size,
				bytelen, nexthop);

		if (cmd == RTM_NEWROUTE) {
			if (nexthop->rmap_src.ipv4.s_addr)
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->rmap_src.ipv4, bytelen);
			else if (nexthop->src.ipv4.s_addr)
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->src.ipv4, bytelen);
		}
                
                //Mehran Memarnejad
                vpp_data.ifindex = nexthop->ifindex;
                vpp_data.nexthop_addr.ipv4 = nexthop->gate.ipv4;
                //vpp_data.ifname = "Giga\0"; //We will get it using inet_aton()
                vpp_data.table_index = 0; //For now in default table

		if (IS_ZEBRA_DEBUG_KERNEL)
			zlog_debug(
				"netlink_route_multipath() (%s): "
				"nexthop via %s %s if %u(%u)",
				routedesc, inet_ntoa(nexthop->gate.ipv4),
				label_buf, nexthop->ifindex, nexthop->vrf_id);
	}

	if (nexthop->type == NEXTHOP_TYPE_IPV6
	    || nexthop->type == NEXTHOP_TYPE_IPV6_IFINDEX) {
		_netlink_route_nl_add_gateway_info(rtmsg->rtm_family, AF_INET6,
						   nlmsg, req_size, bytelen,
						   nexthop);

		if (cmd == RTM_NEWROUTE) {
			if (!IN6_IS_ADDR_UNSPECIFIED(&nexthop->rmap_src.ipv6))
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->rmap_src.ipv6, bytelen);
			else if (!IN6_IS_ADDR_UNSPECIFIED(&nexthop->src.ipv6))
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->src.ipv6, bytelen);
		}

		if (IS_ZEBRA_DEBUG_KERNEL)
			zlog_debug(
				"netlink_route_multipath() (%s): "
				"nexthop via %s %s if %u(%u)",
				routedesc, inet6_ntoa(nexthop->gate.ipv6),
				label_buf, nexthop->ifindex, nexthop->vrf_id);
	}

	/*
	 * We have the ifindex so we should always send it
	 * This is especially useful if we are doing route
	 * leaking.
	 */
	if (nexthop->type != NEXTHOP_TYPE_BLACKHOLE)
		addattr32(nlmsg, req_size, RTA_OIF, nexthop->ifindex);

	if (nexthop->type == NEXTHOP_TYPE_IFINDEX
	    || nexthop->type == NEXTHOP_TYPE_IPV4_IFINDEX) {
		if (cmd == RTM_NEWROUTE) {
			if (nexthop->rmap_src.ipv4.s_addr)
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->rmap_src.ipv4, bytelen);
			else if (nexthop->src.ipv4.s_addr)
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->src.ipv4, bytelen);
		}
                
                //Mehran Memarnejad
                vpp_data.ifindex = nexthop->ifindex;
                //inet_aton("0.0.0.0", &(vpp_data.nexthop_addr.ipv4)); //No via , reset_vpp_data() handle this
                //vpp_data.ifname = "GigaMehran\0"; //For now just be NULL
                vpp_data.table_index = 0; //For now in default table
        
		if (IS_ZEBRA_DEBUG_KERNEL)
			zlog_debug(
				"netlink_route_multipath() (%s): "
				"nexthop via if %u(%u)",
				routedesc, nexthop->ifindex, nexthop->vrf_id);
	}

	if (nexthop->type == NEXTHOP_TYPE_IPV6_IFINDEX) {
		if (cmd == RTM_NEWROUTE) {
			if (!IN6_IS_ADDR_UNSPECIFIED(&nexthop->rmap_src.ipv6))
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->rmap_src.ipv6, bytelen);
			else if (!IN6_IS_ADDR_UNSPECIFIED(&nexthop->src.ipv6))
				addattr_l(nlmsg, req_size, RTA_PREFSRC,
					  &nexthop->src.ipv6, bytelen);
		}

		if (IS_ZEBRA_DEBUG_KERNEL)
			zlog_debug(
				"netlink_route_multipath() (%s): "
				"nexthop via if %u(%u)",
				routedesc, nexthop->ifindex, nexthop->vrf_id);
	}
        
        //Mehran Memarnejad
        vpp_print_debug2(&vpp_data, "end of _route_build_singlepath:");//m.salari
        vpp_talk(&vpp_data);
    
}
