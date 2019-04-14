/* ip_mroute_dist => "[no] ip mroute A.B.C.D/M$prefix <A.B.C.D$gate|INTERFACE$ifname> [(1-255)$distance]" */
DEFUN_CMD_FUNC_DECL(ip_mroute_dist)
#define funcdecl_ip_mroute_dist static int ip_mroute_dist_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv4 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	struct in_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long distance,\
	const char * distance_str __attribute__ ((unused)))
funcdecl_ip_mroute_dist;
DEFUN_CMD_FUNC_TEXT(ip_mroute_dist)
{
#if 5 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv4 prefix = { };
	const char *prefix_str = NULL;
	struct in_addr gate = { INADDR_ANY };
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long distance = 0;
	const char *distance_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix_ipv4(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
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
	return ip_mroute_dist_magic(self, vty, argc, argv, no, &prefix, prefix_str, gate, gate_str, ifname, distance, distance_str);
}

/* ip_route_blackhole => "[no] ip route	<A.B.C.D/M$prefix|A.B.C.D$prefix A.B.C.D$mask>                        	<reject|blackhole>$flag                                               	[{                                                                    	  tag (1-4294967295)                                                  	  |(1-255)$distance                                                   	  |vrf NAME                                                           	  |label WORD                                                         	  |table (1-4294967295)                                                         }]" */
DEFUN_CMD_FUNC_DECL(ip_route_blackhole)
#define funcdecl_ip_route_blackhole static int ip_route_blackhole_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * prefix,\
	struct in_addr mask,\
	const char * mask_str __attribute__ ((unused)),\
	const char * flag,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * vrf,\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)))
funcdecl_ip_route_blackhole;
DEFUN_CMD_FUNC_TEXT(ip_route_blackhole)
{
#if 9 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *prefix = NULL;
	struct in_addr mask = { INADDR_ANY };
	const char *mask_str = NULL;
	const char *flag = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *vrf = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mask")) {
			mask_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &mask);
		}
		if (!strcmp(argv[_i]->varname, "flag")) {
			flag = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "vrf")) {
			vrf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
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
	return ip_route_blackhole_magic(self, vty, argc, argv, no, prefix, mask, mask_str, flag, tag, tag_str, distance, distance_str, vrf, label, table, table_str);
}

/* ip_route_blackhole_vrf => "[no] ip route	<A.B.C.D/M$prefix|A.B.C.D$prefix A.B.C.D$mask>                        	<reject|blackhole>$flag                                               	[{                                                                    	  tag (1-4294967295)                                                  	  |(1-255)$distance                                                   	  |label WORD                                                         	  |table (1-4294967295)                                                         }]" */
DEFUN_CMD_FUNC_DECL(ip_route_blackhole_vrf)
#define funcdecl_ip_route_blackhole_vrf static int ip_route_blackhole_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * prefix,\
	struct in_addr mask,\
	const char * mask_str __attribute__ ((unused)),\
	const char * flag,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)))
funcdecl_ip_route_blackhole_vrf;
DEFUN_CMD_FUNC_TEXT(ip_route_blackhole_vrf)
{
#if 8 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *prefix = NULL;
	struct in_addr mask = { INADDR_ANY };
	const char *mask_str = NULL;
	const char *flag = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mask")) {
			mask_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &mask);
		}
		if (!strcmp(argv[_i]->varname, "flag")) {
			flag = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
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
	return ip_route_blackhole_vrf_magic(self, vty, argc, argv, no, prefix, mask, mask_str, flag, tag, tag_str, distance, distance_str, label, table, table_str);
}

/* ip_route_address_interface => "[no] ip route	<A.B.C.D/M$prefix|A.B.C.D$prefix A.B.C.D$mask> 	A.B.C.D$gate                                   	INTERFACE$ifname                               	[{                                             	  tag (1-4294967295)                           	  |(1-255)$distance                            	  |vrf NAME                                    	  |label WORD                                  	  |table (1-4294967295)                        	  |nexthop-vrf NAME                                      }]" */
DEFUN_CMD_FUNC_DECL(ip_route_address_interface)
#define funcdecl_ip_route_address_interface static int ip_route_address_interface_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * prefix,\
	struct in_addr mask,\
	const char * mask_str __attribute__ ((unused)),\
	struct in_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * vrf,\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ip_route_address_interface;
