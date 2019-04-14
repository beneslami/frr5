/* Zebra PW code
 * Copyright (C) 2016 Volta Networks, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <zebra.h>

#include "log.h"
#include "memory.h"
#include "thread.h"
#include "command.h"
#include "vrf.h"

#include "zebra/debug.h"
#include "zebra/rib.h"
#include "zebra/zserv.h"
#include "zebra/zapi_msg.h"
#include "zebra/zebra_rnh.h"
#include "zebra/zebra_vrf.h"
#include "zebra/zebra_pw.h"

DEFINE_MTYPE_STATIC(LIB, PW, "Pseudowire")

DEFINE_QOBJ_TYPE(zebra_pw)

DEFINE_HOOK(pw_install, (struct zebra_pw * pw), (pw))
DEFINE_HOOK(pw_uninstall, (struct zebra_pw * pw), (pw))

#define MPLS_NO_LABEL MPLS_INVALID_LABEL

extern struct zebra_t zebrad;

//Mehran Memarnejad
//In order to check deletion of mpls-tunnel use "vppctl show mpls tunnel" command not the "vppctl sh int" command
//"mpls tunnel del mpls-tunnel0 l2-only via 2.1.1.2" ... I think the rest of cmd is optional for deletion
void detach_iface_from_bridge(char *ifname);
void detach_tunnel_from_bridge(char *mpw);
void attach_iface_to_bridge(char *br_name, char *ifname);
void attach_tunnel_to_bridge(char *br_name, char *mpw);
void del_bridge(char *br_name);
void add_bridge(char *br_name);
void del_mpls_tunnel(struct zebra_pw *pw);
void add_mpls_tunnel(struct zebra_pw *pw);
void zebra_pw_set_mpw_bridge(struct zebra_pw *pw, char *mpls_tunnel);
char *find_tunnel(char* lsr_id_str, uint32_t outlabel);
char *create_tunnel(char* lsr_id_str, uint32_t outlabel);
char *mpw_to_mpls_tunnel(char *mpw);
char *mpls_tunnel_to_mpw(char *mpls_tunnel);
int del_association(char *mpw);
int add_association(char *mpw, char *mpls_tunnel);

void detach_iface_from_bridge(char *ifname){
    /*This function is used to detach an interface from bridge*/
    char cmd[100];
    
    sprintf(cmd, "sudo vppctl set interface l3 %s", ifname);
    printf("%s\n", cmd);
    system(cmd);
    
    return;
}

void detach_tunnel_from_bridge(char *mpw){
    
    /*This function is used to detach a mpls_tunnel from bridge*/
    char cmd[100];
    char *mpls_tunnel;
    printf("1. here is detach_tunnel_from_bridge before mpw_to_mpls_tunnel\n");
    //check to see if the tunnel has been created
    mpls_tunnel = mpw_to_mpls_tunnel(mpw);
    printf("2. here is detach_tunnel_from_bridge after mpw_to_mpls_tunnel\n");
    if(mpls_tunnel){
        sprintf(cmd, "sudo vppctl set interface l3 %s", mpls_tunnel);
        printf("%s\n", cmd);
        system(cmd);
    }
    
    return;  
}

void attach_iface_to_bridge(char *br_name, char *ifname){
    /*This function is used to detach an interface from bridge*/
    char cmd[100];
    
    //Note that bridge's name is like bridge4 so after adding br_name to 6, we can get the bridge domain number
    sprintf(cmd, "sudo vppctl set interface l2 bridge %s %s", ifname, (br_name+6));
    printf("%s\n", cmd);
    system(cmd);
    
    return;
}

void attach_tunnel_to_bridge(char *br_name, char *mpw){
    /*This function is used to detach an interface from bridge*/
    char cmd[100];
    char *mpls_tunnel;
    
    //Note that bridge's name is like bridge4 so after adding br_name to 6, we can get the bridge domain number
    //check to see if the tunnel has been created
    mpls_tunnel = mpw_to_mpls_tunnel(mpw);
    if(mpls_tunnel){
        //Split Horizon Group (SHG) for mpls tunnels are set to 1, so that we can avoid loop in full mesh scenario 
        sprintf(cmd, "sudo vppctl set interface l2 bridge %s %s 1", mpls_tunnel, (br_name+6));
        printf("%s\n", cmd);
        system(cmd);
    }

    return;
}

