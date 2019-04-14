/* bgp_router_id => "bgp router-id A.B.C.D" */
DEFUN_CMD_FUNC_DECL(bgp_router_id)
#define funcdecl_bgp_router_id static int bgp_router_id_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	struct in_addr router_id,\
	const char * router_id_str __attribute__ ((unused)))
funcdecl_bgp_router_id;
DEFUN_CMD_FUNC_TEXT(bgp_router_id)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	struct in_addr router_id = { INADDR_ANY };
	const char *router_id_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "router_id")) {
			router_id_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &router_id);
		}
#if 1 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 1 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return bgp_router_id_magic(self, vty, argc, argv, router_id, router_id_str);
}

/* no_bgp_router_id => "no bgp router-id [A.B.C.D]" */
DEFUN_CMD_FUNC_DECL(no_bgp_router_id)
#define funcdecl_no_bgp_router_id static int no_bgp_router_id_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	struct in_addr router_id,\
	const char * router_id_str __attribute__ ((unused)))
funcdecl_no_bgp_router_id;
DEFUN_CMD_FUNC_TEXT(no_bgp_router_id)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	struct in_addr router_id = { INADDR_ANY };
	const char *router_id_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "router_id")) {
			router_id_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &router_id);
		}
#if 1 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 1 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return no_bgp_router_id_magic(self, vty, argc, argv, router_id, router_id_str);
}

/* af_rd_vpn_export => "[no] rd vpn export ASN:NN_OR_IP-ADDRESS:NN$rd_str" */
DEFUN_CMD_FUNC_DECL(af_rd_vpn_export)
#define funcdecl_af_rd_vpn_export static int af_rd_vpn_export_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * rd_str)
funcdecl_af_rd_vpn_export;
DEFUN_CMD_FUNC_TEXT(af_rd_vpn_export)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *rd_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "rd_str")) {
			rd_str = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return af_rd_vpn_export_magic(self, vty, argc, argv, no, rd_str);
}

/* af_label_vpn_export => "[no] label vpn export <(0-1048575)$label_val|auto$label_auto>" */
DEFUN_CMD_FUNC_DECL(af_label_vpn_export)
#define funcdecl_af_label_vpn_export static int af_label_vpn_export_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long label_val,\
	const char * label_val_str __attribute__ ((unused)),\
	const char * label_auto)
funcdecl_af_label_vpn_export;
DEFUN_CMD_FUNC_TEXT(af_label_vpn_export)
{
#if 3 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long label_val = 0;
	const char *label_val_str = NULL;
	const char *label_auto = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label_val")) {
			label_val_str = argv[_i]->arg;
			char *_end;
			label_val = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "label_auto")) {
			label_auto = argv[_i]->arg;
		}
#if 1 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 1 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return af_label_vpn_export_magic(self, vty, argc, argv, no, label_val, label_val_str, label_auto);
}

/* af_nexthop_vpn_export => "[no] nexthop vpn export <A.B.C.D|X:X::X:X>$nexthop_str" */
DEFUN_CMD_FUNC_DECL(af_nexthop_vpn_export)
#define funcdecl_af_nexthop_vpn_export static int af_nexthop_vpn_export_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const union sockunion * nexthop_str,\
	const char * nexthop_str_str __attribute__ ((unused)))
funcdecl_af_nexthop_vpn_export;
DEFUN_CMD_FUNC_TEXT(af_nexthop_vpn_export)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	union sockunion s__nexthop_str = { .sa.sa_family = AF_UNSPEC }, *nexthop_str = NULL;
	const char *nexthop_str_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "nexthop_str")) {
			nexthop_str_str = argv[_i]->arg;
			if (argv[_i]->text[0] == 'X') {
				s__nexthop_str.sa.sa_family = AF_INET6;
				_fail = !inet_pton(AF_INET6, argv[_i]->arg, &s__nexthop_str.sin6.sin6_addr);
				nexthop_str = &s__nexthop_str;
			} else {
				s__nexthop_str.sa.sa_family = AF_INET;
				_fail = !inet_aton(argv[_i]->arg, &s__nexthop_str.sin.sin_addr);
				nexthop_str = &s__nexthop_str;
			}
		}
#if 1 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 1 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return af_nexthop_vpn_export_magic(self, vty, argc, argv, no, nexthop_str, nexthop_str_str);
}

/* af_rt_vpn_imexport => "[no] <rt|route-target> vpn <import|export|both>$direction_str RTLIST..." */
DEFUN_CMD_FUNC_DECL(af_rt_vpn_imexport)
#define funcdecl_af_rt_vpn_imexport static int af_rt_vpn_imexport_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * direction_str,\
	const char * rtlist)
funcdecl_af_rt_vpn_imexport;
DEFUN_CMD_FUNC_TEXT(af_rt_vpn_imexport)
{
#if 3 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *direction_str = NULL;
	const char *rtlist = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "direction_str")) {
			direction_str = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "rtlist")) {
			rtlist = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return af_rt_vpn_imexport_magic(self, vty, argc, argv, no, direction_str, rtlist);
}