DEFUN_CMD_FUNC_TEXT(ip_route_address_interface)
{
#if 11 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *prefix = NULL;
	struct in_addr mask = { INADDR_ANY };
	const char *mask_str = NULL;
	struct in_addr gate = { INADDR_ANY };
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *vrf = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mask")) {
			mask_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &mask);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "vrf")) {
			vrf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ip_route_address_interface_magic(self, vty, argc, argv, no, prefix, mask, mask_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, vrf, label, table, table_str, nexthop_vrf);
}

/* ip_route_address_interface_vrf => "[no] ip route	<A.B.C.D/M$prefix|A.B.C.D$prefix A.B.C.D$mask> 	A.B.C.D$gate                                   	INTERFACE$ifname                               	[{                                             	  tag (1-4294967295)                           	  |(1-255)$distance                            	  |label WORD                                  	  |table (1-4294967295)                        	  |nexthop-vrf NAME                                      }]" */
DEFUN_CMD_FUNC_DECL(ip_route_address_interface_vrf)
#define funcdecl_ip_route_address_interface_vrf static int ip_route_address_interface_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * prefix,\
	struct in_addr mask,\
	const char * mask_str __attribute__ ((unused)),\
	struct in_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ip_route_address_interface_vrf;
DEFUN_CMD_FUNC_TEXT(ip_route_address_interface_vrf)
{
#if 10 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *prefix = NULL;
	struct in_addr mask = { INADDR_ANY };
	const char *mask_str = NULL;
	struct in_addr gate = { INADDR_ANY };
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mask")) {
			mask_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &mask);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ip_route_address_interface_vrf_magic(self, vty, argc, argv, no, prefix, mask, mask_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, label, table, table_str, nexthop_vrf);
}

/* ip_route => "[no] ip route	<A.B.C.D/M$prefix|A.B.C.D$prefix A.B.C.D$mask> 	<A.B.C.D$gate|INTERFACE$ifname>                	[{                                             	  tag (1-4294967295)                           	  |(1-255)$distance                            	  |vrf NAME                                    	  |label WORD                                  	  |table (1-4294967295)                        	  |nexthop-vrf NAME                                      }]" */
DEFUN_CMD_FUNC_DECL(ip_route)
#define funcdecl_ip_route static int ip_route_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * prefix,\
	struct in_addr mask,\
	const char * mask_str __attribute__ ((unused)),\
	struct in_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * vrf,\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ip_route;
DEFUN_CMD_FUNC_TEXT(ip_route)
{
#if 11 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *prefix = NULL;
	struct in_addr mask = { INADDR_ANY };
	const char *mask_str = NULL;
	struct in_addr gate = { INADDR_ANY };
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *vrf = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mask")) {
			mask_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &mask);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "vrf")) {
			vrf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ip_route_magic(self, vty, argc, argv, no, prefix, mask, mask_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, vrf, label, table, table_str, nexthop_vrf);
}

/* ip_route_vrf => "[no] ip route	<A.B.C.D/M$prefix|A.B.C.D$prefix A.B.C.D$mask> 	<A.B.C.D$gate|INTERFACE$ifname>                	[{                                             	  tag (1-4294967295)                           	  |(1-255)$distance                            	  |label WORD                                  	  |table (1-4294967295)                        	  |nexthop-vrf NAME                                      }]" */
DEFUN_CMD_FUNC_DECL(ip_route_vrf)
#define funcdecl_ip_route_vrf static int ip_route_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * prefix,\
	struct in_addr mask,\
	const char * mask_str __attribute__ ((unused)),\
	struct in_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ip_route_vrf;
DEFUN_CMD_FUNC_TEXT(ip_route_vrf)
{
#if 10 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *prefix = NULL;
	struct in_addr mask = { INADDR_ANY };
	const char *mask_str = NULL;
	struct in_addr gate = { INADDR_ANY };
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mask")) {
			mask_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &mask);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ip_route_vrf_magic(self, vty, argc, argv, no, prefix, mask, mask_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, label, table, table_str, nexthop_vrf);
}