void del_bridge(char *br_name){
    char cmd[200];
    
    sprintf(cmd, "sudo vppctl create bridge-domain del %s", (br_name+6));
    printf("%s\n", cmd);
    system(cmd);
    
    return;
}

void add_bridge(char *br_name){
    char cmd[200];
    
    sprintf(cmd, "sudo vppctl create bridge-domain %s", (br_name+6));
    printf("%s\n", cmd);
    system(cmd);
    
    return;
}

int del_association(char *mpw){
    int i=0;
    
    printf("1.Here is del_association...\n");
    while(i<=tunnel_n && strcmp(mpw_names_frr[i],mpw)){
        printf("mpw_names_frr[i=%d] = %s\n", i, mpw_names_frr[i]);
        i++;
    }
    
    printf("mpw_names_frr[i=%d] = %s\n", i, mpw_names_frr[i]);
    
    if(i<=tunnel_n){
        int j=i;
        
        //free allocated memory
        printf("Freeing:   mpw_names_frr[i=%d] = %s\n", i, mpw_names_frr[i]);
        printf("Freeing:   mpls_tunnel_names_vpp[i=%d] = %s\n", i, mpls_tunnel_names_vpp[i]);
        free(mpw_names_frr[i]);
        free(mpls_tunnel_names_vpp[i]);
        
        while(j<tunnel_n){
            printf("Freeing:   mpw_names_frr[j=%d] = %s\n", j, mpw_names_frr[j]);
            printf("Freeing:   mpls_tunnel_names_vpp[j=%d] = %s\n", j, mpls_tunnel_names_vpp[j]);
            mpw_names_frr[j]=mpw_names_frr[j+1];
            mpls_tunnel_names_vpp[j]=mpls_tunnel_names_vpp[j+1];
            j++;
        }
        tunnel_n--;
        
        return 1;
    }
    
    return 0;
            
}

int add_association(char *mpw, char *mpls_tunnel){
    
    if(tunnel_n < VPP_TUNNEL_N){
        //increment tunnel_n (index) 
        tunnel_n++;
        mpw_names_frr[tunnel_n] = strdup(mpw);
        mpls_tunnel_names_vpp[tunnel_n] = strdup(mpls_tunnel);
        //printf("mpls_tunnel_names_vpp[%d] : %s\n", tunnel_n, mpls_tunnel_names_vpp[tunnel_n]);
        //printf("mpw_names_frr[%d] : %s\n", tunnel_n, mpw_names_frr[tunnel_n]);
        return 1;
    }
    else{
        printf("Max number of mpls tunnel... NO MORE TUNNEL NAME CAN BE SAVED!!!\n");
        return 0;
    }
}

char *mpw_to_mpls_tunnel(char *mpw){
    printf("1. here is mpw_to_mpls_tunnel .... tunnel_n = %d\n", tunnel_n);
    for(int i=0; i<=tunnel_n; i++){
        printf("2. here is mpw_to_mpls_tunnel   i=%d\n", i);
        if(!strcmp(mpw_names_frr[i],mpw))
            return mpls_tunnel_names_vpp[i];
    }
    return NULL;
}

char *mpls_tunnel_to_mpw(char *mpls_tunnel){
    
    for(int i=0; i<=tunnel_n; i++){
        if(!strcmp(mpls_tunnel_names_vpp[i],mpls_tunnel))
            return mpw_names_frr[i];
    }
    return NULL;
}

