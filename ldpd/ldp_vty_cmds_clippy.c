/* no_ldp_mpls_ldp => "no mpls ldp" */
DEFUN_CMD_FUNC_DECL(no_ldp_mpls_ldp)
#define funcdecl_no_ldp_mpls_ldp static int no_ldp_mpls_ldp_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)))
funcdecl_no_ldp_mpls_ldp;
DEFUN_CMD_FUNC_TEXT(no_ldp_mpls_ldp)
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
	return no_ldp_mpls_ldp_magic(self, vty, argc, argv);
}

/* no_ldp_l2vpn => "no l2vpn WORD$l2vpn_name type vpls" */
DEFUN_CMD_FUNC_DECL(no_ldp_l2vpn)
#define funcdecl_no_ldp_l2vpn static int no_ldp_l2vpn_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * l2vpn_name)
funcdecl_no_ldp_l2vpn;
DEFUN_CMD_FUNC_TEXT(no_ldp_l2vpn)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *l2vpn_name = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "l2vpn_name")) {
			l2vpn_name = argv[_i]->arg;
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
	return no_ldp_l2vpn_magic(self, vty, argc, argv, l2vpn_name);
}

/* no_ldp_address_family => "no address-family <ipv4|ipv6>$af" */
DEFUN_CMD_FUNC_DECL(no_ldp_address_family)
#define funcdecl_no_ldp_address_family static int no_ldp_address_family_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * af)
funcdecl_no_ldp_address_family;
DEFUN_CMD_FUNC_TEXT(no_ldp_address_family)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *af = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "af")) {
			af = argv[_i]->arg;
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
	return no_ldp_address_family_magic(self, vty, argc, argv, af);
}

/* ldp_discovery_link_holdtime => "[no] discovery hello holdtime (1-65535)$holdtime" */
DEFUN_CMD_FUNC_DECL(ldp_discovery_link_holdtime)
#define funcdecl_ldp_discovery_link_holdtime static int ldp_discovery_link_holdtime_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long holdtime,\
	const char * holdtime_str __attribute__ ((unused)))
funcdecl_ldp_discovery_link_holdtime;
DEFUN_CMD_FUNC_TEXT(ldp_discovery_link_holdtime)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long holdtime = 0;
	const char *holdtime_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "holdtime")) {
			holdtime_str = argv[_i]->arg;
			char *_end;
			holdtime = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_discovery_link_holdtime_magic(self, vty, argc, argv, no, holdtime, holdtime_str);
}

/* ldp_discovery_targeted_holdtime => "[no] discovery targeted-hello holdtime (1-65535)$holdtime" */
DEFUN_CMD_FUNC_DECL(ldp_discovery_targeted_holdtime)
#define funcdecl_ldp_discovery_targeted_holdtime static int ldp_discovery_targeted_holdtime_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long holdtime,\
	const char * holdtime_str __attribute__ ((unused)))
funcdecl_ldp_discovery_targeted_holdtime;
DEFUN_CMD_FUNC_TEXT(ldp_discovery_targeted_holdtime)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long holdtime = 0;
	const char *holdtime_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "holdtime")) {
			holdtime_str = argv[_i]->arg;
			char *_end;
			holdtime = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_discovery_targeted_holdtime_magic(self, vty, argc, argv, no, holdtime, holdtime_str);
}

/* ldp_discovery_link_interval => "[no] discovery hello interval (1-65535)$interval" */
DEFUN_CMD_FUNC_DECL(ldp_discovery_link_interval)
#define funcdecl_ldp_discovery_link_interval static int ldp_discovery_link_interval_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long interval,\
	const char * interval_str __attribute__ ((unused)))
funcdecl_ldp_discovery_link_interval;
DEFUN_CMD_FUNC_TEXT(ldp_discovery_link_interval)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long interval = 0;
	const char *interval_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "interval")) {
			interval_str = argv[_i]->arg;
			char *_end;
			interval = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_discovery_link_interval_magic(self, vty, argc, argv, no, interval, interval_str);
}

/* ldp_discovery_targeted_interval => "[no] discovery targeted-hello interval (1-65535)$interval" */
DEFUN_CMD_FUNC_DECL(ldp_discovery_targeted_interval)
#define funcdecl_ldp_discovery_targeted_interval static int ldp_discovery_targeted_interval_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long interval,\
	const char * interval_str __attribute__ ((unused)))
funcdecl_ldp_discovery_targeted_interval;
DEFUN_CMD_FUNC_TEXT(ldp_discovery_targeted_interval)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long interval = 0;
	const char *interval_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "interval")) {
			interval_str = argv[_i]->arg;
			char *_end;
			interval = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_discovery_targeted_interval_magic(self, vty, argc, argv, no, interval, interval_str);
}

/* ldp_dual_stack_transport_connection_prefer_ipv4 => "[no] dual-stack transport-connection prefer ipv4" */
DEFUN_CMD_FUNC_DECL(ldp_dual_stack_transport_connection_prefer_ipv4)
#define funcdecl_ldp_dual_stack_transport_connection_prefer_ipv4 static int ldp_dual_stack_transport_connection_prefer_ipv4_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no)
funcdecl_ldp_dual_stack_transport_connection_prefer_ipv4;
DEFUN_CMD_FUNC_TEXT(ldp_dual_stack_transport_connection_prefer_ipv4)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
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
	return ldp_dual_stack_transport_connection_prefer_ipv4_magic(self, vty, argc, argv, no);
}