/* show_route_table => "show <ip$ipv4|ipv6$ipv6> route table (1-4294967295)$table [json$json]" */
DEFUN_CMD_FUNC_DECL(show_route_table)
#define funcdecl_show_route_table static int show_route_table_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ipv4,\
	const char * ipv6,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * json)
funcdecl_show_route_table;
DEFUN_CMD_FUNC_TEXT(show_route_table)
{
#if 4 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ipv4 = NULL;
	const char *ipv6 = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ipv4")) {
			ipv4 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "ipv6")) {
			ipv6 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "json")) {
			json = argv[_i]->arg;
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
	return show_route_table_magic(self, vty, argc, argv, ipv4, ipv6, table, table_str, json);
}

/* show_route_table_vrf => "show <ip$ipv4|ipv6$ipv6> route table (1-4294967295)$table vrf NAME$vrf_name [json$json]" */
DEFUN_CMD_FUNC_DECL(show_route_table_vrf)
#define funcdecl_show_route_table_vrf static int show_route_table_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ipv4,\
	const char * ipv6,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * vrf_name,\
	const char * json)
funcdecl_show_route_table_vrf;
DEFUN_CMD_FUNC_TEXT(show_route_table_vrf)
{
#if 5 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ipv4 = NULL;
	const char *ipv6 = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *vrf_name = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ipv4")) {
			ipv4 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "ipv6")) {
			ipv6 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "vrf_name")) {
			vrf_name = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "json")) {
			json = argv[_i]->arg;
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
	return show_route_table_vrf_magic(self, vty, argc, argv, ipv4, ipv6, table, table_str, vrf_name, json);
}

/* show_route => "show         <	  ip$ipv4 <fib$fib|route> [vrf <NAME$vrf_name|all$vrf_all>]	   [{	    tag (1-4294967295)	    |A.B.C.D/M$prefix longer-prefixes	    |supernets-only$supernets_only	   }]	   [<	    RR_IP_REDIST_STR_ZEBR$type_str	    |ospf$type_str (1-65535)$ospf_instance_id	   >]          |ipv6$ipv6 <fib$fib|route> [vrf <NAME$vrf_name|all$vrf_all>]	   [{	    tag (1-4294967295)	    |X:X::X:X/M$prefix longer-prefixes	   }]	   [RR_IP6_REDIST_STR_ZEBR$type_str]	 >        [json$json]" */
DEFUN_CMD_FUNC_DECL(show_route)
#define funcdecl_show_route static int show_route_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ipv4,\
	const char * fib,\
	const char * vrf_name,\
	const char * vrf_all,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	const struct prefix * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const char * supernets_only,\
	const char * type_str,\
	long ospf_instance_id,\
	const char * ospf_instance_id_str __attribute__ ((unused)),\
	const char * ipv6,\
	const char * json)
funcdecl_show_route;
DEFUN_CMD_FUNC_TEXT(show_route)
{
#if 11 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ipv4 = NULL;
	const char *fib = NULL;
	const char *vrf_name = NULL;
	const char *vrf_all = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	struct prefix prefix = { };
	const char *prefix_str = NULL;
	const char *supernets_only = NULL;
	const char *type_str = NULL;
	long ospf_instance_id = 0;
	const char *ospf_instance_id_str = NULL;
	const char *ipv6 = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ipv4")) {
			ipv4 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "fib")) {
			fib = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vrf_name")) {
			vrf_name = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vrf_all")) {
			vrf_all = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "supernets_only")) {
			supernets_only = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "type_str")) {
			type_str = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "ospf_instance_id")) {
			ospf_instance_id_str = argv[_i]->arg;
			char *_end;
			ospf_instance_id = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "ipv6")) {
			ipv6 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "json")) {
			json = argv[_i]->arg;
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
	return show_route_magic(self, vty, argc, argv, ipv4, fib, vrf_name, vrf_all, tag, tag_str, &prefix, prefix_str, supernets_only, type_str, ospf_instance_id, ospf_instance_id_str, ipv6, json);
}