char *find_tunnel(char* lsr_id_str, uint32_t outlabel){
    
    static char tunnel_name[IF_NAMESIZE] = "";
    int LINE_BUFSIZE=64;
    char line[64];
    char cmd[512];
    FILE *pipe;
    
    
    sprintf(cmd, "sudo vppctl show mpls tunnel | awk \'BEGIN{RS=\"\\n\\\\[@\"; FS=\"\\n\"} /labels:\\[\\[%u/ && /%s/{print $0} END{ORS=\"\\n\\n\"}\' | awk \'/mpls_tunnel/ && /sw_if_index/{ print $2}\'",outlabel, lsr_id_str);
    
    pipe = popen(cmd,"r");
    if (pipe == NULL) {  /* check for errors */
        //return 1;        /* return with exit code indicating error */
        return NULL;
    }
    
    while (fgets(line, LINE_BUFSIZE, pipe) != NULL){
        if (strlen(line) > 1){
            sscanf(line, "%[^:]s\n", tunnel_name);
            tunnel_name[4]='-';
            printf("\t*** TUNNEL %s FOUND! exiting...\n", tunnel_name);
            return tunnel_name;
        }
    }
    
    //Not found
    return NULL;
}

char *create_tunnel(char* lsr_id_str, uint32_t outlabel){
    
    static char tunnel_name[IF_NAMESIZE] = "";
    int LINE_BUFSIZE=64;
    char line[64];
    char cmd[512];
    FILE *pipe;
    
    //1. Create the tunnel
    sprintf(cmd, "sudo vppctl mpls tunnel l2-only via %s out-labels %u", lsr_id_str, outlabel);
    system(cmd);
    printf("%s\n", cmd);
    printf("*** NEW TUNNEL CREATED\n");

    
    //2. Fetch the new tunnel's name
    sprintf(cmd, "sudo vppctl show mpls tunnel | awk \'BEGIN{RS=\"\\n\\\\[@\"; FS=\"\\n\"} /labels:\\[\\[%u/ && /%s/{print $0} END{ORS=\"\\n\\n\"}\' | awk \'/mpls_tunnel/ && /sw_if_index/{ print $2}\'",outlabel, lsr_id_str);
    
    
    pipe = popen(cmd,"r");
    if (pipe == NULL) {  /* check for errors */
        //return 1;        /* return with exit code indicating error */
        return NULL;
    }
    
    while (fgets(line, LINE_BUFSIZE, pipe) != NULL){
        if (strlen(line) > 1){
            sscanf(line, "%[^:]s\n", tunnel_name);
            //change _ to - in mpls-tunnel name (i.e. mpls_tunnel to mpls-tunnel) => index 4
            tunnel_name[4]='-';
            printf("*** new tunnel's NAME is %s\n", tunnel_name);
        }
    }
 
    return tunnel_name;
}

void del_mpls_tunnel(struct zebra_pw *pw){
    
    char *mpls_tunnel;
    char cmd[512];
    
    mpls_tunnel = mpw_to_mpls_tunnel(pw->ifname);

    if(mpls_tunnel){
		//delete associated local pw label
        sprintf(cmd, "sudo vppctl mpls local-label del eos %d via l2-input-on %s", pw->local_label, mpls_tunnel);
        printf("%s\n", cmd);
        system(cmd);
        //delete associated remote pw label -> They are delete when mpls-tunnel is deleted
        //detach mpls tunnel from bridge
        sprintf(cmd, "sudo vppctl set interface l3 %s", mpls_tunnel);
        printf("%s\n", cmd);
        system(cmd);
        //delete mpls tunnel
        sprintf(cmd, "sudo vppctl mpls tunnel l2-only del %s via %s", mpls_tunnel, inet_ntoa(pw->data.ldp.lsr_id));
        printf("%s\n", cmd);
        system(cmd);
        //show number of tunnels
        printf("BEFORE__ Tunnel_n: %d\n", tunnel_n);
        del_association(pw->ifname);
        printf("AFTER__ Tunnel_n: %d\n", tunnel_n);
    }
    //else
        //printf("No such a mpw named %s\n", pw->ifname);

    return;
        
}