/* af_route_map_vpn_imexport => "[no] route-map vpn <import|export>$direction_str RMAP$rmap_str" */
DEFUN_CMD_FUNC_DECL(af_route_map_vpn_imexport)
#define funcdecl_af_route_map_vpn_imexport static int af_route_map_vpn_imexport_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * direction_str,\
	const char * rmap_str)
funcdecl_af_route_map_vpn_imexport;
DEFUN_CMD_FUNC_TEXT(af_route_map_vpn_imexport)
{
#if 3 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *direction_str = NULL;
	const char *rmap_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "direction_str")) {
			direction_str = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "rmap_str")) {
			rmap_str = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return af_route_map_vpn_imexport_magic(self, vty, argc, argv, no, direction_str, rmap_str);
}

/* af_import_vrf_route_map => "[no] import vrf route-map RMAP$rmap_str" */
DEFUN_CMD_FUNC_DECL(af_import_vrf_route_map)
#define funcdecl_af_import_vrf_route_map static int af_import_vrf_route_map_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * rmap_str)
funcdecl_af_import_vrf_route_map;
DEFUN_CMD_FUNC_TEXT(af_import_vrf_route_map)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *rmap_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "rmap_str")) {
			rmap_str = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return af_import_vrf_route_map_magic(self, vty, argc, argv, no, rmap_str);
}

/* bgp_imexport_vrf => "[no] import vrf NAME$import_name" */
DEFUN_CMD_FUNC_DECL(bgp_imexport_vrf)
#define funcdecl_bgp_imexport_vrf static int bgp_imexport_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * import_name)
funcdecl_bgp_imexport_vrf;
DEFUN_CMD_FUNC_TEXT(bgp_imexport_vrf)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *import_name = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "import_name")) {
			import_name = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return bgp_imexport_vrf_magic(self, vty, argc, argv, no, import_name);
}

/* bgp_imexport_vpn => "[no] <import|export>$direction_str vpn" */
DEFUN_CMD_FUNC_DECL(bgp_imexport_vpn)
#define funcdecl_bgp_imexport_vpn static int bgp_imexport_vpn_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * direction_str)
funcdecl_bgp_imexport_vpn;
DEFUN_CMD_FUNC_TEXT(bgp_imexport_vpn)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *direction_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "direction_str")) {
			direction_str = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return bgp_imexport_vpn_magic(self, vty, argc, argv, no, direction_str);
}

/* af_routetarget_import => "[no] <rt|route-target> redirect import RTLIST..." */
DEFUN_CMD_FUNC_DECL(af_routetarget_import)
#define funcdecl_af_routetarget_import static int af_routetarget_import_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * rtlist)
funcdecl_af_routetarget_import;
DEFUN_CMD_FUNC_TEXT(af_routetarget_import)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *rtlist = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "rtlist")) {
			rtlist = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return af_routetarget_import_magic(self, vty, argc, argv, no, rtlist);
}

/* show_ip_bgp_instance_updgrps_adj_s => "show [ip]$ip bgp [<view|vrf> VIEWVRFNAME$vrf] [<ipv4|ipv6>$afi <unicast|multicast|vpn>$safi] update-groups [SUBGROUP-ID]$sgid <advertise-queue|advertised-routes|packet-queue>$rtq" */
DEFUN_CMD_FUNC_DECL(show_ip_bgp_instance_updgrps_adj_s)
#define funcdecl_show_ip_bgp_instance_updgrps_adj_s static int show_ip_bgp_instance_updgrps_adj_s_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ip,\
	const char * vrf,\
	const char * afi,\
	const char * safi,\
	const char * sgid,\
	const char * rtq)
funcdecl_show_ip_bgp_instance_updgrps_adj_s;
DEFUN_CMD_FUNC_TEXT(show_ip_bgp_instance_updgrps_adj_s)
{
#if 6 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ip = NULL;
	const char *vrf = NULL;
	const char *afi = NULL;
	const char *safi = NULL;
	const char *sgid = NULL;
	const char *rtq = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ip")) {
			ip = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vrf")) {
			vrf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "afi")) {
			afi = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "safi")) {
			safi = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "sgid")) {
			sgid = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "rtq")) {
			rtq = argv[_i]->arg;
		}
#if 0 /* anything that can fail? */
		if (_fail)
			vty_out (vty, "%% invalid input for %s: %s\n",
				   argv[_i]->varname, argv[_i]->arg);
		_failcnt += _fail;
#endif
	}
#if 0 /* anything that can fail? */
	if (_failcnt)
		return CMD_WARNING;
#endif
#endif
	return show_ip_bgp_instance_updgrps_adj_s_magic(self, vty, argc, argv, ip, vrf, afi, safi, sgid, rtq);
}

