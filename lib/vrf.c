/*
 * VRF functions.
 * Copyright (C) 2014 6WIND S.A.
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2, or (at your
 * option) any later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <zebra.h>

/* for basename */
#include <libgen.h>

#include "if.h"
#include "vrf.h"
#include "vrf_int.h"
#include "prefix.h"
#include "table.h"
#include "log.h"
#include "memory.h"
#include "command.h"
#include "ns.h"
#include "privs.h"
#include "nexthop_group.h"

//Mehran Memarnejad
void vpp_del_vrf_fields(int idx){
 
    vpp_vrf_list[idx].name[0] = "\0";
    vpp_vrf_list[idx].vrf_id = 0;
    vpp_vrf_list[idx].table_id = 0;
    
    return;
}

void vpp_add_vrf(char* name, vrf_id_t vrf_id, uint32_t table_id){

    strcpy(vpp_vrf_list[vpp_num_vrf].name, name);
    vpp_vrf_list[vpp_num_vrf].vrf_id = vrf_id;
    vpp_vrf_list[vpp_num_vrf].table_id = table_id;
    
    vpp_num_vrf++;
    
    return;
}

void vpp_upd_vrf_fields(char* name, vrf_id_t vrf_id, uint32_t table_id){

    int idx = 0;
    
    if(name){
        while(idx < vpp_num_vrf && strcmp(vpp_vrf_list[idx].name, name))
            idx++;
        
        if(idx == vpp_num_vrf) //not found
            return;
        
        vpp_vrf_list[idx].vrf_id = vrf_id;
        vpp_vrf_list[idx].table_id = table_id;
    }
    
    return;
}

void vpp_del_vrf(char* name, vrf_id_t vrf_id, uint32_t table_id){

    int idx = 0;
    
    if(name){
        while(idx < vpp_num_vrf && strcmp(vpp_vrf_list[idx].name, name))
            idx++;
        
        if(idx == vpp_num_vrf) //not found
            return;
        
        vpp_del_vrf_fields(idx);
        return;
    }
    
    if(vrf_id > 0){
        while(idx < vpp_num_vrf && vpp_vrf_list[idx].vrf_id != vrf_id)
            idx++;
    
        if(idx == vpp_num_vrf) //not found
            return;
        
        vpp_del_vrf_fields(idx);
        return;
    }
    
    if(table_id > 0){
        while(idx < vpp_num_vrf && vpp_vrf_list[idx].table_id != table_id)
            idx++;
    
        if(idx == vpp_num_vrf) //not found
            return;
        
        vpp_del_vrf_fields(idx);
        return;
    }
}

uint32_t vpp_vrf_name_to_table_id(char* name){
    
    int idx = 0;
    
    if(name){
        while(idx < vpp_num_vrf && strcmp(vpp_vrf_list[idx].name, name))
            idx++;
        
        if(idx == vpp_num_vrf) //not found
            return 0;
        
        return vpp_vrf_list[idx].table_id;
    }
    
    return 0;
}

int vpp_vrf_id_to_table_id(vrf_id_t vrf_id){
    
    int idx = 0;
    
    if(vrf_id >= 0){
        while(idx < vpp_num_vrf && vpp_vrf_list[idx].vrf_id != vrf_id)
            idx++;
    
        if(idx == vpp_num_vrf) //not found
            return -1;
        
        return vpp_vrf_list[idx].table_id;
    }
    
    return -1;
}

//m.salari 
char* vpp_vrf_id_to_name(vrf_id_t vrf_id){
    int idx = 0;
    if(vrf_id >= 0){
         while(idx < vpp_num_vrf && vpp_vrf_list[idx].vrf_id != vrf_id)
            idx++;
    }
    if(idx == vpp_num_vrf) //not found
            return NULL;
    return vpp_vrf_list[idx].name;
}
//\m.salari 

/* default VRF ID value used when VRF backend is not NETNS */
#define VRF_DEFAULT_INTERNAL 0

DEFINE_MTYPE_STATIC(LIB, VRF, "VRF")
DEFINE_MTYPE_STATIC(LIB, VRF_BITMAP, "VRF bit-map")

DEFINE_QOBJ_TYPE(vrf)

static __inline int vrf_id_compare(const struct vrf *, const struct vrf *);
static __inline int vrf_name_compare(const struct vrf *, const struct vrf *);

RB_GENERATE(vrf_id_head, vrf, id_entry, vrf_id_compare);
RB_GENERATE(vrf_name_head, vrf, name_entry, vrf_name_compare);

struct vrf_id_head vrfs_by_id = RB_INITIALIZER(&vrfs_by_id);
struct vrf_name_head vrfs_by_name = RB_INITIALIZER(&vrfs_by_name);

static int vrf_backend;
static struct zebra_privs_t *vrf_daemon_privs;

/*
 * Turn on/off debug code
 * for vrf.
 */
int debug_vrf = 0;

/* Holding VRF hooks  */
struct vrf_master {
	int (*vrf_new_hook)(struct vrf *);
	int (*vrf_delete_hook)(struct vrf *);
	int (*vrf_enable_hook)(struct vrf *);
	int (*vrf_disable_hook)(struct vrf *);
} vrf_master = {
	0,
};