void add_mpls_tunnel(struct zebra_pw *pw){
    
    uint32_t outlabel = pw->remote_label; 
    char* lsr_id_str = inet_ntoa(pw->data.ldp.lsr_id);
    char *tunnel_name;
    char cmd[512];
    
    tunnel_name = find_tunnel(lsr_id_str, outlabel);
    
    if(tunnel_name){
        printf("Tunnel %s exists ...", tunnel_name);
    }
    
    else{
        tunnel_name = create_tunnel(lsr_id_str, outlabel);
        
        //tunnel successfully created
        if(tunnel_name){
            //Save mpw-mpls_tunnel name association
            add_association(pw->ifname, tunnel_name);

            //We need to get the name of mpls tunnel by parsing "vppctl show mpls tunnel" difference
            //printf("sudo vppctl mpls tunnel l2-only via %s ip4-lookup-in-table 0 out-labels %u\n", inet_ntoa(pw->data.ldp.lsr_id), pw->remote_label);
            sprintf(cmd, "sudo vppctl set int state %s up\n", tunnel_name);
            printf("%s\n", cmd);
            system(cmd);
            sprintf(cmd, "sudo vppctl mpls local-label add eos %u via l2-input-on %s\n", pw->local_label, tunnel_name);
            printf("%s\n", cmd);
            system(cmd);

            //set the mpls-tunnel bridge membership
            //Here we make a message from zebra to ldpd and ask ldpd to set membership, because we don't have bridge-mpw relationship
            zebra_pw_set_mpw_bridge(pw, tunnel_name);
        }  
    }
    
    return;
}

void zebra_pw_set_mpw_bridge(struct zebra_pw *pw, char *mpls_tunnel){

    if (pw->client)
	zsend_pw_set_mpw_bridge(pw->client, pw, mpls_tunnel);
}



static int zebra_pw_enabled(struct zebra_pw *);
static void zebra_pw_install(struct zebra_pw *);
static void zebra_pw_uninstall(struct zebra_pw *);
static int zebra_pw_install_retry(struct thread *);
static int zebra_pw_check_reachability(struct zebra_pw *);
static void zebra_pw_update_status(struct zebra_pw *, int);

static inline int zebra_pw_compare(const struct zebra_pw *a,
				   const struct zebra_pw *b)
{
	return (strcmp(a->ifname, b->ifname));
}

RB_GENERATE(zebra_pw_head, zebra_pw, pw_entry, zebra_pw_compare)
RB_GENERATE(zebra_static_pw_head, zebra_pw, static_pw_entry, zebra_pw_compare)

struct zebra_pw *zebra_pw_add(struct zebra_vrf *zvrf, const char *ifname,
			      uint8_t protocol, struct zserv *client)
{
	struct zebra_pw *pw;

	if (IS_ZEBRA_DEBUG_PW)
		zlog_debug("%u: adding pseudowire %s protocol %s",
			   zvrf_id(zvrf), ifname, zebra_route_string(protocol));

	pw = XCALLOC(MTYPE_PW, sizeof(*pw));
	strlcpy(pw->ifname, ifname, sizeof(pw->ifname));
	pw->protocol = protocol;
	pw->vrf_id = zvrf_id(zvrf);
	pw->client = client;
	pw->status = PW_STATUS_DOWN;
	pw->local_label = MPLS_NO_LABEL;
	pw->remote_label = MPLS_NO_LABEL;
	pw->flags = F_PSEUDOWIRE_CWORD;

	RB_INSERT(zebra_pw_head, &zvrf->pseudowires, pw);
	if (pw->protocol == ZEBRA_ROUTE_STATIC) {
		RB_INSERT(zebra_static_pw_head, &zvrf->static_pseudowires, pw);
		QOBJ_REG(pw, zebra_pw);
	}

	return pw;
}

