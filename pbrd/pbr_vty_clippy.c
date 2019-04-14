/* pbr_set_table_range => "[no] pbr table range (10000-4294966272)$lb (10000-4294966272)$ub" */
DEFUN_CMD_FUNC_DECL(pbr_set_table_range)
#define funcdecl_pbr_set_table_range static int pbr_set_table_range_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long lb,\
	const char * lb_str __attribute__ ((unused)),\
	long ub,\
	const char * ub_str __attribute__ ((unused)))
funcdecl_pbr_set_table_range;
DEFUN_CMD_FUNC_TEXT(pbr_set_table_range)
{
#if 3 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long lb = 0;
	const char *lb_str = NULL;
	long ub = 0;
	const char *ub_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "lb")) {
			lb_str = argv[_i]->arg;
			char *_end;
			lb = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "ub")) {
			ub_str = argv[_i]->arg;
			char *_end;
			ub = strtol(argv[_i]->arg, &_end, 10);
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
	return pbr_set_table_range_magic(self, vty, argc, argv, no, lb, lb_str, ub, ub_str);
}

/* pbr_map_match_src => "[no] match src-ip <A.B.C.D/M|X:X::X:X/M>$prefix" */
DEFUN_CMD_FUNC_DECL(pbr_map_match_src)
#define funcdecl_pbr_map_match_src static int pbr_map_match_src_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix * prefix,\
	const char * prefix_str __attribute__ ((unused)))
funcdecl_pbr_map_match_src;
DEFUN_CMD_FUNC_TEXT(pbr_map_match_src)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix prefix = { };
	const char *prefix_str = NULL;

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
			_fail = !str2prefix(argv[_i]->arg, &prefix);
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
	return pbr_map_match_src_magic(self, vty, argc, argv, no, &prefix, prefix_str);
}

/* pbr_map_match_dst => "[no] match dst-ip <A.B.C.D/M|X:X::X:X/M>$prefix" */
DEFUN_CMD_FUNC_DECL(pbr_map_match_dst)
#define funcdecl_pbr_map_match_dst static int pbr_map_match_dst_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const struct prefix * prefix,\
	const char * prefix_str __attribute__ ((unused)))
funcdecl_pbr_map_match_dst;
DEFUN_CMD_FUNC_TEXT(pbr_map_match_dst)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct prefix prefix = { };
	const char *prefix_str = NULL;

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
			_fail = !str2prefix(argv[_i]->arg, &prefix);
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
	return pbr_map_match_dst_magic(self, vty, argc, argv, no, &prefix, prefix_str);
}

/* pbr_map_nexthop_group => "[no] set nexthop-group NAME$name" */
DEFUN_CMD_FUNC_DECL(pbr_map_nexthop_group)
#define funcdecl_pbr_map_nexthop_group static int pbr_map_nexthop_group_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * name)
funcdecl_pbr_map_nexthop_group;
DEFUN_CMD_FUNC_TEXT(pbr_map_nexthop_group)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *name = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "name")) {
			name = argv[_i]->arg;
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
	return pbr_map_nexthop_group_magic(self, vty, argc, argv, no, name);
}

/* pbr_map_nexthop => "[no] set nexthop <A.B.C.D|X:X::X:X>$addr [INTERFACE]$intf [nexthop-vrf NAME$name]" */
DEFUN_CMD_FUNC_DECL(pbr_map_nexthop)
#define funcdecl_pbr_map_nexthop static int pbr_map_nexthop_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const union sockunion * addr,\
	const char * addr_str __attribute__ ((unused)),\
	const char * intf,\
	const char * name)