static int vrf_is_enabled(struct vrf *vrf);

/* VRF list existance check by name. */
struct vrf *vrf_lookup_by_name(const char *name)
{
	struct vrf vrf;
	strlcpy(vrf.name, name, sizeof(vrf.name));
	return (RB_FIND(vrf_name_head, &vrfs_by_name, &vrf));
}

static __inline int vrf_id_compare(const struct vrf *a, const struct vrf *b)
{
	return (a->vrf_id - b->vrf_id);
}

static int vrf_name_compare(const struct vrf *a, const struct vrf *b)
{
	return strcmp(a->name, b->name);
}

/* if ns_id is different and not VRF_UNKNOWN,
 * then update vrf identifier, and enable VRF
 */
static void vrf_update_vrf_id(ns_id_t ns_id, void *opaqueptr)
{
	ns_id_t vrf_id = (vrf_id_t)ns_id;
	vrf_id_t old_vrf_id;
	struct vrf *vrf = (struct vrf *)opaqueptr;

	if (!vrf)
		return;
	old_vrf_id = vrf->vrf_id;
	if (vrf_id == vrf->vrf_id)
		return;
	if (vrf->vrf_id != VRF_UNKNOWN)
		RB_REMOVE(vrf_id_head, &vrfs_by_id, vrf);
	vrf->vrf_id = vrf_id;
	RB_INSERT(vrf_id_head, &vrfs_by_id, vrf);
	if (old_vrf_id == VRF_UNKNOWN)
		vrf_enable((struct vrf *)vrf);
}

int vrf_switch_to_netns(vrf_id_t vrf_id)
{
	char *name;
	struct vrf *vrf = vrf_lookup_by_id(vrf_id);

	/* VRF is default VRF. silently ignore */
	if (!vrf || vrf->vrf_id == VRF_DEFAULT)
		return 1;	/* 1 = default */
	/* VRF has no NETNS backend. silently ignore */
	if (vrf->data.l.netns_name[0] == '\0')
		return 2;	/* 2 = no netns */
	name = ns_netns_pathname(NULL, vrf->data.l.netns_name);
	if (debug_vrf)
		zlog_debug("VRF_SWITCH: %s(%u)", name, vrf->vrf_id);
	return ns_switch_to_netns(name);
}

int vrf_switchback_to_initial(void)
{
	int ret = ns_switchback_to_initial();

	if (ret == 0 && debug_vrf)
		zlog_debug("VRF_SWITCHBACK");
	return ret;
}

/* Get a VRF. If not found, create one.
 * Arg:
 *   name   - The name of the vrf.  May be NULL if unknown.
 *   vrf_id - The vrf_id of the vrf.  May be VRF_UNKNOWN if unknown
 * Description: Please note that this routine can be called with just the name
 * and 0 vrf-id
 */
struct vrf *vrf_get(vrf_id_t vrf_id, const char *name)
{
	struct vrf *vrf = NULL;
	int new = 0;

	if (debug_vrf)
		zlog_debug("VRF_GET: %s(%u)", name == NULL ? "(NULL)" : name,
			   vrf_id);

	/* Nothing to see, move along here */
	if (!name && vrf_id == VRF_UNKNOWN)
		return NULL;

	/* attempt to find already available VRF
	 */
	if (name)
		vrf = vrf_lookup_by_name(name);
	/* Try to find VRF both by ID and name */
	if (!vrf && vrf_id != VRF_UNKNOWN)
		vrf = vrf_lookup_by_id(vrf_id);

        int newvrf = 0;//m.salari
	if (vrf == NULL) {
            newvrf = 1;//m.salari
                
		vrf = XCALLOC(MTYPE_VRF, sizeof(struct vrf));
		vrf->vrf_id = VRF_UNKNOWN;
		QOBJ_REG(vrf, vrf);
		new = 1;
                
		if (debug_vrf)
			zlog_debug("VRF(%u) %s is created.", vrf_id,
				   (name) ? name : "(NULL)");
	}

	/* Set identifier */
	if (vrf_id != VRF_UNKNOWN && vrf->vrf_id == VRF_UNKNOWN) {
		vrf->vrf_id = vrf_id;
		RB_INSERT(vrf_id_head, &vrfs_by_id, vrf);
	}

	/* Set name */
	if (name && vrf->name[0] != '\0' && strcmp(name, vrf->name)) {
		RB_REMOVE(vrf_name_head, &vrfs_by_name, vrf);
		strlcpy(vrf->name, name, sizeof(vrf->name));
		RB_INSERT(vrf_name_head, &vrfs_by_name, vrf);
	} else if (name && vrf->name[0] == '\0') {
		strlcpy(vrf->name, name, sizeof(vrf->name));
		RB_INSERT(vrf_name_head, &vrfs_by_name, vrf);
	}
	if (new &&vrf_master.vrf_new_hook)
		(*vrf_master.vrf_new_hook)(vrf);