/* show_route_detail => "show         <          ip$ipv4 route [vrf <NAME$vrf_name|all$vrf_all>]          <	   A.B.C.D$address	   |A.B.C.D/M$prefix	  >          |ipv6$ipv6 route [vrf <NAME$vrf_name|all$vrf_all>]          <	   X:X::X:X$address	   |X:X::X:X/M$prefix	  >	 >" */
DEFUN_CMD_FUNC_DECL(show_route_detail)
#define funcdecl_show_route_detail static int show_route_detail_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ipv4,\
	const char * vrf_name,\
	const char * vrf_all,\
	const union sockunion * address,\
	const char * address_str __attribute__ ((unused)),\
	const struct prefix * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const char * ipv6)
funcdecl_show_route_detail;
DEFUN_CMD_FUNC_TEXT(show_route_detail)
{
#if 6 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ipv4 = NULL;
	const char *vrf_name = NULL;
	const char *vrf_all = NULL;
	union sockunion s__address = { .sa.sa_family = AF_UNSPEC }, *address = NULL;
	const char *address_str = NULL;
	struct prefix prefix = { };
	const char *prefix_str = NULL;
	const char *ipv6 = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ipv4")) {
			ipv4 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vrf_name")) {
			vrf_name = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vrf_all")) {
			vrf_all = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			if (argv[_i]->text[0] == 'X') {
				s__address.sa.sa_family = AF_INET6;
				_fail = !inet_pton(AF_INET6, argv[_i]->arg, &s__address.sin6.sin6_addr);
				address = &s__address;
			} else {
				s__address.sa.sa_family = AF_INET;
				_fail = !inet_aton(argv[_i]->arg, &s__address.sin.sin_addr);
				address = &s__address;
			}
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "ipv6")) {
			ipv6 = argv[_i]->arg;
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
	return show_route_detail_magic(self, vty, argc, argv, ipv4, vrf_name, vrf_all, address, address_str, &prefix, prefix_str, ipv6);
}

/* show_route_summary => "show         <          ip$ipv4 route [vrf <NAME$vrf_name|all$vrf_all>]            summary [prefix$prefix]          |ipv6$ipv6 route [vrf <NAME$vrf_name|all$vrf_all>]	    summary [prefix$prefix]	 >" */
DEFUN_CMD_FUNC_DECL(show_route_summary)
#define funcdecl_show_route_summary static int show_route_summary_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ipv4,\
	const char * vrf_name,\
	const char * vrf_all,\
	const char * prefix,\
	const char * ipv6)
funcdecl_show_route_summary;
DEFUN_CMD_FUNC_TEXT(show_route_summary)
{
#if 5 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ipv4 = NULL;
	const char *vrf_name = NULL;
	const char *vrf_all = NULL;
	const char *prefix = NULL;
	const char *ipv6 = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ipv4")) {
			ipv4 = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vrf_name")) {
			vrf_name = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vrf_all")) {
			vrf_all = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "ipv6")) {
			ipv6 = argv[_i]->arg;
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
	return show_route_summary_magic(self, vty, argc, argv, ipv4, vrf_name, vrf_all, prefix, ipv6);
}

/* ipv6_route_blackhole => "[no] ipv6 route X:X::X:X/M$prefix [from X:X::X:X/M]           <Null0|reject|blackhole>$flag                              [{                                                           tag (1-4294967295)                                         |(1-255)$distance                                          |vrf NAME                                                  |label WORD                                                |table (1-4294967295)                                    }]" */
DEFUN_CMD_FUNC_DECL(ipv6_route_blackhole)
#define funcdecl_ipv6_route_blackhole static int ipv6_route_blackhole_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv6 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const struct prefix_ipv6 * from,\
	const char * from_str __attribute__ ((unused)),\
	const char * flag,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * vrf,\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)))
