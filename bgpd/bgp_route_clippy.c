/* bgp_network => "[no] network 	<A.B.C.D/M$prefix|A.B.C.D$address [mask A.B.C.D$netmask]> 	[{route-map WORD$map_name|label-index (0-1048560)$label_index| 	backdoor$backdoor}]" */
DEFUN_CMD_FUNC_DECL(bgp_network)
#define funcdecl_bgp_network static int bgp_network_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv4 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	struct in_addr address,\
	const char * address_str __attribute__ ((unused)),\
	struct in_addr netmask,\
	const char * netmask_str __attribute__ ((unused)),\
	const char * map_name,\
	long label_index,\
	const char * label_index_str __attribute__ ((unused)),\
	const char * backdoor)
funcdecl_bgp_network;
DEFUN_CMD_FUNC_TEXT(bgp_network)
{
#if 7 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv4 prefix = { };
	const char *prefix_str = NULL;
	struct in_addr address = { INADDR_ANY };
	const char *address_str = NULL;
	struct in_addr netmask = { INADDR_ANY };
	const char *netmask_str = NULL;
	const char *map_name = NULL;
	long label_index = 0;
	const char *label_index_str = NULL;
	const char *backdoor = NULL;

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
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &address);
		}
		if (!strcmp(argv[_i]->varname, "netmask")) {
			netmask_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &netmask);
		}
		if (!strcmp(argv[_i]->varname, "map_name")) {
			map_name = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label_index")) {
			label_index_str = argv[_i]->arg;
			char *_end;
			label_index = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "backdoor")) {
			backdoor = argv[_i]->arg;
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
	return bgp_network_magic(self, vty, argc, argv, no, &prefix, prefix_str, address, address_str, netmask, netmask_str, map_name, label_index, label_index_str, backdoor);
}

/* ipv6_bgp_network => "[no] network X:X::X:X/M$prefix 	[{route-map WORD$map_name|label-index (0-1048560)$label_index}]" */
DEFUN_CMD_FUNC_DECL(ipv6_bgp_network)
#define funcdecl_ipv6_bgp_network static int ipv6_bgp_network_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix_ipv6 * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const char * map_name,\
	long label_index,\
	const char * label_index_str __attribute__ ((unused)))
funcdecl_ipv6_bgp_network;
DEFUN_CMD_FUNC_TEXT(ipv6_bgp_network)
{
#if 4 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix_ipv6 prefix = { };
	const char *prefix_str = NULL;
	const char *map_name = NULL;
	long label_index = 0;
	const char *label_index_str = NULL;

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
		if (!strcmp(argv[_i]->varname, "map_name")) {
			map_name = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "label_index")) {
			label_index_str = argv[_i]->arg;
			char *_end;
			label_index = strtol(argv[_i]->arg, &_end, 10);
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
	return ipv6_bgp_network_magic(self, vty, argc, argv, no, &prefix, prefix_str, map_name, label_index, label_index_str);
}