        //m.salari
        if (newvrf) {
                //Mehran Memarnejad
                //If vrf not found create it in Linux
                //Linux already has default vrf, so no need to create it again
                if(strcmp(name, VRF_DEFAULT_NAME)){
                    char cmd[200];
                    //create vrf
                    sprintf(cmd, "sudo ip link add name %s type vrf table %u", name, vpp_num_vrf);
                    printf("%s: %s\n", __func__, cmd);
                    system(cmd); 
                    //set vrf up
                    sprintf(cmd, "sudo ip link set dev %s up", name);
                    printf("%s: %s\n", __func__, cmd);
                    system(cmd);
                    //create vpp vrf
                    //don't make vpp vrf ... linux uses RTM_NEWLINK for updates as well
                    sprintf(cmd, "sudo vppctl ip table add %u", vpp_num_vrf);
                    printf("%s: %s\n", __func__, cmd);
                    system(cmd);
                    //add it to vpp_vrf_list   table_id = vpp_num_vrf
//                    vpp_add_vrf(name, VRF_UNKNOWN, vpp_num_vrf);        
                    
                    vpp_add_vrf(name, vrf->vrf_id, vpp_num_vrf); //m.salari
                    //deleteme
                    printf("%s: VRF ADD %u\n", __func__, vrf->vrf_id);
                    int i;
                    for (i = 0; i < vpp_num_vrf; i++) {
                        printf(" ++ VRF %u (%u) = %s \n", vpp_vrf_list[i].vrf_id, vpp_vrf_list[i].table_id, vpp_vrf_list[i].name);
                    }
                    //\deleteme
                }
        }
        //\m.salari

	return vrf;
}

/* Delete a VRF. This is called when the underlying VRF goes away, a
 * pre-configured VRF is deleted or when shutting down (vrf_terminate()).
 */
void vrf_delete(struct vrf *vrf)
{
	if (debug_vrf)
		zlog_debug("VRF %u is to be deleted.", vrf->vrf_id);

	if (vrf_is_enabled(vrf))
		vrf_disable(vrf);

	/* If the VRF is user configured, it'll stick around, just remove
	 * the ID mapping. Interfaces assigned to this VRF should've been
	 * removed already as part of the VRF going down.
	 */
	if (vrf_is_user_cfged(vrf)) {
		if (vrf->vrf_id != VRF_UNKNOWN) {
			/* Delete any VRF interfaces - should be only
			 * the VRF itself, other interfaces should've
			 * been moved out of the VRF.
			 */
			if_terminate(vrf);
			RB_REMOVE(vrf_id_head, &vrfs_by_id, vrf);
			vrf->vrf_id = VRF_UNKNOWN;
		}
		return;
	}

	if (vrf_master.vrf_delete_hook)
		(*vrf_master.vrf_delete_hook)(vrf);

	QOBJ_UNREG(vrf);
        //Mehran Memarnejad
        //Don't delete interfaces, instead bind it to default VRF
        //if default vrf is to be deleted, then delete interfaces as well, else bind interfaces to default vrf
        if(vrf->vrf_id == 0)
            if_terminate(vrf);
        else
            vpp_if_terminate(vrf);

	if (vrf->vrf_id != VRF_UNKNOWN)
		RB_REMOVE(vrf_id_head, &vrfs_by_id, vrf);
	if (vrf->name[0] != '\0')
		RB_REMOVE(vrf_name_head, &vrfs_by_name, vrf);
        
        //Mehran Memarnejad
        int table_id;
        table_id = vpp_vrf_id_to_table_id(vrf->vrf_id);
        //Don't delete vpp default vrf ... it's not possible to delete vpp default vrf
        if(table_id > 0){
            char cmd[200];
            sprintf(cmd, "sudo ip link del dev %s", vrf->name);
            printf("%s: %s\n", __func__, cmd);
            system(cmd);
            sprintf(cmd, "sudo vppctl ip table del %d", table_id);
            printf("%s %s\n", __func__, cmd);
            system(cmd);
        }
        
	XFREE(MTYPE_VRF, vrf);
}

/* Look up a VRF by identifier. */
struct vrf *vrf_lookup_by_id(vrf_id_t vrf_id)
{
	struct vrf vrf;
	vrf.vrf_id = vrf_id;
	return (RB_FIND(vrf_id_head, &vrfs_by_id, &vrf));
}

/*
 * Enable a VRF - that is, let the VRF be ready to use.
 * The VRF_ENABLE_HOOK callback will be called to inform
 * that they can allocate resources in this VRF.
 *
 * RETURN: 1 - enabled successfully; otherwise, 0.
 */
int vrf_enable(struct vrf *vrf)
{
	if (vrf_is_enabled(vrf))
		return 1;

	if (debug_vrf)
		zlog_debug("VRF %u is enabled.", vrf->vrf_id);

	SET_FLAG(vrf->status, VRF_ACTIVE);

	if (vrf_master.vrf_enable_hook)
		(*vrf_master.vrf_enable_hook)(vrf);

	/*
	 * If we have any nexthop group entries that
	 * are awaiting vrf initialization then
	 * let's let people know about it
	 */
	nexthop_group_enable_vrf(vrf);

	return 1;
}