funcdecl_ipv6_route_blackhole;
DEFUN_CMD_FUNC_TEXT(ipv6_route_blackhole)
{
#if 9 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv6 prefix = { };
	const char *prefix_str = NULL;
	struct prefix_ipv6 from = { };
	const char *from_str = NULL;
	const char *flag = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *vrf = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "from")) {
			from_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &from);
		}
		if (!strcmp(argv[_i]->varname, "flag")) {
			flag = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "vrf")) {
			vrf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
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
	return ipv6_route_blackhole_magic(self, vty, argc, argv, no, &prefix, prefix_str, &from, from_str, flag, tag, tag_str, distance, distance_str, vrf, label, table, table_str);
}

/* ipv6_route_blackhole_vrf => "[no] ipv6 route X:X::X:X/M$prefix [from X:X::X:X/M]           <Null0|reject|blackhole>$flag                              [{                                                           tag (1-4294967295)                                         |(1-255)$distance                                          |label WORD                                                |table (1-4294967295)                                    }]" */
DEFUN_CMD_FUNC_DECL(ipv6_route_blackhole_vrf)
#define funcdecl_ipv6_route_blackhole_vrf static int ipv6_route_blackhole_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv6 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const struct prefix_ipv6 * from,\
	const char * from_str __attribute__ ((unused)),\
	const char * flag,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)))
funcdecl_ipv6_route_blackhole_vrf;
DEFUN_CMD_FUNC_TEXT(ipv6_route_blackhole_vrf)
{
#if 8 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv6 prefix = { };
	const char *prefix_str = NULL;
	struct prefix_ipv6 from = { };
	const char *from_str = NULL;
	const char *flag = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "from")) {
			from_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &from);
		}
		if (!strcmp(argv[_i]->varname, "flag")) {
			flag = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
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
	return ipv6_route_blackhole_vrf_magic(self, vty, argc, argv, no, &prefix, prefix_str, &from, from_str, flag, tag, tag_str, distance, distance_str, label, table, table_str);
}

/* ipv6_route_address_interface => "[no] ipv6 route X:X::X:X/M$prefix [from X:X::X:X/M]           X:X::X:X$gate                                              INTERFACE$ifname                                           [{                                                           tag (1-4294967295)                                         |(1-255)$distance                                          |vrf NAME                                                  |label WORD                                    	    |table (1-4294967295)                                      |nexthop-vrf NAME                                        }]" */
DEFUN_CMD_FUNC_DECL(ipv6_route_address_interface)
#define funcdecl_ipv6_route_address_interface static int ipv6_route_address_interface_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv6 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const struct prefix_ipv6 * from,\
	const char * from_str __attribute__ ((unused)),\
	struct in6_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * vrf,\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ipv6_route_address_interface;
DEFUN_CMD_FUNC_TEXT(ipv6_route_address_interface)
{
#if 11 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv6 prefix = { };
	const char *prefix_str = NULL;
	struct prefix_ipv6 from = { };
	const char *from_str = NULL;
	struct in6_addr gate = IN6ADDR_ANY_INIT;
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *vrf = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "from")) {
			from_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &from);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_pton(AF_INET6, argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "vrf")) {
			vrf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ipv6_route_address_interface_magic(self, vty, argc, argv, no, &prefix, prefix_str, &from, from_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, vrf, label, table, table_str, nexthop_vrf);
}

/* ipv6_route_address_interface_vrf => "[no] ipv6 route X:X::X:X/M$prefix [from X:X::X:X/M]           X:X::X:X$gate                                              INTERFACE$ifname                                           [{                                                           tag (1-4294967295)                                         |(1-255)$distance                                          |label WORD                                    	    |table (1-4294967295)                                      |nexthop-vrf NAME                                        }]" */
DEFUN_CMD_FUNC_DECL(ipv6_route_address_interface_vrf)
#define funcdecl_ipv6_route_address_interface_vrf static int ipv6_route_address_interface_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv6 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const struct prefix_ipv6 * from,\
	const char * from_str __attribute__ ((unused)),\
	struct in6_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ipv6_route_address_interface_vrf;
DEFUN_CMD_FUNC_TEXT(ipv6_route_address_interface_vrf)
{
#if 10 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv6 prefix = { };
	const char *prefix_str = NULL;
	struct prefix_ipv6 from = { };
	const char *from_str = NULL;
	struct in6_addr gate = IN6ADDR_ANY_INIT;
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "from")) {
			from_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &from);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_pton(AF_INET6, argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ipv6_route_address_interface_vrf_magic(self, vty, argc, argv, no, &prefix, prefix_str, &from, from_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, label, table, table_str, nexthop_vrf);
}