void zebra_pw_del(struct zebra_vrf *zvrf, struct zebra_pw *pw)
{
	if (IS_ZEBRA_DEBUG_PW)
		zlog_debug("%u: deleting pseudowire %s protocol %s", pw->vrf_id,
			   pw->ifname, zebra_route_string(pw->protocol));

	/* remove nexthop tracking */
	zebra_deregister_rnh_pseudowire(pw->vrf_id, pw);

	/* uninstall */
	if (pw->status == PW_STATUS_UP)
		hook_call(pw_uninstall, pw);
	else if (pw->install_retry_timer)
		THREAD_TIMER_OFF(pw->install_retry_timer);

	/* unlink and release memory */
	RB_REMOVE(zebra_pw_head, &zvrf->pseudowires, pw);
	if (pw->protocol == ZEBRA_ROUTE_STATIC)
		RB_REMOVE(zebra_static_pw_head, &zvrf->static_pseudowires, pw);
	XFREE(MTYPE_PW, pw);
}

void zebra_pw_change(struct zebra_pw *pw, ifindex_t ifindex, int type, int af,
		     union g_addr *nexthop, uint32_t local_label,
		     uint32_t remote_label, uint8_t flags,
		     union pw_protocol_fields *data)
{
	zebra_deregister_rnh_pseudowire(pw->vrf_id, pw);

	pw->ifindex = ifindex;
	pw->type = type;
	pw->af = af;
	pw->nexthop = *nexthop;
	pw->local_label = local_label;
	pw->remote_label = remote_label;
	pw->flags = flags;
	pw->data = *data;

	if (zebra_pw_enabled(pw))
		zebra_register_rnh_pseudowire(pw->vrf_id, pw);
	else
		zebra_pw_uninstall(pw);
}

struct zebra_pw *zebra_pw_find(struct zebra_vrf *zvrf, const char *ifname)
{
	struct zebra_pw pw;
	strlcpy(pw.ifname, ifname, sizeof(pw.ifname));
	return (RB_FIND(zebra_pw_head, &zvrf->pseudowires, &pw));
}

static int zebra_pw_enabled(struct zebra_pw *pw)
{
	if (pw->protocol == ZEBRA_ROUTE_STATIC) {
		if (pw->local_label == MPLS_NO_LABEL
		    || pw->remote_label == MPLS_NO_LABEL || pw->af == AF_UNSPEC)
			return 0;
		return 1;
	} else
		return pw->enabled;
}

void zebra_pw_update(struct zebra_pw *pw)
{
	if (zebra_pw_check_reachability(pw) < 0) {
		zebra_pw_uninstall(pw);
		/* wait for NHT and try again later */
	} else {
		/*
		 * Install or reinstall the pseudowire (e.g. to update
		 * parameters like the nexthop or the use of the control word).
		 */
		zebra_pw_install(pw);
	}
}

static void zebra_pw_install(struct zebra_pw *pw)
{
	if (IS_ZEBRA_DEBUG_PW)
		zlog_debug("%u: installing pseudowire %s protocol %s",
			   pw->vrf_id, pw->ifname,
			   zebra_route_string(pw->protocol));

	if (hook_call(pw_install, pw)) {
		zebra_pw_install_failure(pw);
		return;
	}

	if (pw->status == PW_STATUS_DOWN)
		zebra_pw_update_status(pw, PW_STATUS_UP);
}

static void zebra_pw_uninstall(struct zebra_pw *pw)
{
        //Mehran Memarnejad
        //Just a quick treatment
        del_mpls_tunnel(pw);
        
	if (pw->status == PW_STATUS_DOWN)
		return;

	if (IS_ZEBRA_DEBUG_PW)
		zlog_debug("%u: uninstalling pseudowire %s protocol %s",
			   pw->vrf_id, pw->ifname,
			   zebra_route_string(pw->protocol));

	/* ignore any possible error */
	hook_call(pw_uninstall, pw);

	if (zebra_pw_enabled(pw))
		zebra_pw_update_status(pw, PW_STATUS_DOWN);
}

/*
 * Installation of the pseudowire in the kernel or hardware has failed. This
 * function will notify the pseudowire client about the failure and schedule
 * to retry the installation later. This function can be called by an external
 * agent that performs the pseudowire installation in an asynchronous way.
 */