/*
 * Disable a VRF - that is, let the VRF be unusable.
 * The VRF_DELETE_HOOK callback will be called to inform
 * that they must release the resources in the VRF.
 */
void vrf_disable(struct vrf *vrf)
{
	if (!vrf_is_enabled(vrf))
		return;

	UNSET_FLAG(vrf->status, VRF_ACTIVE);

	if (debug_vrf)
		zlog_debug("VRF %u is to be disabled.", vrf->vrf_id);

	/* Till now, nothing to be done for the default VRF. */
	// Pending: see why this statement.

	if (vrf_master.vrf_disable_hook)
		(*vrf_master.vrf_disable_hook)(vrf);
}

const char *vrf_id_to_name(vrf_id_t vrf_id)
{
	struct vrf *vrf;

	vrf = vrf_lookup_by_id(vrf_id);
	if (vrf)
		return vrf->name;

	return "n/a";
}

vrf_id_t vrf_name_to_id(const char *name)
{
	struct vrf *vrf;
	vrf_id_t vrf_id = VRF_DEFAULT; // Pending: need a way to return invalid
				       // id/ routine not used.

	vrf = vrf_lookup_by_name(name);
	if (vrf)
		vrf_id = vrf->vrf_id;

	return vrf_id;
}

/* Get the data pointer of the specified VRF. If not found, create one. */
void *vrf_info_get(vrf_id_t vrf_id)
{
	struct vrf *vrf = vrf_get(vrf_id, NULL);
	return vrf->info;
}

/* Look up the data pointer of the specified VRF. */
void *vrf_info_lookup(vrf_id_t vrf_id)
{
	struct vrf *vrf = vrf_lookup_by_id(vrf_id);
	return vrf ? vrf->info : NULL;
}

/*
 * VRF bit-map
 */

#define VRF_BITMAP_NUM_OF_GROUPS            1024
#define VRF_BITMAP_NUM_OF_BITS_IN_GROUP (UINT32_MAX / VRF_BITMAP_NUM_OF_GROUPS)
#define VRF_BITMAP_NUM_OF_BYTES_IN_GROUP                                       \
	(VRF_BITMAP_NUM_OF_BITS_IN_GROUP / CHAR_BIT + 1) /* +1 for ensure */

#define VRF_BITMAP_GROUP(_id) ((_id) / VRF_BITMAP_NUM_OF_BITS_IN_GROUP)
#define VRF_BITMAP_BIT_OFFSET(_id) ((_id) % VRF_BITMAP_NUM_OF_BITS_IN_GROUP)

#define VRF_BITMAP_INDEX_IN_GROUP(_bit_offset) ((_bit_offset) / CHAR_BIT)
#define VRF_BITMAP_FLAG(_bit_offset)                                           \
	(((uint8_t)1) << ((_bit_offset) % CHAR_BIT))

struct vrf_bitmap {
	uint8_t *groups[VRF_BITMAP_NUM_OF_GROUPS];
};

vrf_bitmap_t vrf_bitmap_init(void)
{
	return (vrf_bitmap_t)XCALLOC(MTYPE_VRF_BITMAP,
				     sizeof(struct vrf_bitmap));
}

void vrf_bitmap_free(vrf_bitmap_t bmap)
{
	struct vrf_bitmap *bm = (struct vrf_bitmap *)bmap;
	int i;

	if (bmap == VRF_BITMAP_NULL)
		return;

	for (i = 0; i < VRF_BITMAP_NUM_OF_GROUPS; i++)
		if (bm->groups[i])
			XFREE(MTYPE_VRF_BITMAP, bm->groups[i]);

	XFREE(MTYPE_VRF_BITMAP, bm);
}

void vrf_bitmap_set(vrf_bitmap_t bmap, vrf_id_t vrf_id)
{
	struct vrf_bitmap *bm = (struct vrf_bitmap *)bmap;
	uint8_t group = VRF_BITMAP_GROUP(vrf_id);
	uint8_t offset = VRF_BITMAP_BIT_OFFSET(vrf_id);

	if (bmap == VRF_BITMAP_NULL || vrf_id == VRF_UNKNOWN)
		return;

	if (bm->groups[group] == NULL)
		bm->groups[group] = XCALLOC(MTYPE_VRF_BITMAP,
					    VRF_BITMAP_NUM_OF_BYTES_IN_GROUP);

	SET_FLAG(bm->groups[group][VRF_BITMAP_INDEX_IN_GROUP(offset)],
		 VRF_BITMAP_FLAG(offset));
}

void vrf_bitmap_unset(vrf_bitmap_t bmap, vrf_id_t vrf_id)
{
	struct vrf_bitmap *bm = (struct vrf_bitmap *)bmap;
	uint8_t group = VRF_BITMAP_GROUP(vrf_id);
	uint8_t offset = VRF_BITMAP_BIT_OFFSET(vrf_id);

	if (bmap == VRF_BITMAP_NULL || vrf_id == VRF_UNKNOWN
	    || bm->groups[group] == NULL)
		return;

	UNSET_FLAG(bm->groups[group][VRF_BITMAP_INDEX_IN_GROUP(offset)],
		   VRF_BITMAP_FLAG(offset));
}