/* ldp_dual_stack_cisco_interop => "[no] dual-stack cisco-interop" */
DEFUN_CMD_FUNC_DECL(ldp_dual_stack_cisco_interop)
#define funcdecl_ldp_dual_stack_cisco_interop static int ldp_dual_stack_cisco_interop_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no)
funcdecl_ldp_dual_stack_cisco_interop;
DEFUN_CMD_FUNC_TEXT(ldp_dual_stack_cisco_interop)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
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
	return ldp_dual_stack_cisco_interop_magic(self, vty, argc, argv, no);
}

/* ldp_neighbor_password => "[no] neighbor A.B.C.D$neighbor password WORD$password" */
DEFUN_CMD_FUNC_DECL(ldp_neighbor_password)
#define funcdecl_ldp_neighbor_password static int ldp_neighbor_password_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in_addr neighbor,\
	const char * neighbor_str __attribute__ ((unused)),\
	const char * password)
funcdecl_ldp_neighbor_password;
DEFUN_CMD_FUNC_TEXT(ldp_neighbor_password)
{
#if 3 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in_addr neighbor = { INADDR_ANY };
	const char *neighbor_str = NULL;
	const char *password = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "neighbor")) {
			neighbor_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &neighbor);
		}
		if (!strcmp(argv[_i]->varname, "password")) {
			password = argv[_i]->arg;
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
	return ldp_neighbor_password_magic(self, vty, argc, argv, no, neighbor, neighbor_str, password);
}

/* ldp_neighbor_session_holdtime => "[no] neighbor A.B.C.D$neighbor session holdtime (15-65535)$holdtime" */
DEFUN_CMD_FUNC_DECL(ldp_neighbor_session_holdtime)
#define funcdecl_ldp_neighbor_session_holdtime static int ldp_neighbor_session_holdtime_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in_addr neighbor,\
	const char * neighbor_str __attribute__ ((unused)),\
	long holdtime,\
	const char * holdtime_str __attribute__ ((unused)))
funcdecl_ldp_neighbor_session_holdtime;
DEFUN_CMD_FUNC_TEXT(ldp_neighbor_session_holdtime)
{
#if 3 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in_addr neighbor = { INADDR_ANY };
	const char *neighbor_str = NULL;
	long holdtime = 0;
	const char *holdtime_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "neighbor")) {
			neighbor_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &neighbor);
		}
		if (!strcmp(argv[_i]->varname, "holdtime")) {
			holdtime_str = argv[_i]->arg;
			char *_end;
			holdtime = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_neighbor_session_holdtime_magic(self, vty, argc, argv, no, neighbor, neighbor_str, holdtime, holdtime_str);
}

/* ldp_neighbor_ttl_security => "[no] neighbor A.B.C.D$neighbor ttl-security <disable|hops (1-254)$hops>" */
DEFUN_CMD_FUNC_DECL(ldp_neighbor_ttl_security)
#define funcdecl_ldp_neighbor_ttl_security static int ldp_neighbor_ttl_security_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in_addr neighbor,\
	const char * neighbor_str __attribute__ ((unused)),\
	long hops,\
	const char * hops_str __attribute__ ((unused)))
funcdecl_ldp_neighbor_ttl_security;
DEFUN_CMD_FUNC_TEXT(ldp_neighbor_ttl_security)
{
#if 3 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in_addr neighbor = { INADDR_ANY };
	const char *neighbor_str = NULL;
	long hops = 0;
	const char *hops_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "neighbor")) {
			neighbor_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &neighbor);
		}
		if (!strcmp(argv[_i]->varname, "hops")) {
			hops_str = argv[_i]->arg;
			char *_end;
			hops = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_neighbor_ttl_security_magic(self, vty, argc, argv, no, neighbor, neighbor_str, hops, hops_str);
}

/* ldp_router_id => "[no] router-id A.B.C.D$address" */
DEFUN_CMD_FUNC_DECL(ldp_router_id)
#define funcdecl_ldp_router_id static int ldp_router_id_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in_addr address,\
	const char * address_str __attribute__ ((unused)))
funcdecl_ldp_router_id;
DEFUN_CMD_FUNC_TEXT(ldp_router_id)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in_addr address = { INADDR_ANY };
	const char *address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &address);
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
	return ldp_router_id_magic(self, vty, argc, argv, no, address, address_str);
}

/* ldp_discovery_targeted_hello_accept => "[no] discovery targeted-hello accept [from <(1-199)|(1300-2699)|WORD>$from_acl]" */
DEFUN_CMD_FUNC_DECL(ldp_discovery_targeted_hello_accept)
#define funcdecl_ldp_discovery_targeted_hello_accept static int ldp_discovery_targeted_hello_accept_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * from_acl)
funcdecl_ldp_discovery_targeted_hello_accept;
DEFUN_CMD_FUNC_TEXT(ldp_discovery_targeted_hello_accept)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *from_acl = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "from_acl")) {
			from_acl = argv[_i]->arg;
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
	return ldp_discovery_targeted_hello_accept_magic(self, vty, argc, argv, no, from_acl);
}

/* ldp_discovery_transport_address_ipv4 => "[no] discovery transport-address A.B.C.D$address" */
DEFUN_CMD_FUNC_DECL(ldp_discovery_transport_address_ipv4)
#define funcdecl_ldp_discovery_transport_address_ipv4 static int ldp_discovery_transport_address_ipv4_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in_addr address,\
	const char * address_str __attribute__ ((unused)))
funcdecl_ldp_discovery_transport_address_ipv4;
DEFUN_CMD_FUNC_TEXT(ldp_discovery_transport_address_ipv4)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in_addr address = { INADDR_ANY };
	const char *address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &address);
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
	return ldp_discovery_transport_address_ipv4_magic(self, vty, argc, argv, no, address, address_str);
}