void zebra_pw_install_failure(struct zebra_pw *pw)
{
	if (IS_ZEBRA_DEBUG_PW)
		zlog_debug(
			"%u: failed installing pseudowire %s, "
			"scheduling retry in %u seconds",
			pw->vrf_id, pw->ifname, PW_INSTALL_RETRY_INTERVAL);

	/* schedule to retry later */
	THREAD_TIMER_OFF(pw->install_retry_timer);
	thread_add_timer(zebrad.master, zebra_pw_install_retry, pw,
			 PW_INSTALL_RETRY_INTERVAL, &pw->install_retry_timer);

	zebra_pw_update_status(pw, PW_STATUS_DOWN);
}

static int zebra_pw_install_retry(struct thread *thread)
{
	struct zebra_pw *pw = THREAD_ARG(thread);

	pw->install_retry_timer = NULL;
	zebra_pw_install(pw);

	return 0;
}

static void zebra_pw_update_status(struct zebra_pw *pw, int status)
{
	pw->status = status;
	if (pw->client)
		zsend_pw_update(pw->client, pw);
}

static int zebra_pw_check_reachability(struct zebra_pw *pw)
{
	struct route_entry *re;
	struct nexthop *nexthop;

	/* TODO: consider GRE/L2TPv3 tunnels in addition to MPLS LSPs */

	/* find route to the remote end of the pseudowire */
	re = rib_match(family2afi(pw->af), SAFI_UNICAST, pw->vrf_id,
		       &pw->nexthop, NULL);
	if (!re) {
		if (IS_ZEBRA_DEBUG_PW)
			zlog_warn("%s: no route found for %s", __func__,
				  pw->ifname);
		return -1;
	}

	/*
	 * Need to ensure that there's a label binding for all nexthops.
	 * Otherwise, ECMP for this route could render the pseudowire unusable.
	 */
	for (ALL_NEXTHOPS(re->ng, nexthop)) {
		if (!nexthop->nh_label) {
			if (IS_ZEBRA_DEBUG_PW)
				zlog_warn("%s: unlabeled route for %s",
					  __func__, pw->ifname);
			return -1;
		}
	}

	return 0;
}

static int zebra_pw_client_close(struct zserv *client)
{
	struct vrf *vrf;
	struct zebra_vrf *zvrf;
	struct zebra_pw *pw, *tmp;

	RB_FOREACH (vrf, vrf_id_head, &vrfs_by_id) {
		zvrf = vrf->info;
		RB_FOREACH_SAFE (pw, zebra_pw_head, &zvrf->pseudowires, tmp) {
			if (pw->client != client)
				continue;
			zebra_pw_del(zvrf, pw);
		}
	}

	return 0;
}

void zebra_pw_init(struct zebra_vrf *zvrf)
{
	RB_INIT(zebra_pw_head, &zvrf->pseudowires);
	RB_INIT(zebra_static_pw_head, &zvrf->static_pseudowires);

	hook_register(zserv_client_close, zebra_pw_client_close);
}

void zebra_pw_exit(struct zebra_vrf *zvrf)
{
	struct zebra_pw *pw;

	while (!RB_EMPTY(zebra_pw_head, &zvrf->pseudowires)) {
		pw = RB_ROOT(zebra_pw_head, &zvrf->pseudowires);

		zebra_pw_del(zvrf, pw);
	}
}