int vrf_bitmap_check(vrf_bitmap_t bmap, vrf_id_t vrf_id)
{
	struct vrf_bitmap *bm = (struct vrf_bitmap *)bmap;
	uint8_t group = VRF_BITMAP_GROUP(vrf_id);
	uint8_t offset = VRF_BITMAP_BIT_OFFSET(vrf_id);

	if (bmap == VRF_BITMAP_NULL || vrf_id == VRF_UNKNOWN
	    || bm->groups[group] == NULL)
		return 0;

	return CHECK_FLAG(bm->groups[group][VRF_BITMAP_INDEX_IN_GROUP(offset)],
			  VRF_BITMAP_FLAG(offset))
		       ? 1
		       : 0;
}

static void vrf_autocomplete(vector comps, struct cmd_token *token)
{
	struct vrf *vrf = NULL;

	RB_FOREACH (vrf, vrf_name_head, &vrfs_by_name) {
		if (vrf->vrf_id != VRF_DEFAULT)
			vector_set(comps, XSTRDUP(MTYPE_COMPLETION, vrf->name));
	}
}

static const struct cmd_variable_handler vrf_var_handlers[] = {
	{
		.varname = "vrf",
		.completions = vrf_autocomplete,
	},
	{.completions = NULL},
};

/* Initialize VRF module. */
void vrf_init(int (*create)(struct vrf *), int (*enable)(struct vrf *),
	      int (*disable)(struct vrf *), int (*delete)(struct vrf *))
{
	struct vrf *default_vrf;

	/* initialise NS, in case VRF backend if NETNS */
	ns_init();
	if (debug_vrf)
		zlog_debug("%s: Initializing VRF subsystem",
			   __PRETTY_FUNCTION__);

	vrf_master.vrf_new_hook = create;
	vrf_master.vrf_enable_hook = enable;
	vrf_master.vrf_disable_hook = disable;
	vrf_master.vrf_delete_hook = delete;

	/* The default VRF always exists. */
	default_vrf = vrf_get(VRF_DEFAULT, VRF_DEFAULT_NAME);
	if (!default_vrf) {
		zlog_err("vrf_init: failed to create the default VRF!");
		exit(1);
	}

	/* Enable the default VRF. */
	if (!vrf_enable(default_vrf)) {
		zlog_err("vrf_init: failed to enable the default VRF!");
		exit(1);
	}

	cmd_variable_handler_register(vrf_var_handlers);
        
        //Mehran Memarnejad
        //VRF Initilization tasks
        printf("Vrf Initilization...\n");
        vpp_num_vrf = 0;
        //add default vrf to vpp_vrf_list
        vpp_add_vrf(default_vrf->name, default_vrf->vrf_id, default_vrf->data.l.table_id);
        printf("Default VRF added name: %s vrf_id: %u table_id: %u\n", vpp_vrf_list[0].name, vpp_vrf_list[0].vrf_id, vpp_vrf_list[0].table_id);
}

/* Terminate VRF module. */
void vrf_terminate(void)
{
	struct vrf *vrf;

	if (debug_vrf)
		zlog_debug("%s: Shutting down vrf subsystem",
			   __PRETTY_FUNCTION__);

	while (!RB_EMPTY(vrf_id_head, &vrfs_by_id)) {
		vrf = RB_ROOT(vrf_id_head, &vrfs_by_id);

		/* Clear configured flag and invoke delete. */
		UNSET_FLAG(vrf->status, VRF_CONFIGURED);
		vrf_delete(vrf);
	}

	while (!RB_EMPTY(vrf_name_head, &vrfs_by_name)) {
		vrf = RB_ROOT(vrf_name_head, &vrfs_by_name);

		/* Clear configured flag and invoke delete. */
		UNSET_FLAG(vrf->status, VRF_CONFIGURED);
		vrf_delete(vrf);
	}
}

static int vrf_default_accepts_vrf(int type)
{
	const char *fname = NULL;
	char buf[32] = {0x0};
	int ret = 0;
	FILE *fd = NULL;

	/*
	 * TCP & UDP services running in the default VRF context (ie., not bound
	 * to any VRF device) can work across all VRF domains by enabling the
	 * tcp_l3mdev_accept and udp_l3mdev_accept sysctl options:
	 * sysctl -w net.ipv4.tcp_l3mdev_accept=1
	 * sysctl -w net.ipv4.udp_l3mdev_accept=1
	 */
	if (type == SOCK_STREAM)
		fname = "/proc/sys/net/ipv4/tcp_l3mdev_accept";
	else if (type == SOCK_DGRAM)
		fname = "/proc/sys/net/ipv4/udp_l3mdev_accept";
	else
		return ret;
	fd = fopen(fname, "r");
	if (fd == NULL)
		return ret;
	fgets(buf, 32, fd);
	ret = atoi(buf);
	fclose(fd);
	return ret;
}