/* ldp_discovery_transport_address_ipv6 => "[no] discovery transport-address X:X::X:X$address" */
DEFUN_CMD_FUNC_DECL(ldp_discovery_transport_address_ipv6)
#define funcdecl_ldp_discovery_transport_address_ipv6 static int ldp_discovery_transport_address_ipv6_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in6_addr address,\
	const char * address_str __attribute__ ((unused)))
funcdecl_ldp_discovery_transport_address_ipv6;
DEFUN_CMD_FUNC_TEXT(ldp_discovery_transport_address_ipv6)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in6_addr address = IN6ADDR_ANY_INIT;
	const char *address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			_fail = !inet_pton(AF_INET6, argv[_i]->arg, &address);
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
	return ldp_discovery_transport_address_ipv6_magic(self, vty, argc, argv, no, address, address_str);
}

/* ldp_label_local_advertise => "[no] label local advertise [{to <(1-199)|(1300-2699)|WORD>$to_acl|for <(1-199)|(1300-2699)|WORD>$for_acl}]" */
DEFUN_CMD_FUNC_DECL(ldp_label_local_advertise)
#define funcdecl_ldp_label_local_advertise static int ldp_label_local_advertise_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * to_acl,\
	const char * for_acl)
funcdecl_ldp_label_local_advertise;
DEFUN_CMD_FUNC_TEXT(ldp_label_local_advertise)
{
#if 3 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *to_acl = NULL;
	const char *for_acl = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "to_acl")) {
			to_acl = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "for_acl")) {
			for_acl = argv[_i]->arg;
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
	return ldp_label_local_advertise_magic(self, vty, argc, argv, no, to_acl, for_acl);
}

/* ldp_label_local_advertise_explicit_null => "[no] label local advertise explicit-null [for <(1-199)|(1300-2699)|WORD>$for_acl]" */
DEFUN_CMD_FUNC_DECL(ldp_label_local_advertise_explicit_null)
#define funcdecl_ldp_label_local_advertise_explicit_null static int ldp_label_local_advertise_explicit_null_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * for_acl)
funcdecl_ldp_label_local_advertise_explicit_null;
DEFUN_CMD_FUNC_TEXT(ldp_label_local_advertise_explicit_null)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *for_acl = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "for_acl")) {
			for_acl = argv[_i]->arg;
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
	return ldp_label_local_advertise_explicit_null_magic(self, vty, argc, argv, no, for_acl);
}

/* ldp_label_local_allocate => "[no] label local allocate <host-routes$host_routes|for <(1-199)|(1300-2699)|WORD>$for_acl>" */
DEFUN_CMD_FUNC_DECL(ldp_label_local_allocate)
#define funcdecl_ldp_label_local_allocate static int ldp_label_local_allocate_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * host_routes,\
	const char * for_acl)
funcdecl_ldp_label_local_allocate;
DEFUN_CMD_FUNC_TEXT(ldp_label_local_allocate)
{
#if 3 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *host_routes = NULL;
	const char *for_acl = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "host_routes")) {
			host_routes = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "for_acl")) {
			for_acl = argv[_i]->arg;
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
	return ldp_label_local_allocate_magic(self, vty, argc, argv, no, host_routes, for_acl);
}

/* ldp_label_remote_accept => "[no] label remote accept {from <(1-199)|(1300-2699)|WORD>$from_acl|for <(1-199)|(1300-2699)|WORD>$for_acl}" */
DEFUN_CMD_FUNC_DECL(ldp_label_remote_accept)
#define funcdecl_ldp_label_remote_accept static int ldp_label_remote_accept_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * from_acl,\
	const char * for_acl)
funcdecl_ldp_label_remote_accept;
DEFUN_CMD_FUNC_TEXT(ldp_label_remote_accept)
{
#if 3 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *from_acl = NULL;
	const char *for_acl = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "from_acl")) {
			from_acl = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "for_acl")) {
			for_acl = argv[_i]->arg;
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
	return ldp_label_remote_accept_magic(self, vty, argc, argv, no, from_acl, for_acl);
}

/* ldp_ttl_security_disable => "[no] ttl-security disable" */
DEFUN_CMD_FUNC_DECL(ldp_ttl_security_disable)
#define funcdecl_ldp_ttl_security_disable static int ldp_ttl_security_disable_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no)
funcdecl_ldp_ttl_security_disable;
DEFUN_CMD_FUNC_TEXT(ldp_ttl_security_disable)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
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
	return ldp_ttl_security_disable_magic(self, vty, argc, argv, no);
}

/* ldp_session_holdtime => "[no] session holdtime (15-65535)$holdtime" */
DEFUN_CMD_FUNC_DECL(ldp_session_holdtime)
#define funcdecl_ldp_session_holdtime static int ldp_session_holdtime_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long holdtime,\
	const char * holdtime_str __attribute__ ((unused)))
funcdecl_ldp_session_holdtime;
DEFUN_CMD_FUNC_TEXT(ldp_session_holdtime)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long holdtime = 0;
	const char *holdtime_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "holdtime")) {
			holdtime_str = argv[_i]->arg;
			char *_end;
			holdtime = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_session_holdtime_magic(self, vty, argc, argv, no, holdtime, holdtime_str);
}

/* no_ldp_interface => "no interface IFNAME$ifname" */
DEFUN_CMD_FUNC_DECL(no_ldp_interface)
#define funcdecl_no_ldp_interface static int no_ldp_interface_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ifname)
funcdecl_no_ldp_interface;
DEFUN_CMD_FUNC_TEXT(no_ldp_interface)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ifname = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
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
	return no_ldp_interface_magic(self, vty, argc, argv, ifname);
}