funcdecl_pbr_map_nexthop;
DEFUN_CMD_FUNC_TEXT(pbr_map_nexthop)
{
#if 4 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	union sockunion s__addr = { .sa.sa_family = AF_UNSPEC }, *addr = NULL;
	const char *addr_str = NULL;
	const char *intf = NULL;
	const char *name = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "addr")) {
			addr_str = argv[_i]->arg;
			if (argv[_i]->text[0] == 'X') {
				s__addr.sa.sa_family = AF_INET6;
				_fail = !inet_pton(AF_INET6, argv[_i]->arg, &s__addr.sin6.sin6_addr);
				addr = &s__addr;
			} else {
				s__addr.sa.sa_family = AF_INET;
				_fail = !inet_aton(argv[_i]->arg, &s__addr.sin.sin_addr);
				addr = &s__addr;
			}
		}
		if (!strcmp(argv[_i]->varname, "intf")) {
			intf = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "name")) {
			name = argv[_i]->arg;
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
	return pbr_map_nexthop_magic(self, vty, argc, argv, no, addr, addr_str, intf, name);
}

/* pbr_policy => "[no] pbr-policy NAME$mapname" */
DEFUN_CMD_FUNC_DECL(pbr_policy)
#define funcdecl_pbr_policy static int pbr_policy_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * mapname)
funcdecl_pbr_policy;
DEFUN_CMD_FUNC_TEXT(pbr_policy)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *mapname = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mapname")) {
			mapname = argv[_i]->arg;
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
	return pbr_policy_magic(self, vty, argc, argv, no, mapname);
}

/* show_pbr => "show pbr" */
DEFUN_CMD_FUNC_DECL(show_pbr)
#define funcdecl_show_pbr static int show_pbr_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)))
funcdecl_show_pbr;
DEFUN_CMD_FUNC_TEXT(show_pbr)
{
#if 0 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

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
	return show_pbr_magic(self, vty, argc, argv);
}

/* show_pbr_map => "show pbr map [NAME$name] [detail$detail]" */
DEFUN_CMD_FUNC_DECL(show_pbr_map)
#define funcdecl_show_pbr_map static int show_pbr_map_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * name,\
	const char * detail)
funcdecl_show_pbr_map;
DEFUN_CMD_FUNC_TEXT(show_pbr_map)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *name = NULL;
	const char *detail = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "name")) {
			name = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "detail")) {
			detail = argv[_i]->arg;
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
	return show_pbr_map_magic(self, vty, argc, argv, name, detail);
}

/* show_pbr_nexthop_group => "show pbr nexthop-groups [WORD$word]" */
DEFUN_CMD_FUNC_DECL(show_pbr_nexthop_group)
#define funcdecl_show_pbr_nexthop_group static int show_pbr_nexthop_group_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * word)
funcdecl_show_pbr_nexthop_group;
DEFUN_CMD_FUNC_TEXT(show_pbr_nexthop_group)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *word = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "word")) {
			word = argv[_i]->arg;
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
	return show_pbr_nexthop_group_magic(self, vty, argc, argv, word);
}

/* show_pbr_interface => "show pbr interface [NAME$name]" */
DEFUN_CMD_FUNC_DECL(show_pbr_interface)
#define funcdecl_show_pbr_interface static int show_pbr_interface_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * name)
funcdecl_show_pbr_interface;
DEFUN_CMD_FUNC_TEXT(show_pbr_interface)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *name = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "name")) {
			name = argv[_i]->arg;
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
	return show_pbr_interface_magic(self, vty, argc, argv, name);
}

/* debug_pbr => "[no] debug pbr [{map$map|zebra$zebra|nht$nht|events$events}]" */
DEFUN_CMD_FUNC_DECL(debug_pbr)
#define funcdecl_debug_pbr static int debug_pbr_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * map,\
	const char * zebra,\
	const char * nht,\
	const char * events)
funcdecl_debug_pbr;
DEFUN_CMD_FUNC_TEXT(debug_pbr)
{
#if 5 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *map = NULL;
	const char *zebra = NULL;
	const char *nht = NULL;
	const char *events = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "map")) {
			map = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "zebra")) {
			zebra = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "nht")) {
			nht = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "events")) {
			events = argv[_i]->arg;
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
	return debug_pbr_magic(self, vty, argc, argv, no, map, zebra, nht, events);
}