/* Create a socket for the VRF. */
int vrf_socket(int domain, int type, int protocol, vrf_id_t vrf_id,
	       char *interfacename)
{
	int ret, save_errno, ret2;

	ret = vrf_switch_to_netns(vrf_id);
	if (ret < 0)
		zlog_err("%s: Can't switch to VRF %u (%s)", __func__, vrf_id,
			 safe_strerror(errno));
	if (ret > 0 && interfacename && vrf_default_accepts_vrf(type)) {
		zlog_err("VRF socket not used since net.ipv4.%s_l3mdev_accept != 0",
			  (type == SOCK_STREAM ? "tcp" : "udp"));
		errno = EEXIST; /* not sure if this is the best error... */
		return -2;
	}
	ret = socket(domain, type, protocol);
	save_errno = errno;
	ret2 = vrf_switchback_to_initial();
	if (ret2 < 0)
		zlog_err("%s: Can't switchback from VRF %u (%s)", __func__,
			 vrf_id, safe_strerror(errno));
	errno = save_errno;
	if (ret <= 0)
		return ret;
	ret2 = vrf_bind(vrf_id, ret, interfacename);
	if (ret2 < 0) {
		close(ret);
		ret = ret2;
	}
	return ret;
}

int vrf_is_backend_netns(void)
{
	return (vrf_backend == VRF_BACKEND_NETNS);
}

int vrf_get_backend(void)
{
	return vrf_backend;
}

void vrf_configure_backend(int vrf_backend_netns)
{
	vrf_backend = vrf_backend_netns;
}

int vrf_handler_create(struct vty *vty, const char *vrfname,
		       struct vrf **vrf)
{
	struct vrf *vrfp;

	if (strlen(vrfname) > VRF_NAMSIZ) {
		if (vty)
			vty_out(vty,
				"%% VRF name %s invalid: length exceeds %d bytes\n",
				vrfname, VRF_NAMSIZ);
		else
			zlog_warn(
				"%% VRF name %s invalid: length exceeds %d bytes\n",
				vrfname, VRF_NAMSIZ);
		return CMD_WARNING_CONFIG_FAILED;
	}

	vrfp = vrf_get(VRF_UNKNOWN, vrfname);

	if (vty)
		VTY_PUSH_CONTEXT(VRF_NODE, vrfp);

	if (vrf)
		*vrf = vrfp;
        //m.salari
        struct vrf * vrf2 = vrf_lookup_by_name(vrfname);
        printf(" -*-*-*-* %u\n", vrf2->vrf_id);
        //\m.salari
        
	return CMD_SUCCESS;
}

int vrf_netns_handler_create(struct vty *vty, struct vrf *vrf, char *pathname,
			     ns_id_t ns_id, ns_id_t internal_ns_id)
{
	struct ns *ns = NULL;

	if (!vrf)
		return CMD_WARNING_CONFIG_FAILED;
	if (vrf->vrf_id != VRF_UNKNOWN && vrf->ns_ctxt == NULL) {
		if (vty)
			vty_out(vty,
				"VRF %u is already configured with VRF %s\n",
				vrf->vrf_id, vrf->name);
		else
			zlog_warn("VRF %u is already configured with VRF %s\n",
				  vrf->vrf_id, vrf->name);
		return CMD_WARNING_CONFIG_FAILED;
	}
	if (vrf->ns_ctxt != NULL) {
		ns = (struct ns *)vrf->ns_ctxt;
		if (ns && 0 != strcmp(ns->name, pathname)) {
			if (vty)
				vty_out(vty,
					"VRF %u already configured with NETNS %s\n",
					vrf->vrf_id, ns->name);
			else
				zlog_warn(
					"VRF %u already configured with NETNS %s",
					vrf->vrf_id, ns->name);
			return CMD_WARNING_CONFIG_FAILED;
		}
	}
	ns = ns_lookup_name(pathname);
	if (ns && ns->vrf_ctxt) {
		struct vrf *vrf2 = (struct vrf *)ns->vrf_ctxt;

		if (vrf2 == vrf)
			return CMD_SUCCESS;
		if (vty)
			vty_out(vty,
				"NS %s is already configured"
				" with VRF %u(%s)\n",
				ns->name, vrf2->vrf_id, vrf2->name);
		else
			zlog_warn("NS %s is already configured with VRF %u(%s)",
				  ns->name, vrf2->vrf_id, vrf2->name);
		return CMD_WARNING_CONFIG_FAILED;
	}
	ns = ns_get_created(ns, pathname, ns_id);
	ns->internal_ns_id = internal_ns_id;
	ns->vrf_ctxt = (void *)vrf;
	vrf->ns_ctxt = (void *)ns;
	/* update VRF netns NAME */
	if (vrf)
		strlcpy(vrf->data.l.netns_name, basename(pathname), NS_NAMSIZ);

	if (!ns_enable(ns, vrf_update_vrf_id)) {
		if (vty)
			vty_out(vty, "Can not associate NS %u with NETNS %s\n",
				ns->ns_id, ns->name);
		else
			zlog_warn("Can not associate NS %u with NETNS %s",
				  ns->ns_id, ns->name);
		return CMD_WARNING_CONFIG_FAILED;
	}

	return CMD_SUCCESS;
}