/* ldp_neighbor_ipv4_targeted => "[no] neighbor A.B.C.D$address targeted" */
DEFUN_CMD_FUNC_DECL(ldp_neighbor_ipv4_targeted)
#define funcdecl_ldp_neighbor_ipv4_targeted static int ldp_neighbor_ipv4_targeted_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in_addr address,\
	const char * address_str __attribute__ ((unused)))
funcdecl_ldp_neighbor_ipv4_targeted;
DEFUN_CMD_FUNC_TEXT(ldp_neighbor_ipv4_targeted)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in_addr address = { INADDR_ANY };
	const char *address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &address);
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
	return ldp_neighbor_ipv4_targeted_magic(self, vty, argc, argv, no, address, address_str);
}

/* ldp_neighbor_ipv6_targeted => "[no] neighbor X:X::X:X$address targeted" */
DEFUN_CMD_FUNC_DECL(ldp_neighbor_ipv6_targeted)
#define funcdecl_ldp_neighbor_ipv6_targeted static int ldp_neighbor_ipv6_targeted_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in6_addr address,\
	const char * address_str __attribute__ ((unused)))
funcdecl_ldp_neighbor_ipv6_targeted;
DEFUN_CMD_FUNC_TEXT(ldp_neighbor_ipv6_targeted)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in6_addr address = IN6ADDR_ANY_INIT;
	const char *address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			_fail = !inet_pton(AF_INET6, argv[_i]->arg, &address);
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
	return ldp_neighbor_ipv6_targeted_magic(self, vty, argc, argv, no, address, address_str);
}

/* show_mac_bridge_domain => "show mac-address bridge (3-4094)" */
DEFUN_CMD_FUNC_DECL(show_mac_bridge_domain)
#define funcdecl_show_mac_bridge_domain static int show_mac_bridge_domain_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	long bridge,\
	const char * bridge_str __attribute__ ((unused)))
funcdecl_show_mac_bridge_domain;
DEFUN_CMD_FUNC_TEXT(show_mac_bridge_domain)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	long bridge = 0;
	const char *bridge_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "bridge")) {
			bridge_str = argv[_i]->arg;
			char *_end;
			bridge = strtol(argv[_i]->arg, &_end, 10);
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
	return show_mac_bridge_domain_magic(self, vty, argc, argv, bridge, bridge_str);
}

/* l2fib_flush_mac_bridge_domain => "l2fib flush-mac bridge (3-4094)" */
DEFUN_CMD_FUNC_DECL(l2fib_flush_mac_bridge_domain)
#define funcdecl_l2fib_flush_mac_bridge_domain static int l2fib_flush_mac_bridge_domain_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	long bridge,\
	const char * bridge_str __attribute__ ((unused)))
funcdecl_l2fib_flush_mac_bridge_domain;
DEFUN_CMD_FUNC_TEXT(l2fib_flush_mac_bridge_domain)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	long bridge = 0;
	const char *bridge_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "bridge")) {
			bridge_str = argv[_i]->arg;
			char *_end;
			bridge = strtol(argv[_i]->arg, &_end, 10);
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
	return l2fib_flush_mac_bridge_domain_magic(self, vty, argc, argv, bridge, bridge_str);
}

/* ldp_create_bridge => "create bridge (3-4094)" */
DEFUN_CMD_FUNC_DECL(ldp_create_bridge)
#define funcdecl_ldp_create_bridge static int ldp_create_bridge_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	long bridge,\
	const char * bridge_str __attribute__ ((unused)))
funcdecl_ldp_create_bridge;
DEFUN_CMD_FUNC_TEXT(ldp_create_bridge)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	long bridge = 0;
	const char *bridge_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "bridge")) {
			bridge_str = argv[_i]->arg;
			char *_end;
			bridge = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_create_bridge_magic(self, vty, argc, argv, bridge, bridge_str);
}

/* no_ldp_create_bridge => "no create bridge (3-4094)" */
DEFUN_CMD_FUNC_DECL(no_ldp_create_bridge)
#define funcdecl_no_ldp_create_bridge static int no_ldp_create_bridge_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	long bridge,\
	const char * bridge_str __attribute__ ((unused)))
funcdecl_no_ldp_create_bridge;
DEFUN_CMD_FUNC_TEXT(no_ldp_create_bridge)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	long bridge = 0;
	const char *bridge_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "bridge")) {
			bridge_str = argv[_i]->arg;
			char *_end;
			bridge = strtol(argv[_i]->arg, &_end, 10);
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
	return no_ldp_create_bridge_magic(self, vty, argc, argv, bridge, bridge_str);
}

/* ldp_create_pseudowire => "create pseudowire (0-12288)" */
DEFUN_CMD_FUNC_DECL(ldp_create_pseudowire)
#define funcdecl_ldp_create_pseudowire static int ldp_create_pseudowire_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	long pseudowire,\
	const char * pseudowire_str __attribute__ ((unused)))
funcdecl_ldp_create_pseudowire;
DEFUN_CMD_FUNC_TEXT(ldp_create_pseudowire)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	long pseudowire = 0;
	const char *pseudowire_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "pseudowire")) {
			pseudowire_str = argv[_i]->arg;
			char *_end;
			pseudowire = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_create_pseudowire_magic(self, vty, argc, argv, pseudowire, pseudowire_str);
}