/* ipv6_route => "[no] ipv6 route X:X::X:X/M$prefix [from X:X::X:X/M]           <X:X::X:X$gate|INTERFACE$ifname>                           [{                                                           tag (1-4294967295)                                         |(1-255)$distance                                          |vrf NAME                                                  |label WORD                                    	    |table (1-4294967295)                                      |nexthop-vrf NAME                                        }]" */
DEFUN_CMD_FUNC_DECL(ipv6_route)
#define funcdecl_ipv6_route static int ipv6_route_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv6 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const struct prefix_ipv6 * from,\
	const char * from_str __attribute__ ((unused)),\
	struct in6_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * vrf,\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ipv6_route;
DEFUN_CMD_FUNC_TEXT(ipv6_route)
{
#if 11 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv6 prefix = { };
	const char *prefix_str = NULL;
	struct prefix_ipv6 from = { };
	const char *from_str = NULL;
	struct in6_addr gate = IN6ADDR_ANY_INIT;
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *vrf = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "from")) {
			from_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &from);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_pton(AF_INET6, argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "vrf")) {
			vrf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ipv6_route_magic(self, vty, argc, argv, no, &prefix, prefix_str, &from, from_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, vrf, label, table, table_str, nexthop_vrf);
}

/* ipv6_route_vrf => "[no] ipv6 route X:X::X:X/M$prefix [from X:X::X:X/M]           <X:X::X:X$gate|INTERFACE$ifname>                           [{                                                           tag (1-4294967295)                                         |(1-255)$distance                                          |label WORD                                    	    |table (1-4294967295)                                      |nexthop-vrf NAME                                        }]" */
DEFUN_CMD_FUNC_DECL(ipv6_route_vrf)
#define funcdecl_ipv6_route_vrf static int ipv6_route_vrf_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv6 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const struct prefix_ipv6 * from,\
	const char * from_str __attribute__ ((unused)),\
	struct in6_addr gate,\
	const char * gate_str __attribute__ ((unused)),\
	const char * ifname,\
	long tag,\
	const char * tag_str __attribute__ ((unused)),\
	long distance,\
	const char * distance_str __attribute__ ((unused)),\
	const char * label,\
	long table,\
	const char * table_str __attribute__ ((unused)),\
	const char * nexthop_vrf)
funcdecl_ipv6_route_vrf;
DEFUN_CMD_FUNC_TEXT(ipv6_route_vrf)
{
#if 10 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv6 prefix = { };
	const char *prefix_str = NULL;
	struct prefix_ipv6 from = { };
	const char *from_str = NULL;
	struct in6_addr gate = IN6ADDR_ANY_INIT;
	const char *gate_str = NULL;
	const char *ifname = NULL;
	long tag = 0;
	const char *tag_str = NULL;
	long distance = 0;
	const char *distance_str = NULL;
	const char *label = NULL;
	long table = 0;
	const char *table_str = NULL;
	const char *nexthop_vrf = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "from")) {
			from_str = argv[_i]->arg;
			_fail = !str2prefix_ipv6(argv[_i]->arg, &from);
		}
		if (!strcmp(argv[_i]->varname, "gate")) {
			gate_str = argv[_i]->arg;
			_fail = !inet_pton(AF_INET6, argv[_i]->arg, &gate);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "tag")) {
			tag_str = argv[_i]->arg;
			char *_end;
			tag = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "distance")) {
			distance_str = argv[_i]->arg;
			char *_end;
			distance = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "label")) {
			label = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "table")) {
			table_str = argv[_i]->arg;
			char *_end;
			table = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "nexthop_vrf")) {
			nexthop_vrf = argv[_i]->arg;
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
	return ipv6_route_vrf_magic(self, vty, argc, argv, no, &prefix, prefix_str, &from, from_str, gate, gate_str, ifname, tag, tag_str, distance, distance_str, label, table, table_str, nexthop_vrf);
}