int vrf_is_mapped_on_netns(vrf_id_t vrf_id)
{
	struct vrf *vrf = vrf_lookup_by_id(vrf_id);

	if (!vrf || vrf->data.l.netns_name[0] == '\0')
		return 0;
	if (vrf->vrf_id == VRF_DEFAULT)
		return 0;
	return 1;
}

/* vrf CLI commands */
DEFUN_NOSH(vrf_exit,
           vrf_exit_cmd,
	   "exit-vrf",
	   "Exit current mode and down to previous mode\n")
{
	/* We have to set vrf context to default vrf */
	VTY_PUSH_CONTEXT(VRF_NODE, vrf_get(VRF_DEFAULT, VRF_DEFAULT_NAME));
	vty->node = CONFIG_NODE;
	return CMD_SUCCESS;
}

DEFUN_NOSH (vrf,
       vrf_cmd,
       "vrf NAME",
       "Select a VRF to configure\n"
       "VRF's name\n")
{
	int idx_name = 1;
	const char *vrfname = argv[idx_name]->arg;

	return vrf_handler_create(vty, vrfname, NULL);
}

DEFUN_NOSH (no_vrf,
           no_vrf_cmd,
           "no vrf NAME",
           NO_STR
           "Delete a pseudo VRF's configuration\n"
           "VRF's name\n")
{
	const char *vrfname = argv[2]->arg;

	struct vrf *vrfp;

	vrfp = vrf_lookup_by_name(vrfname);

	if (vrfp == NULL) {
		vty_out(vty, "%% VRF %s does not exist\n", vrfname);
		return CMD_WARNING_CONFIG_FAILED;
	}
   
	//if (CHECK_FLAG(vrfp->status, VRF_ACTIVE)) {
	//	vty_out(vty, "%% Only inactive VRFs can be deleted\n");
	//	return CMD_WARNING_CONFIG_FAILED;
	//}
        //Mehran Memarnejad
        char cmd[200];
        sprintf(cmd, "sudo ip link set dev %s down", vrfname);
        printf("%s: %s\n", __func__, cmd);
        system(cmd);
        
	/* Clear configured flag and invoke delete. */
	UNSET_FLAG(vrfp->status, VRF_CONFIGURED);
        //Don't delete it here, let Linux inform FRR about "ip link del" command and then netlink_vrf_change in if_netlink.c will delete it 
	//vrf_delete(vrfp);
        
        //Mehran Memarnejad
        //char cmd[200];
        sprintf(cmd, "sudo ip link del dev %s", vrfname);
        printf("%s: %s\n", __func__, cmd);
        system(cmd);
        
	return CMD_SUCCESS;
}


struct cmd_node vrf_node = {VRF_NODE, "%s(config-vrf)# ", 1};

DEFUN (vrf_netns,
       vrf_netns_cmd,
       "netns NAME",
       "Attach VRF to a Namespace\n"
       "The file name in " NS_RUN_DIR ", or a full pathname\n")
{
	int idx_name = 1, ret;
	char *pathname = ns_netns_pathname(vty, argv[idx_name]->arg);

	VTY_DECLVAR_CONTEXT(vrf, vrf);

	if (!pathname)
		return CMD_WARNING_CONFIG_FAILED;

	if (vrf_daemon_privs &&
	    vrf_daemon_privs->change(ZPRIVS_RAISE))
		zlog_err("%s: Can't raise privileges", __func__);

	ret = vrf_netns_handler_create(vty, vrf, pathname,
				       NS_UNKNOWN, NS_UNKNOWN);

	if (vrf_daemon_privs &&
	    vrf_daemon_privs->change(ZPRIVS_LOWER))
		zlog_err("%s: Can't lower privileges", __func__);
	return ret;
}

DEFUN (no_vrf_netns,
	no_vrf_netns_cmd,
	"no netns [NAME]",
	NO_STR
	"Detach VRF from a Namespace\n"
	"The file name in " NS_RUN_DIR ", or a full pathname\n")
{
	struct ns *ns = NULL;

	VTY_DECLVAR_CONTEXT(vrf, vrf);

	if (!vrf_is_backend_netns()) {
		vty_out(vty, "VRF backend is not Netns. Aborting\n");
		return CMD_WARNING_CONFIG_FAILED;
	}
	if (!vrf->ns_ctxt) {
		vty_out(vty, "VRF %s(%u) is not configured with NetNS\n",
			vrf->name, vrf->vrf_id);
		return CMD_WARNING_CONFIG_FAILED;
	}

	ns = (struct ns *)vrf->ns_ctxt;

	ns->vrf_ctxt = NULL;
	vrf_disable(vrf);
	/* vrf ID from VRF is necessary for Zebra
	 * so that propagate to other clients is done
	 */
	ns_delete(ns);
	vrf->ns_ctxt = NULL;
	return CMD_SUCCESS;
}

/*
 * Debug CLI for vrf's
 */