DEFUN_NOSH (pseudowire_if,
	    pseudowire_if_cmd,
	    "[no] pseudowire IFNAME",
	    NO_STR
	    "Static pseudowire configuration\n"
	    "Pseudowire name\n")
{
	struct zebra_vrf *zvrf;
	struct zebra_pw *pw;
	int idx = 0;
	const char *ifname;

	zvrf = vrf_info_lookup(VRF_DEFAULT);
	if (!zvrf)
		return CMD_WARNING;

	argv_find(argv, argc, "IFNAME", &idx);
	ifname = argv[idx]->arg;
	pw = zebra_pw_find(zvrf, ifname);
	if (pw && pw->protocol != ZEBRA_ROUTE_STATIC) {
		vty_out(vty, "%% Pseudowire is not static\n");
		return CMD_WARNING;
	}

	if (argv_find(argv, argc, "no", &idx)) {
		if (!pw)
			return CMD_SUCCESS;
		zebra_pw_del(zvrf, pw);
		return CMD_SUCCESS;
	}

	if (!pw)
		pw = zebra_pw_add(zvrf, ifname, ZEBRA_ROUTE_STATIC, NULL);
	VTY_PUSH_CONTEXT(PW_NODE, pw);

	return CMD_SUCCESS;
}

DEFUN (pseudowire_labels,
       pseudowire_labels_cmd,
       "[no] mpls label local (16-1048575) remote (16-1048575)",
       NO_STR
       "MPLS L2VPN PW command\n"
       "MPLS L2VPN static labels\n"
       "Local pseudowire label\n"
       "Local pseudowire label\n"
       "Remote pseudowire label\n"
       "Remote pseudowire label\n")
{
	VTY_DECLVAR_CONTEXT(zebra_pw, pw);
	int idx = 0;
	mpls_label_t local_label, remote_label;

	if (argv_find(argv, argc, "no", &idx)) {
		local_label = MPLS_NO_LABEL;
		remote_label = MPLS_NO_LABEL;
	} else {
		argv_find(argv, argc, "local", &idx);
		local_label = atoi(argv[idx + 1]->arg);
		argv_find(argv, argc, "remote", &idx);
		remote_label = atoi(argv[idx + 1]->arg);
	}

	zebra_pw_change(pw, pw->ifindex, pw->type, pw->af, &pw->nexthop,
			local_label, remote_label, pw->flags, &pw->data);

	return CMD_SUCCESS;
}

DEFUN (pseudowire_neighbor,
       pseudowire_neighbor_cmd,
       "[no] neighbor <A.B.C.D|X:X::X:X>",
       NO_STR
       "Specify the IPv4 or IPv6 address of the remote endpoint\n"
       "IPv4 address\n"
       "IPv6 address\n")
{
	VTY_DECLVAR_CONTEXT(zebra_pw, pw);
	int idx = 0;
	const char *address;
	int af;
	union g_addr nexthop;

	af = AF_UNSPEC;
	memset(&nexthop, 0, sizeof(nexthop));

	if (!argv_find(argv, argc, "no", &idx)) {
		argv_find(argv, argc, "neighbor", &idx);
		address = argv[idx + 1]->arg;

		if (inet_pton(AF_INET, address, &nexthop.ipv4) == 1)
			af = AF_INET;
		else if (inet_pton(AF_INET6, address, &nexthop.ipv6) == 1)
			af = AF_INET6;
		else {
			vty_out(vty, "%% Malformed address\n");
			return CMD_WARNING;
		}
	}

	zebra_pw_change(pw, pw->ifindex, pw->type, af, &nexthop,
			pw->local_label, pw->remote_label, pw->flags,
			&pw->data);

	return CMD_SUCCESS;
}

DEFUN (pseudowire_control_word,
       pseudowire_control_word_cmd,
       "[no] control-word <exclude|include>",
       NO_STR
       "Control-word options\n"
       "Exclude control-word in pseudowire packets\n"
       "Include control-word in pseudowire packets\n")
{
	VTY_DECLVAR_CONTEXT(zebra_pw, pw);
	int idx = 0;
	uint8_t flags = 0;

	if (argv_find(argv, argc, "no", &idx))
		flags = F_PSEUDOWIRE_CWORD;
	else {
		argv_find(argv, argc, "control-word", &idx);
		if (argv[idx + 1]->text[0] == 'i')
			flags = F_PSEUDOWIRE_CWORD;
	}

	zebra_pw_change(pw, pw->ifindex, pw->type, pw->af, &pw->nexthop,
			pw->local_label, pw->remote_label, flags, &pw->data);

	return CMD_SUCCESS;
}