/* no_ldp_create_pseudowire => "no create pseudowire (0-12288)" */
DEFUN_CMD_FUNC_DECL(no_ldp_create_pseudowire)
#define funcdecl_no_ldp_create_pseudowire static int no_ldp_create_pseudowire_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	long pseudowire,\
	const char * pseudowire_str __attribute__ ((unused)))
funcdecl_no_ldp_create_pseudowire;
DEFUN_CMD_FUNC_TEXT(no_ldp_create_pseudowire)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	long pseudowire = 0;
	const char *pseudowire_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "pseudowire")) {
			pseudowire_str = argv[_i]->arg;
			char *_end;
			pseudowire = strtol(argv[_i]->arg, &_end, 10);
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
	return no_ldp_create_pseudowire_magic(self, vty, argc, argv, pseudowire, pseudowire_str);
}

/* ldp_mtu => "[no] mtu (1500-9180)$mtu" */
DEFUN_CMD_FUNC_DECL(ldp_mtu)
#define funcdecl_ldp_mtu static int ldp_mtu_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long mtu,\
	const char * mtu_str __attribute__ ((unused)))
funcdecl_ldp_mtu;
DEFUN_CMD_FUNC_TEXT(ldp_mtu)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long mtu = 0;
	const char *mtu_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "mtu")) {
			mtu_str = argv[_i]->arg;
			char *_end;
			mtu = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_mtu_magic(self, vty, argc, argv, no, mtu, mtu_str);
}

/* ldp_bridge => "[no] bridge IFNAME$ifname" */
DEFUN_CMD_FUNC_DECL(ldp_bridge)
#define funcdecl_ldp_bridge static int ldp_bridge_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * ifname)
funcdecl_ldp_bridge;
DEFUN_CMD_FUNC_TEXT(ldp_bridge)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *ifname = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
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
	return ldp_bridge_magic(self, vty, argc, argv, no, ifname);
}

/* ldp_member_interface => "[no] member interface IFNAME$ifname" */
DEFUN_CMD_FUNC_DECL(ldp_member_interface)
#define funcdecl_ldp_member_interface static int ldp_member_interface_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * ifname)
funcdecl_ldp_member_interface;
DEFUN_CMD_FUNC_TEXT(ldp_member_interface)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *ifname = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
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
	return ldp_member_interface_magic(self, vty, argc, argv, no, ifname);
}

/* ldp_member_interface_shg => "[no] member interface IFNAME$ifname split-horizon group (1-1000)" */
DEFUN_CMD_FUNC_DECL(ldp_member_interface_shg)
#define funcdecl_ldp_member_interface_shg static int ldp_member_interface_shg_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * ifname,\
	long group,\
	const char * group_str __attribute__ ((unused)))
funcdecl_ldp_member_interface_shg;
DEFUN_CMD_FUNC_TEXT(ldp_member_interface_shg)
{
#if 3 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *ifname = NULL;
	long group = 0;
	const char *group_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "group")) {
			group_str = argv[_i]->arg;
			char *_end;
			group = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_member_interface_shg_magic(self, vty, argc, argv, no, ifname, group, group_str);
}

/* no_ldp_member_pseudowire => "no member pseudowire IFNAME$ifname [split-horizon group (1-1000)]" */
DEFUN_CMD_FUNC_DECL(no_ldp_member_pseudowire)
#define funcdecl_no_ldp_member_pseudowire static int no_ldp_member_pseudowire_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * ifname,\
	long group,\
	const char * group_str __attribute__ ((unused)))
funcdecl_no_ldp_member_pseudowire;
DEFUN_CMD_FUNC_TEXT(no_ldp_member_pseudowire)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *ifname = NULL;
	long group = 0;
	const char *group_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "group")) {
			group_str = argv[_i]->arg;
			char *_end;
			group = strtol(argv[_i]->arg, &_end, 10);
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
	return no_ldp_member_pseudowire_magic(self, vty, argc, argv, ifname, group, group_str);
}

/* ldp_vc_type => "[no] vc type <ethernet|ethernet-tagged>$vc_type" */
DEFUN_CMD_FUNC_DECL(ldp_vc_type)
#define funcdecl_ldp_vc_type static int ldp_vc_type_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * vc_type)
funcdecl_ldp_vc_type;
DEFUN_CMD_FUNC_TEXT(ldp_vc_type)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *vc_type = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vc_type")) {
			vc_type = argv[_i]->arg;
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
	return ldp_vc_type_magic(self, vty, argc, argv, no, vc_type);
}

/* ldp_control_word => "[no] control-word <exclude|include>$preference" */
DEFUN_CMD_FUNC_DECL(ldp_control_word)
#define funcdecl_ldp_control_word static int ldp_control_word_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * preference)
funcdecl_ldp_control_word;
DEFUN_CMD_FUNC_TEXT(ldp_control_word)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *preference = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "preference")) {
			preference = argv[_i]->arg;
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
	return ldp_control_word_magic(self, vty, argc, argv, no, preference);
}

/* ldp_neighbor_address => "[no] neighbor address <A.B.C.D|X:X::X:X>$pw_address" */
DEFUN_CMD_FUNC_DECL(ldp_neighbor_address)
#define funcdecl_ldp_neighbor_address static int ldp_neighbor_address_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const union sockunion * pw_address,\
	const char * pw_address_str __attribute__ ((unused)))