DEFUN (vrf_debug,
      vrf_debug_cmd,
      "debug vrf",
      DEBUG_STR
      "VRF Debugging\n")
{
	debug_vrf = 1;

	return CMD_SUCCESS;
}

DEFUN (no_vrf_debug,
      no_vrf_debug_cmd,
      "no debug vrf",
      NO_STR
      DEBUG_STR
      "VRF Debugging\n")
{
	debug_vrf = 0;

	return CMD_SUCCESS;
}

static int vrf_write_host(struct vty *vty)
{
	if (debug_vrf)
		vty_out(vty, "debug vrf\n");

	return 1;
}

static struct cmd_node vrf_debug_node = {VRF_DEBUG_NODE, "", 1};

void vrf_install_commands(void)
{
	install_node(&vrf_debug_node, vrf_write_host);

	install_element(CONFIG_NODE, &vrf_debug_cmd);
	install_element(ENABLE_NODE, &vrf_debug_cmd);
	install_element(CONFIG_NODE, &no_vrf_debug_cmd);
	install_element(ENABLE_NODE, &no_vrf_debug_cmd);
}

void vrf_cmd_init(int (*writefunc)(struct vty *vty),
		  struct zebra_privs_t *daemon_privs)
{
	install_element(CONFIG_NODE, &vrf_cmd);
	install_element(CONFIG_NODE, &no_vrf_cmd);
	install_node(&vrf_node, writefunc);
	install_default(VRF_NODE);
	install_element(VRF_NODE, &vrf_exit_cmd);
	if (vrf_is_backend_netns() && ns_have_netns()) {
		/* Install NS commands. */
		vrf_daemon_privs = daemon_privs;
		install_element(VRF_NODE, &vrf_netns_cmd);
		install_element(VRF_NODE, &no_vrf_netns_cmd);
	}
}

vrf_id_t vrf_get_default_id(void)
{
	struct vrf *vrf = vrf_lookup_by_name(VRF_DEFAULT_NAME);

	if (vrf)
		return vrf->vrf_id;
	/* backend netns is only known by zebra
	 * for other daemons, we return VRF_DEFAULT_INTERNAL
	 */
	if (vrf_is_backend_netns())
		return ns_get_default_id();
	else
		return VRF_DEFAULT_INTERNAL;
}

int vrf_bind(vrf_id_t vrf_id, int fd, char *name)
{
	int ret = 0;

	if (fd < 0 || name == NULL)
		return fd;
	if (vrf_is_mapped_on_netns(vrf_id))
		return fd;
#ifdef SO_BINDTODEVICE
	ret = setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, name, strlen(name)+1);
	if (ret < 0)
		zlog_debug("bind to interface %s failed, errno=%d", name,
			   errno);
#endif /* SO_BINDTODEVICE */
	return ret;
}
int vrf_getaddrinfo(const char *node, const char *service,
		    const struct addrinfo *hints, struct addrinfo **res,
		    vrf_id_t vrf_id)
{
	int ret, ret2, save_errno;

	ret = vrf_switch_to_netns(vrf_id);
	if (ret < 0)
		zlog_err("%s: Can't switch to VRF %u (%s)", __func__, vrf_id,
			 safe_strerror(errno));
	ret = getaddrinfo(node, service, hints, res);
	save_errno = errno;
	ret2 = vrf_switchback_to_initial();
	if (ret2 < 0)
		zlog_err("%s: Can't switchback from VRF %u (%s)", __func__,
			 vrf_id, safe_strerror(errno));
	errno = save_errno;
	return ret;
}

int vrf_ioctl(vrf_id_t vrf_id, int d, unsigned long request, char *params)
{
	int ret, saved_errno, rc;

	ret = vrf_switch_to_netns(vrf_id);
	if (ret < 0) {
		zlog_err("%s: Can't switch to VRF %u (%s)", __func__, vrf_id,
			 safe_strerror(errno));
		return 0;
	}
	rc = ioctl(d, request, params);
	saved_errno = errno;
	ret = vrf_switchback_to_initial();
	if (ret < 0)
		zlog_err("%s: Can't switchback from VRF %u (%s)", __func__,
			 vrf_id, safe_strerror(errno));
	errno = saved_errno;
	return rc;
}

int vrf_sockunion_socket(const union sockunion *su, vrf_id_t vrf_id,
			 char *interfacename)
{
	int ret, save_errno, ret2;

	ret = vrf_switch_to_netns(vrf_id);
	if (ret < 0)
		zlog_err("%s: Can't switch to VRF %u (%s)", __func__, vrf_id,
			 safe_strerror(errno));
	ret = sockunion_socket(su);
	save_errno = errno;
	ret2 = vrf_switchback_to_initial();
	if (ret2 < 0)
		zlog_err("%s: Can't switchback from VRF %u (%s)", __func__,
			 vrf_id, safe_strerror(errno));
	errno = save_errno;

	if (ret <= 0)
		return ret;
	ret2 = vrf_bind(vrf_id, ret, interfacename);
	if (ret2 < 0) {
		close(ret);
		ret = ret2;
	}
	return ret;
}