DEFUN (show_pseudowires,
       show_pseudowires_cmd,
       "show mpls pseudowires",
       SHOW_STR
       MPLS_STR
       "Pseudowires\n")
{
	struct zebra_vrf *zvrf;
	struct zebra_pw *pw;

	zvrf = vrf_info_lookup(VRF_DEFAULT);
	if (!zvrf)
		return 0;

	vty_out(vty, "%-16s %-24s %-12s %-8s %-10s\n", "Interface", "Neighbor",
		"Labels", "Protocol", "Status");

	RB_FOREACH (pw, zebra_pw_head, &zvrf->pseudowires) {
		char buf_nbr[INET6_ADDRSTRLEN];
		char buf_labels[64];

		inet_ntop(pw->af, &pw->nexthop, buf_nbr, sizeof(buf_nbr));

		if (pw->local_label != MPLS_NO_LABEL
		    && pw->remote_label != MPLS_NO_LABEL)
			snprintf(buf_labels, sizeof(buf_labels), "%u/%u",
				 pw->local_label, pw->remote_label);
		else
			snprintf(buf_labels, sizeof(buf_labels), "-");

		vty_out(vty, "%-16s %-24s %-12s %-8s %-10s\n", pw->ifname,
			(pw->af != AF_UNSPEC) ? buf_nbr : "-", buf_labels,
			zebra_route_string(pw->protocol),
			(zebra_pw_enabled(pw) && pw->status == PW_STATUS_UP)
				? "UP"
				: "DOWN");
	}

	return CMD_SUCCESS;
}

/* Pseudowire configuration write function. */
static int zebra_pw_config(struct vty *vty)
{
	int write = 0;
	struct zebra_vrf *zvrf;
	struct zebra_pw *pw;

	zvrf = vrf_info_lookup(VRF_DEFAULT);
	if (!zvrf)
		return 0;

	RB_FOREACH (pw, zebra_static_pw_head, &zvrf->static_pseudowires) {
		vty_out(vty, "pseudowire %s\n", pw->ifname);
		if (pw->local_label != MPLS_NO_LABEL
		    && pw->remote_label != MPLS_NO_LABEL)
			vty_out(vty, " mpls label local %u remote %u\n",
				pw->local_label, pw->remote_label);
		else
			vty_out(vty,
				" ! Incomplete config, specify the static "
				"MPLS labels\n");

		if (pw->af != AF_UNSPEC) {
			char buf[INET6_ADDRSTRLEN];
			inet_ntop(pw->af, &pw->nexthop, buf, sizeof(buf));
			vty_out(vty, " neighbor %s\n", buf);
		} else
			vty_out(vty,
				" ! Incomplete config, specify a neighbor "
				"address\n");

		if (!(pw->flags & F_PSEUDOWIRE_CWORD))
			vty_out(vty, " control-word exclude\n");

		vty_out(vty, "!\n");
		write = 1;
	}

	return write;
}

static struct cmd_node pw_node = {
	PW_NODE, "%s(config-pw)# ", 1,
};

void zebra_pw_vty_init(void)
{
	install_node(&pw_node, zebra_pw_config);
	install_default(PW_NODE);

	install_element(CONFIG_NODE, &pseudowire_if_cmd);
	install_element(PW_NODE, &pseudowire_labels_cmd);
	install_element(PW_NODE, &pseudowire_neighbor_cmd);
	install_element(PW_NODE, &pseudowire_control_word_cmd);

	install_element(VIEW_NODE, &show_pseudowires_cmd);
        
        //Mehran Memarnejad
        hook_register(pw_install, add_mpls_tunnel);
        hook_register(pw_uninstall, del_mpls_tunnel);
        
        //Allocate space for saving mpw-mpls_tunnel name association
        mpw_names_frr = (char **) malloc(sizeof(char*) * VPP_TUNNEL_N);
        mpls_tunnel_names_vpp = (char **) malloc(sizeof(char*) * VPP_TUNNEL_N);
        tunnel_n = -1;
}