funcdecl_ldp_neighbor_address;
DEFUN_CMD_FUNC_TEXT(ldp_neighbor_address)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	union sockunion s__pw_address = { .sa.sa_family = AF_UNSPEC }, *pw_address = NULL;
	const char *pw_address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "pw_address")) {
			pw_address_str = argv[_i]->arg;
			if (argv[_i]->text[0] == 'X') {
				s__pw_address.sa.sa_family = AF_INET6;
				_fail = !inet_pton(AF_INET6, argv[_i]->arg, &s__pw_address.sin6.sin6_addr);
				pw_address = &s__pw_address;
			} else {
				s__pw_address.sa.sa_family = AF_INET;
				_fail = !inet_aton(argv[_i]->arg, &s__pw_address.sin.sin_addr);
				pw_address = &s__pw_address;
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
	return ldp_neighbor_address_magic(self, vty, argc, argv, no, pw_address, pw_address_str);
}

/* ldp_neighbor_lsr_id => "[no] neighbor lsr-id A.B.C.D$address" */
DEFUN_CMD_FUNC_DECL(ldp_neighbor_lsr_id)
#define funcdecl_ldp_neighbor_lsr_id static int ldp_neighbor_lsr_id_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	struct in_addr address,\
	const char * address_str __attribute__ ((unused)))
funcdecl_ldp_neighbor_lsr_id;
DEFUN_CMD_FUNC_TEXT(ldp_neighbor_lsr_id)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	struct in_addr address = { INADDR_ANY };
	const char *address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "address")) {
			address_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &address);
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
	return ldp_neighbor_lsr_id_magic(self, vty, argc, argv, no, address, address_str);
}

/* ldp_pw_id => "[no] pw-id (1-4294967295)$pwid" */
DEFUN_CMD_FUNC_DECL(ldp_pw_id)
#define funcdecl_ldp_pw_id static int ldp_pw_id_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	long pwid,\
	const char * pwid_str __attribute__ ((unused)))
funcdecl_ldp_pw_id;
DEFUN_CMD_FUNC_TEXT(ldp_pw_id)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	long pwid = 0;
	const char *pwid_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "pwid")) {
			pwid_str = argv[_i]->arg;
			char *_end;
			pwid = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_pw_id_magic(self, vty, argc, argv, no, pwid, pwid_str);
}

/* ldp_pw_status_disable => "[no] pw-status disable" */
DEFUN_CMD_FUNC_DECL(ldp_pw_status_disable)
#define funcdecl_ldp_pw_status_disable static int ldp_pw_status_disable_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no)
funcdecl_ldp_pw_status_disable;
DEFUN_CMD_FUNC_TEXT(ldp_pw_status_disable)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
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
	return ldp_pw_status_disable_magic(self, vty, argc, argv, no);
}

/* ldp_clear_mpls_ldp_neighbor => "clear mpls ldp neighbor [<A.B.C.D|X:X::X:X>]$address" */
DEFUN_CMD_FUNC_DECL(ldp_clear_mpls_ldp_neighbor)
#define funcdecl_ldp_clear_mpls_ldp_neighbor static int ldp_clear_mpls_ldp_neighbor_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const union sockunion * address,\
	const char * address_str __attribute__ ((unused)))
funcdecl_ldp_clear_mpls_ldp_neighbor;
DEFUN_CMD_FUNC_TEXT(ldp_clear_mpls_ldp_neighbor)
{
#if 1 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	union sockunion s__address = { .sa.sa_family = AF_UNSPEC }, *address = NULL;
	const char *address_str = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

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
	return ldp_clear_mpls_ldp_neighbor_magic(self, vty, argc, argv, address, address_str);
}

/* ldp_debug_mpls_ldp_discovery_hello => "[no] debug mpls ldp discovery hello <recv|sent>$dir" */
DEFUN_CMD_FUNC_DECL(ldp_debug_mpls_ldp_discovery_hello)
#define funcdecl_ldp_debug_mpls_ldp_discovery_hello static int ldp_debug_mpls_ldp_discovery_hello_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * dir)
funcdecl_ldp_debug_mpls_ldp_discovery_hello;
DEFUN_CMD_FUNC_TEXT(ldp_debug_mpls_ldp_discovery_hello)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *dir = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "dir")) {
			dir = argv[_i]->arg;
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
	return ldp_debug_mpls_ldp_discovery_hello_magic(self, vty, argc, argv, no, dir);
}

/* ldp_debug_mpls_ldp_type => "[no] debug mpls ldp <errors|event|labels|zebra>$type" */
DEFUN_CMD_FUNC_DECL(ldp_debug_mpls_ldp_type)
#define funcdecl_ldp_debug_mpls_ldp_type static int ldp_debug_mpls_ldp_type_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * type)
funcdecl_ldp_debug_mpls_ldp_type;
DEFUN_CMD_FUNC_TEXT(ldp_debug_mpls_ldp_type)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *type = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "type")) {
			type = argv[_i]->arg;
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
	return ldp_debug_mpls_ldp_type_magic(self, vty, argc, argv, no, type);
}

/* ldp_debug_mpls_ldp_messages_recv => "[no] debug mpls ldp messages recv [all]$all" */
DEFUN_CMD_FUNC_DECL(ldp_debug_mpls_ldp_messages_recv)
#define funcdecl_ldp_debug_mpls_ldp_messages_recv static int ldp_debug_mpls_ldp_messages_recv_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * all)
funcdecl_ldp_debug_mpls_ldp_messages_recv;
DEFUN_CMD_FUNC_TEXT(ldp_debug_mpls_ldp_messages_recv)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *all = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "all")) {
			all = argv[_i]->arg;
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
	return ldp_debug_mpls_ldp_messages_recv_magic(self, vty, argc, argv, no, all);
}

/* ldp_debug_mpls_ldp_messages_sent => "[no] debug mpls ldp messages sent [all]$all" */
DEFUN_CMD_FUNC_DECL(ldp_debug_mpls_ldp_messages_sent)
#define funcdecl_ldp_debug_mpls_ldp_messages_sent static int ldp_debug_mpls_ldp_messages_sent_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * no,\
	const char * all)
funcdecl_ldp_debug_mpls_ldp_messages_sent;
DEFUN_CMD_FUNC_TEXT(ldp_debug_mpls_ldp_messages_sent)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *no = NULL;
	const char *all = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "no")) {
			no = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "all")) {
			all = argv[_i]->arg;
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
	return ldp_debug_mpls_ldp_messages_sent_magic(self, vty, argc, argv, no, all);
}

/* ldp_show_mpls_ldp_binding => "show mpls ldp [<ipv4|ipv6>]$af binding	  [<A.B.C.D/M|X:X::X:X/M>$prefix [longer-prefixes$longer_prefixes]]	  [{	    neighbor A.B.C.D$nbr	    |local-label (0-1048575)$local_label	    |remote-label (0-1048575)$remote_label	  }]	 [detail]$detail [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_mpls_ldp_binding)
#define funcdecl_ldp_show_mpls_ldp_binding static int ldp_show_mpls_ldp_binding_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * af,\
	const struct prefix * prefix,\
	const char * prefix_str __attribute__ ((unused)),\
	const char * longer_prefixes,\
	struct in_addr nbr,\
	const char * nbr_str __attribute__ ((unused)),\
	long local_label,\
	const char * local_label_str __attribute__ ((unused)),\
	long remote_label,\
	const char * remote_label_str __attribute__ ((unused)),\
	const char * detail,\
	const char * json)
funcdecl_ldp_show_mpls_ldp_binding;
DEFUN_CMD_FUNC_TEXT(ldp_show_mpls_ldp_binding)
{
#if 8 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *af = NULL;
	struct prefix prefix = { };
	const char *prefix_str = NULL;
	const char *longer_prefixes = NULL;
	struct in_addr nbr = { INADDR_ANY };
	const char *nbr_str = NULL;
	long local_label = 0;
	const char *local_label_str = NULL;
	long remote_label = 0;
	const char *remote_label_str = NULL;
	const char *detail = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "af")) {
			af = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "prefix")) {
			prefix_str = argv[_i]->arg;
			_fail = !str2prefix(argv[_i]->arg, &prefix);
		}
		if (!strcmp(argv[_i]->varname, "longer_prefixes")) {
			longer_prefixes = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "nbr")) {
			nbr_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &nbr);
		}
		if (!strcmp(argv[_i]->varname, "local_label")) {
			local_label_str = argv[_i]->arg;
			char *_end;
			local_label = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "remote_label")) {
			remote_label_str = argv[_i]->arg;
			char *_end;
			remote_label = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "detail")) {
			detail = argv[_i]->arg;
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
	return ldp_show_mpls_ldp_binding_magic(self, vty, argc, argv, af, &prefix, prefix_str, longer_prefixes, nbr, nbr_str, local_label, local_label_str, remote_label, remote_label_str, detail, json);
}

/* ldp_show_mpls_ldp_discovery => "show mpls ldp [<ipv4|ipv6>]$af discovery [detail]$detail [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_mpls_ldp_discovery)
#define funcdecl_ldp_show_mpls_ldp_discovery static int ldp_show_mpls_ldp_discovery_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * af,\
	const char * detail,\
	const char * json)
funcdecl_ldp_show_mpls_ldp_discovery;
DEFUN_CMD_FUNC_TEXT(ldp_show_mpls_ldp_discovery)
{
#if 3 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *af = NULL;
	const char *detail = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "af")) {
			af = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "detail")) {
			detail = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "json")) {
			json = argv[_i]->arg;
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
	return ldp_show_mpls_ldp_discovery_magic(self, vty, argc, argv, af, detail, json);
}

/* ldp_show_mpls_ldp_interface => "show mpls ldp [<ipv4|ipv6>]$af interface [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_mpls_ldp_interface)
#define funcdecl_ldp_show_mpls_ldp_interface static int ldp_show_mpls_ldp_interface_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * af,\
	const char * json)
funcdecl_ldp_show_mpls_ldp_interface;
DEFUN_CMD_FUNC_TEXT(ldp_show_mpls_ldp_interface)
{
#if 2 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *af = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "af")) {
			af = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "json")) {
			json = argv[_i]->arg;
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
	return ldp_show_mpls_ldp_interface_magic(self, vty, argc, argv, af, json);
}

/* ldp_show_mpls_ldp_capabilities => "show mpls ldp capabilities [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_mpls_ldp_capabilities)
#define funcdecl_ldp_show_mpls_ldp_capabilities static int ldp_show_mpls_ldp_capabilities_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	const char * json)
funcdecl_ldp_show_mpls_ldp_capabilities;
DEFUN_CMD_FUNC_TEXT(ldp_show_mpls_ldp_capabilities)
{
#if 1 /* anything to parse? */
	int _i;
#if 0 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 0 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "json")) {
			json = argv[_i]->arg;
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
	return ldp_show_mpls_ldp_capabilities_magic(self, vty, argc, argv, json);
}

/* ldp_show_mpls_ldp_neighbor => "show mpls ldp neighbor [A.B.C.D]$lsr_id [detail]$detail [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_mpls_ldp_neighbor)
#define funcdecl_ldp_show_mpls_ldp_neighbor static int ldp_show_mpls_ldp_neighbor_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	struct in_addr lsr_id,\
	const char * lsr_id_str __attribute__ ((unused)),\
	const char * detail,\
	const char * json)
funcdecl_ldp_show_mpls_ldp_neighbor;
DEFUN_CMD_FUNC_TEXT(ldp_show_mpls_ldp_neighbor)
{
#if 3 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	struct in_addr lsr_id = { INADDR_ANY };
	const char *lsr_id_str = NULL;
	const char *detail = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "lsr_id")) {
			lsr_id_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &lsr_id);
		}
		if (!strcmp(argv[_i]->varname, "detail")) {
			detail = argv[_i]->arg;
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
	return ldp_show_mpls_ldp_neighbor_magic(self, vty, argc, argv, lsr_id, lsr_id_str, detail, json);
}

/* ldp_show_mpls_ldp_neighbor_capabilities => "show mpls ldp neighbor [A.B.C.D]$lsr_id capabilities [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_mpls_ldp_neighbor_capabilities)
#define funcdecl_ldp_show_mpls_ldp_neighbor_capabilities static int ldp_show_mpls_ldp_neighbor_capabilities_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	struct in_addr lsr_id,\
	const char * lsr_id_str __attribute__ ((unused)),\
	const char * json)
funcdecl_ldp_show_mpls_ldp_neighbor_capabilities;
DEFUN_CMD_FUNC_TEXT(ldp_show_mpls_ldp_neighbor_capabilities)
{
#if 2 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	struct in_addr lsr_id = { INADDR_ANY };
	const char *lsr_id_str = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "lsr_id")) {
			lsr_id_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &lsr_id);
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
	return ldp_show_mpls_ldp_neighbor_capabilities_magic(self, vty, argc, argv, lsr_id, lsr_id_str, json);
}

/* ldp_show_l2vpn_atom_binding => "show l2vpn atom binding	  [{	    A.B.C.D$peer	    |local-label (16-1048575)$local_label	    |remote-label (16-1048575)$remote_label	  }]	 [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_l2vpn_atom_binding)
#define funcdecl_ldp_show_l2vpn_atom_binding static int ldp_show_l2vpn_atom_binding_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	struct in_addr peer,\
	const char * peer_str __attribute__ ((unused)),\
	long local_label,\
	const char * local_label_str __attribute__ ((unused)),\
	long remote_label,\
	const char * remote_label_str __attribute__ ((unused)),\
	const char * json)
funcdecl_ldp_show_l2vpn_atom_binding;
DEFUN_CMD_FUNC_TEXT(ldp_show_l2vpn_atom_binding)
{
#if 4 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	struct in_addr peer = { INADDR_ANY };
	const char *peer_str = NULL;
	long local_label = 0;
	const char *local_label_str = NULL;
	long remote_label = 0;
	const char *remote_label_str = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "peer")) {
			peer_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &peer);
		}
		if (!strcmp(argv[_i]->varname, "local_label")) {
			local_label_str = argv[_i]->arg;
			char *_end;
			local_label = strtol(argv[_i]->arg, &_end, 10);
			_fail = (_end == argv[_i]->arg) || (*_end != '\0');
		}
		if (!strcmp(argv[_i]->varname, "remote_label")) {
			remote_label_str = argv[_i]->arg;
			char *_end;
			remote_label = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_show_l2vpn_atom_binding_magic(self, vty, argc, argv, peer, peer_str, local_label, local_label_str, remote_label, remote_label_str, json);
}

/* ldp_show_l2vpn_atom_vc => "show l2vpn atom vc	  [{	    A.B.C.D$peer	    |interface IFNAME$ifname	    |vc-id (1-4294967295)$vcid	  }]	 [json]$json" */
DEFUN_CMD_FUNC_DECL(ldp_show_l2vpn_atom_vc)
#define funcdecl_ldp_show_l2vpn_atom_vc static int ldp_show_l2vpn_atom_vc_magic(\
	const struct cmd_element *self __attribute__ ((unused)),\
	struct vty *vty __attribute__ ((unused)),\
	int argc __attribute__ ((unused)),\
	struct cmd_token *argv[] __attribute__ ((unused)),\
	struct in_addr peer,\
	const char * peer_str __attribute__ ((unused)),\
	const char * ifname,\
	long vcid,\
	const char * vcid_str __attribute__ ((unused)),\
	const char * json)
funcdecl_ldp_show_l2vpn_atom_vc;
DEFUN_CMD_FUNC_TEXT(ldp_show_l2vpn_atom_vc)
{
#if 4 /* anything to parse? */
	int _i;
#if 1 /* anything that can fail? */
	unsigned _fail = 0, _failcnt = 0;
#endif
	struct in_addr peer = { INADDR_ANY };
	const char *peer_str = NULL;
	const char *ifname = NULL;
	long vcid = 0;
	const char *vcid_str = NULL;
	const char *json = NULL;

	for (_i = 0; _i < argc; _i++) {
		if (!argv[_i]->varname)
			continue;
#if 1 /* anything that can fail? */
		_fail = 0;
#endif

		if (!strcmp(argv[_i]->varname, "peer")) {
			peer_str = argv[_i]->arg;
			_fail = !inet_aton(argv[_i]->arg, &peer);
		}
		if (!strcmp(argv[_i]->varname, "ifname")) {
			ifname = argv[_i]->arg;
		}
		if (!strcmp(argv[_i]->varname, "vcid")) {
			vcid_str = argv[_i]->arg;
			char *_end;
			vcid = strtol(argv[_i]->arg, &_end, 10);
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
	return ldp_show_l2vpn_atom_vc_magic(self, vty, argc, argv, peer, peer_str, ifname, vcid, vcid_str, json);
}

