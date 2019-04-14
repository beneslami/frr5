/*
 * Interface function.
 * Copyright (C) 1997, 1999 Kunihiro Ishiguro
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
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

#include "if.h"
#include "vty.h"
#include "sockunion.h"
#include "prefix.h"
#include "command.h"
#include "memory.h"
#include "zebra_memory.h"
#include "ioctl.h"
#include "connected.h"
#include "log.h"
#include "zclient.h"
#include "vrf.h"
#include "filter.h"

#include "zebra/rtadv.h"
#include "zebra_ns.h"
#include "zebra_vrf.h"
#include "zebra/interface.h"
#include "zebra/rib.h"
#include "zebra/rt.h"
#include "zebra/zserv.h"
#include "zebra/redistribute.h"
#include "zebra/debug.h"
#include "zebra/irdp.h"
#include "zebra/zebra_ptm.h"
#include "zebra/rt_netlink.h"
#include "zebra/interface.h"
#include "zebra/zebra_vxlan.h"
#include "zebra/zebra_static.h"

//Reza_Mirzaei: port_security
struct port_sec
{
    char interfce[100];
    char limit[10];
    char flag_s[10];
    int sticky_set;
    int mac_set;
	char mac[100];
    int p_sec_set;
};

#define psec_n 40
struct port_sec psec[psec_n];
int psec_i = 0;
int feature_p_sec;

/*char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    // Count how many elements will be extracted. 
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Add space for trailing token. 
    count += last_comma < (a_str + strlen(a_str) - 1);

    // Add space for terminating null string so caller
     //  knows where the list of returned strings ends. 
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}*/


// o.zeynalpour vpls (Q in Q)
static int enable_qinq=0;

/***************************** o.zeynalpour Radius ***************************/
// Radius code section 
// last modification: 17 july 2018 by M.Moslemi

void insert_aaa(const char *ip1, const char *auth_port2, const char *acct_port3, const char *time_out4, const char *key5) {

  char ip[300];
  char auth[30];
  char acct[30];
  char time[30];
  char  key1[100];
  FILE *fp1;
  FILE *fp2;
  FILE *fp3;

  strcpy(ip, ip1);
  strcpy(auth, auth_port2);
  strcpy(acct, acct_port3);
  strcpy(time, time_out4);
  strcpy(key1, key5);

  system("sudo chmod a+rw /etc/pam_radius_auth.conf");
  fp1 = fopen("/etc/pam_radius_auth.conf", "a+(a+t)");
  if (!fp1) {
    printf("%s", "ERROR: Can't open file '/etc/pam_radius_auth.conf'.\n\n");
    return 0;
  }
 
  //radius-server host { hostname | ip-address }[ timeout seconds ] [ key string] 
  fprintf(fp1,"%s        %s        %s\n\n",ip, key1, time);
  //save file for show radius-server
  system("sudo chmod a+rw /etc/raddb/log_server/server_log.txt");
  fp2 = fopen("/etc/raddb/log_server/server_log.txt", "a+(a+t)");
  if (!fp2){
    printf("%s", "ERROR: Can't open file '/etc/raddb/log_server/server_log.txt'.\n\n");
    return 0;
  }
 fprintf(fp2, "radius_server host %s auth-port %s acct-port %s timeout %s key %s\n", ip, auth, acct, time, key1);
  //end show radius-server

  //save file for show_running-config
  system("sudo chmod a+rw /etc/raddb/log_server/server_log_show_run");
  fp3 = fopen("/etc/raddb/log_server/server_log_show_run","a+(a+t)");
  if (!fp3) {
    printf("%s", "ERROR: Can't open file '/etc/raddb/log_server/server_log_show_run'.\n\n");
    return 0;
  }
  fprintf(fp3, "radius_server host %s auth-port %s acct-port %s timeout %s key %s\n", ip, auth, acct, time, key1);
  //end_show_running-conf

  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
} //end insert_aaa()

/**************************** remove radius_server *****************************/
void no_radius(const char *ip1) {

  char ip[300];
  char str[300];

  strcpy(ip, ip1);
  system("sudo chmod a+rw /etc/pam_radius_auth.conf");

  FILE *fp;
  FILE *fp1;
  FILE *fp2;
  FILE *fp3;
  FILE *fp4;
  fp = fopen("/etc/pam_radius_auth.conf", "a+(a+t)");
  if (!fp) {
    printf("%s", "ERROR: Can't open file '/etc/pam_radius_auth.conf'.\n\n");
    return 0;
  }  
  sprintf(str, "sudo grep -vwE \"%s\" /etc/pam_radius_auth.conf > /etc/raddb/server1.txt", ip);
  system(str);
  system("sudo mv /etc/raddb/server1.txt /etc/pam_radius_auth.conf");

  //remove from show radius-server
  system("sudo chmod a+rw /etc/raddb/log_server/server_log.txt");
  fp1 = fopen("/etc/raddb/log_server/server_log.txt", "a+(a+t)");
  if(!fp1) {
    printf("can't open file '/etc/raddb/log_server/server_log.txt'.\n");
    return 0;
  }
  system("sudo chmod a+rw /etc/raddb/log_server/server_log1.txt ");
  fp2 = fopen("/etc/raddb/log_server/server_log1.txt","a+(a+t)");
  if (!fp2) {
    printf("%s", "ERROR: Can't open file '/etc/raddb/log_server/server_log1.txt'.\n\n");
    return 0;
  }
  sprintf(str, "sudo grep -vwE \"%s\" /etc/raddb/log_server/server_log.txt > /etc/raddb/log_server/server_log1.txt", ip);
  system(str);
  system("sudo cp /etc/raddb/log_server/server_log1.txt /etc/raddb/log_server/server_log.txt");
  // end remove show radius-server

  //remove from show running-config
  system("sudo chmod a+rw /etc/raddb/log_server/server_log_show_run");
  fp3 = fopen("/etc/raddb/log_server/server_log_show_run","a+(a+t)");
  if (!fp3) {
    printf("%s", "ERROR: Can't open file '/etc/raddb/log_server/server_log_show_run'.\n\n");
    return 0;
  }
  system("sudo chmod a+rw /etc/raddb/log_server/server_log_show_run1");
  fp4 = fopen("/etc/raddb/log_server/server_log_show_run1","a+(a+t)");
  if (!fp4) {
    printf("%s", "ERROR: Can't open file '/etc/raddb/log_server/server_log_show_run1'.\n\n");
    return 0;
  }
  sprintf(str, "sudo grep -vwE \"%s\" /etc/raddb/log_server/server_log_show_run > /etc/raddb/log_server/server_log_show_run1", ip);
  system(str);
  system("sudo cp /etc/raddb/log_server/server_log_show_run1 /etc/raddb/log_server/server_log_show_run"); 
  // end remove show running-config

  fclose(fp);
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
} //end no_radius()

/********************************* show radius *********************************/
void show_radius(struct vty *vty) {

  //read a line from file
  FILE *fp;
  fp = fopen("/etc/raddb/log_server/server_log.txt", "r");
  if (!fp) {
    printf("%s", "ERROR: Can't open file '/etc/raddb/log_server/server_log.txt'.\n\n");
    exit (1);
  }
  int size = 1024, pos;
  int c;
  char *buffer = (char *)malloc(size);
  if (fp) {
    do { // read all lines in file
      pos = 0;
      do { // read one line
        c = fgetc(fp);
        if (c != EOF) buffer[pos++] = (char)c;
        if (pos >= size - 1) { // increase buffer length - leave room for 0
          size *= 2;
          buffer = (char*)realloc(buffer, size);
        }
      } while (c != EOF && c != '\n');
      buffer[pos] = 0;
      // line is now in buffer
      vty_out(vty, "%s", buffer);
    } while (c != EOF); 
    fclose(fp);           
  }
  free(buffer);
}//end show_radius()

/**************************** write configure aaa ****************************/
void write_cofigure_radius(char *str) {
  char str1[300];
  char str2[400];
  strcpy(str1, str);
  FILE *fp;

  system("sudo chmod a+rw /etc/raddb/config/radius_confg");
  system("sudo chmod a+rw /etc/raddb/config/radius_confg1.txt");
  sprintf(str2, "sudo grep -vwE \"%s\" /etc/raddb/config/radius_confg >/etc/raddb/config/radius_confg1.txt", str1);
  system(str2);
  system("sudo cp /etc/raddb/config/radius_confg1.txt  /etc/raddb/config/radius_confg"); 
  fp = fopen("/etc/raddb/config/radius_confg", "a+(a+t)");
  if (!fp) {
    printf("%s", "ERROR: Can't open file '/etc/raddb/config/radius_confg'.\n\n");
    return 0;
  }
  fprintf(fp, "%s\n", str1);
  fclose(fp);
} //end write configure aaa

/************************* write configure no aaa ****************************/
write_cofigure_no_radius(char *str) {
  char str1[300];
  strcpy(str1,str);
  FILE *fp;

  system("sudo chmod a+rw /etc/raddb/config/radius_confg");
  //remove all congfigure
  system("sudo  sed -i '1,$d'  /etc/raddb/config/radius_confg");
  system("sudo  sed -i '1,$d'  /etc/raddb/log_server/server_log.txt");
  system("sudo  sed -i '33,$d' /etc/pam_radius_auth.conf");
  //end remove
  fp = fopen("/etc/raddb/config/radius_confg", "a+(a+t)");
  if (!fp) {
    printf("%s", "ERROR: Can't open file '/etc/raddb/config/radius_confg'.\n\n");
    return 0;
  }
  fprintf(fp, "%s\n", str1);
  fclose(fp);
}
//end write configure no aaa

// end of radius code section
/***************************** o.zeynalpour Radius ***************************/
// m.moslemi

//******************************************* o.zeynalpour TACACS+************************************************/
void write_file_show_tacacs_server_host(char *str){
    //start write command line in file show
    FILE *fp1,*fp2;
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server");
    fp1=fopen("/usr/local/rahyab/show_tacacs_server","a+(a+t)");
    if(!fp1){
        printf("can't open file show_tacacs_server\n");
        return 0;
    }
    fprintf(fp1,"Tacacs+ Server     : %s\n",str);
//end show radius-server
//save command line file for show_running-config
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run");
    fp2=fopen("/usr/local/rahyab/show_tacacs_server_run","a+(a+t)");
    if(!fp2){
        printf("can't open file show_tacacs_server_run\n");
        return 0;
    }
    fprintf(fp2,"tacacs-server host %s\n",str);
//end_show_running-conf
    fclose(fp1);
    fclose(fp2);
}
void write_file_show_tacacs_server_key(char *str){
    //start save command line in show_running
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run");
    FILE *fp1;
    fp1=fopen("/usr/local/rahyab/show_tacacs_server_run","a+(a+t)");
    if(!fp1){
        printf("can't open file show_tacacs_server_run\n");
        return 0;
    }
    fprintf(fp1,"tacacs-server key %s\n",str);
//end_show_running-conf
    fclose(fp1);
}
void write_file_show_auth_tacacs(char *str){
    //start save command line in show_running
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run");
    FILE *fp1;
    fp1=fopen("/usr/local/rahyab/show_tacacs_server_run","a+(a+t)");
    if(!fp1){
        printf("can't open file show_tacacs_server_run\n");
        return 0;
    }
    fprintf(fp1,"%s\n",str);
//end_show_running-conf
    fclose(fp1);
}
void write_file_show_author_tacacs(char *str){
    //start save command line in show_running
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run");
    FILE *fp1;
    fp1=fopen("/usr/local/rahyab/show_tacacs_server_run","a+(a+t)");
    if(!fp1){
        printf("can't open file show_tacacs_server_run\n");
        return 0;
    }
    fprintf(fp1,"%s\n",str);
//end_show_running-conf
    fclose(fp1);
}
void write_file_show_acct_tacacs(char *str){
    //start save command line in show_running
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run");
    FILE *fp1;
    fp1=fopen("/usr/local/rahyab/show_tacacs_server_run","a+(a+t)");
    if(!fp1){
        printf("can't open file show_tacacs_server_run\n");
        return 0;
    }
    fprintf(fp1,"%s\n",str);
//end_show_running-conf
    fclose(fp1);
}
void show_tacacs(struct vty *vty){
    //read a line from file
    FILE *fp;
    fp=fopen("/usr/local/rahyab/show_tacacs_server","r");
    if(!fp){
        printf("can't open file show_tacacs_server\n");
        return 0;
    }
    int size = 1024, pos;
    int c;
    char *buffer = (char *)malloc(size);
    if(fp) {
        do { // read all lines in file
            pos = 0;
            do{ // read one line
                c = fgetc(fp);
                if(c != EOF) buffer[pos++] = (char)c;
                if(pos >= size - 1) { // increase buffer length - leave room for 0
                    size *=2;
                    buffer = (char*)realloc(buffer, size);
                }
            }while(c != EOF && c != '\n');
            buffer[pos] = 0;
            // line is now in buffer
            vty_out(vty,"%s",buffer);
        } while(c != EOF);
        fclose(fp);
    }
    free(buffer);
}
void no_tacacs_server_host(char *str){
//remove from show tacacs
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server ");
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_tmp");
    FILE *fp;
    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
    FILE *fp4;
    fp=fopen("/usr/local/rahyab/show_tacacs_server","a+(a+t)");
    if(!fp){
        printf("can't open file\n");
        return 0;
    }
    char str1[300];

    sprintf(str1,"sudo grep -vwE \"%s\" /usr/local/rahyab/show_tacacs_server > /usr/local/rahyab/show_tacacs_server_tmp",str);
    system(str1);
    system("sudo cp /usr/local/rahyab/show_tacacs_server_tmp /usr/local/rahyab/show_tacacs_server");
//remove from file /etc/pam.d/tacacs
    system("sudo chmod a+rw /etc/pam.d/tacacs");
    fp1=fopen("/etc/pam.d/tacacs","a+(a+t)");
    if(!fp1){
        printf("can't open file /etc/pam.d/tacacs\n");
        return 0;
    }
    system("sudo chmod a+rw /etc/pam.d/tacacs_tmp ");
    fp2=fopen("/etc/pam.d/tacacs_tmp","a+(a+t)");
    if(!fp2){
        printf("can't open file /etc/pam.d/tacacs_tmp\n");
        return 0;
    }
    char str2[300];
    sprintf(str2,"sudo sed 's/server=%s//g'   /etc/pam.d/tacacs > /etc/pam.d/tacacs_tmp",str);
    system(str2);
    system("sudo cp  /etc/pam.d/tacacs_tmp  /etc/pam.d/tacacs");
// end remove from pam_tacacs
//remove from show running-config
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run");
    fp3=fopen("/usr/local/rahyab/show_tacacs_server_run","a+(a+t)");
    if(!fp3){
        printf("can't open file\n");
        return 0;
    }
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run_tmp ");
    fp4=fopen("/usr/local/rahyab/show_tacacs_server_run_tmp","a+(a+t)");
    if(!fp4){
        printf("can't open file\n");
        return 0;
    }
    char str3[300];
    sprintf(str3,"sudo grep -vwE \"%s\" /usr/local/rahyab/show_tacacs_server_run > /usr/local/rahyab/show_tacacs_server_run_tmp",str);
    system(str3);
    system("sudo cp /usr/local/rahyab/show_tacacs_server_run_tmp /usr/local/rahyab/show_tacacs_server_run");
// end remove show running-config
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
}
void no_tacacs_server_key(char *str){
    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
    FILE *fp4;
//remove from file /etc/pam.d/tacacs
    system("sudo chmod a+rw /etc/pam.d/tacacs");
    fp1=fopen("/etc/pam.d/tacacs","a+(a+t)");
    if(!fp1){
        printf("can't open file /etc/pam.d/tacacs");
        return 0;
    }
    system("sudo chmod a+rw /etc/pam.d/tacacs_tmp ");
    fp2=fopen("/etc/pam.d/tacacs_tmp","a+(a+t)");
    if(!fp2){
        printf("can't open file\n");
        return 0;
    }
    char str1[300];
    sprintf(str1,"sudo sed 's/secret=%s//g'  /etc/pam.d/tacacs > /etc/pam.d/tacacs_tmp",str);
    system(str1);
    system("sudo cp /etc/pam.d/tacacs_tmp /etc/pam.d/tacacs");
// end remove from pam_tacacs
//remove from show running-config
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run");
    fp3=fopen("/usr/local/rahyab/show_tacacs_server_run","a+(a+t)");
    if(!fp3){
        printf("can't open file\n");
        return 0;
    }
    system("sudo chmod a+rw /usr/local/rahyab/show_tacacs_server_run_tmp ");
    fp4=fopen("/usr/local/rahyab/show_tacacs_server_run_tmp","a+(a+t)");
    if(!fp4){
        printf("can't open file\n");
        return 0;
    }
    char str2[300];
    sprintf(str2,"sudo grep -vwE \"%s\" /usr/local/rahyab/show_tacacs_server_run > /usr/local/rahyab/show_tacacs_server_run_tmp",str);
    system(str2);
    system("sudo cp /usr/local/rahyab/show_tacacs_server_run_tmp /usr/local/rahyab/show_tacacs_server_run");
// end remove show running-config
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
}
//************************END TACACS+ ***********************************//


#define ZEBRA_PTM_SUPPORT

DEFINE_HOOK(zebra_if_extra_info, (struct vty * vty, struct interface *ifp),
(vty, ifp))
DEFINE_HOOK(zebra_if_config_wr, (struct vty * vty, struct interface *ifp),
(vty, ifp))


static void if_down_del_nbr_connected(struct interface *ifp);

static int if_zebra_speed_update(struct thread *thread)
{
    struct interface *ifp = THREAD_ARG(thread);
    struct zebra_if *zif = ifp->info;
    uint32_t new_speed;

    zif->speed_update = NULL;

    new_speed = kernel_get_speed(ifp);
    if (new_speed != ifp->speed) {
        zlog_info("%s: %s old speed: %u new speed: %u",
                  __PRETTY_FUNCTION__, ifp->name, ifp->speed,
                  new_speed);
        ifp->speed = new_speed;
        if_add_update(ifp);
    }

    return 1;
}

static void zebra_if_node_destroy(route_table_delegate_t *delegate,
                                  struct route_table *table,
                                  struct route_node *node)
{
    if (node->info)
        list_delete_and_null((struct list **)&node->info);
    route_node_destroy(delegate, table, node);
}

route_table_delegate_t zebra_if_table_delegate = {
        .create_node = route_node_create,
        .destroy_node = zebra_if_node_destroy};

/* Called when new interface is added. */
static int if_zebra_new_hook(struct interface *ifp)
{
    struct zebra_if *zebra_if;

    zebra_if = XCALLOC(MTYPE_TMP, sizeof(struct zebra_if));

    zebra_if->multicast = IF_ZEBRA_MULTICAST_UNSPEC;
    zebra_if->shutdown = IF_ZEBRA_SHUTDOWN_OFF;
    zebra_ptm_if_init(zebra_if);

    ifp->ptm_enable = zebra_ptm_get_enable_state();
#if defined(HAVE_RTADV)
    {
		/* Set default router advertise values. */
		struct rtadvconf *rtadv;

		rtadv = &zebra_if->rtadv;

		rtadv->AdvSendAdvertisements = 0;
		rtadv->MaxRtrAdvInterval = RTADV_MAX_RTR_ADV_INTERVAL;
		rtadv->MinRtrAdvInterval = RTADV_MIN_RTR_ADV_INTERVAL;
		rtadv->AdvIntervalTimer = 0;
		rtadv->AdvManagedFlag = 0;
		rtadv->AdvOtherConfigFlag = 0;
		rtadv->AdvHomeAgentFlag = 0;
		rtadv->AdvLinkMTU = 0;
		rtadv->AdvReachableTime = 0;
		rtadv->AdvRetransTimer = 0;
		rtadv->AdvCurHopLimit = 0;
		rtadv->AdvDefaultLifetime =
			-1; /* derive from MaxRtrAdvInterval */
		rtadv->HomeAgentPreference = 0;
		rtadv->HomeAgentLifetime =
			-1; /* derive from AdvDefaultLifetime */
		rtadv->AdvIntervalOption = 0;
		rtadv->DefaultPreference = RTADV_PREF_MEDIUM;

		rtadv->AdvPrefixList = list_new();
	}
#endif /* HAVE_RTADV */

    /* Initialize installed address chains tree. */
    zebra_if->ipv4_subnets =
            route_table_init_with_delegate(&zebra_if_table_delegate);

    ifp->info = zebra_if;

    /*
	 * Some platforms are telling us that the interface is
	 * up and ready to go.  When we check the speed we
	 * sometimes get the wrong value.  Wait a couple
	 * of seconds and ask again.  Hopefully it's all settled
	 * down upon startup.
	 */
    thread_add_timer(zebrad.master, if_zebra_speed_update, ifp, 15,
                     &zebra_if->speed_update);
    return 0;
}

/* Called when interface is deleted. */
static int if_zebra_delete_hook(struct interface *ifp)
{
    struct zebra_if *zebra_if;

    if (ifp->info) {
        zebra_if = ifp->info;

        /* Free installed address chains tree. */
        if (zebra_if->ipv4_subnets)
            route_table_finish(zebra_if->ipv4_subnets);
#if defined(HAVE_RTADV)

        struct rtadvconf *rtadv;

		rtadv = &zebra_if->rtadv;
		list_delete_and_null(&rtadv->AdvPrefixList);
#endif /* HAVE_RTADV */

        THREAD_OFF(zebra_if->speed_update);

        XFREE(MTYPE_TMP, zebra_if);
    }

    return 0;
}

/* Build the table key */
static void if_build_key(uint32_t ifindex, struct prefix *p)
{
    p->family = AF_INET;
    p->prefixlen = IPV4_MAX_BITLEN;
    p->u.prefix4.s_addr = ifindex;
}

/* Link an interface in a per NS interface tree */
struct interface *if_link_per_ns(struct zebra_ns *ns, struct interface *ifp)
{
    struct prefix p;
    struct route_node *rn;

    if (ifp->ifindex == IFINDEX_INTERNAL)
        return NULL;

    if_build_key(ifp->ifindex, &p);
    rn = route_node_get(ns->if_table, &p);
    if (rn->info) {
        ifp = (struct interface *)rn->info;
        route_unlock_node(rn); /* get */
        ifp->node = rn;
        return ifp;
    }

    rn->info = ifp;
    ifp->node = rn;

    return ifp;
}

/* Delete a VRF. This is called in vrf_terminate(). */
void if_unlink_per_ns(struct interface *ifp)
{
    ifp->node->info = NULL;
    route_unlock_node(ifp->node);
    ifp->node = NULL;
}

/* Look up an interface by identifier within a NS */
struct interface *if_lookup_by_index_per_ns(struct zebra_ns *ns,
                                            uint32_t ifindex)
{
    struct prefix p;
    struct route_node *rn;
    struct interface *ifp = NULL;

    if_build_key(ifindex, &p);
    rn = route_node_lookup(ns->if_table, &p);
    if (rn) {
        ifp = (struct interface *)rn->info;
        route_unlock_node(rn); /* lookup */
    }
    return ifp;
}

/* Look up an interface by name within a NS */
struct interface *if_lookup_by_name_per_ns(struct zebra_ns *ns,
                                           const char *ifname)
{
    struct route_node *rn;
    struct interface *ifp;

    for (rn = route_top(ns->if_table); rn; rn = route_next(rn)) {
        ifp = (struct interface *)rn->info;
        if (ifp && strcmp(ifp->name, ifname) == 0)
            return (ifp);
    }

    return NULL;
}

/* this function must be used only if the vrf backend
 * is a netns backend
 */
struct interface *if_lookup_by_name_not_ns(ns_id_t ns_id,
                                           const char *ifname)
{
    struct interface *ifp;
    struct ns *ns;

    RB_FOREACH (ns, ns_head, &ns_tree) {
        if (ns->ns_id == ns_id)
            continue;
        /* if_delete_update has removed interface
		 * from zns->if_table
		 * so to look for interface, use the vrf list
		 */
        ifp = if_lookup_by_name(ifname, (vrf_id_t)ns->ns_id);
        if (!ifp)
            continue;
        return ifp;
    }
    return NULL;
}

const char *ifindex2ifname_per_ns(struct zebra_ns *zns, unsigned int ifindex)
{
    struct interface *ifp;

    return ((ifp = if_lookup_by_index_per_ns(zns, ifindex)) != NULL)
           ? ifp->name
           : "unknown";
}

/* Tie an interface address to its derived subnet list of addresses. */
int if_subnet_add(struct interface *ifp, struct connected *ifc)
{
    struct route_node *rn;
    struct zebra_if *zebra_if;
    struct prefix cp;
    struct list *addr_list;

    assert(ifp && ifp->info && ifc);
    zebra_if = ifp->info;

    /* Get address derived subnet node and associated address list, while
	   marking
	   address secondary attribute appropriately. */
    cp = *CONNECTED_PREFIX(ifc);
    apply_mask(&cp);
    rn = route_node_get(zebra_if->ipv4_subnets, &cp);

    if ((addr_list = rn->info))
        SET_FLAG(ifc->flags, ZEBRA_IFA_SECONDARY);
    else {
        UNSET_FLAG(ifc->flags, ZEBRA_IFA_SECONDARY);
        rn->info = addr_list = list_new();
        route_lock_node(rn);
    }

    /* Tie address at the tail of address list. */
    listnode_add(addr_list, ifc);

    /* Return list element count. */
    return (addr_list->count);
}

/* Untie an interface address from its derived subnet list of addresses. */
int if_subnet_delete(struct interface *ifp, struct connected *ifc)
{
    struct route_node *rn;
    struct zebra_if *zebra_if;
    struct list *addr_list;
    struct prefix cp;

    assert(ifp && ifp->info && ifc);
    zebra_if = ifp->info;

    cp = *CONNECTED_PREFIX(ifc);
    apply_mask(&cp);

    /* Get address derived subnet node. */
    rn = route_node_lookup(zebra_if->ipv4_subnets, &cp);
    if (!(rn && rn->info)) {
        zlog_warn(
                "Trying to remove an address from an unknown subnet."
                        " (please report this bug)");
        return -1;
    }
    route_unlock_node(rn);

    /* Untie address from subnet's address list. */
    addr_list = rn->info;

    /* Deleting an address that is not registered is a bug.
	 * In any case, we shouldn't decrement the lock counter if the address
	 * is unknown. */
    if (!listnode_lookup(addr_list, ifc)) {
        zlog_warn(
                "Trying to remove an address from a subnet where it is not"
                        " currently registered. (please report this bug)");
        return -1;
    }

    listnode_delete(addr_list, ifc);
    route_unlock_node(rn);

    /* Return list element count, if not empty. */
    if (addr_list->count) {
        /* If deleted address is primary, mark subsequent one as such
		 * and distribute. */
        if (!CHECK_FLAG(ifc->flags, ZEBRA_IFA_SECONDARY)) {
            ifc = listgetdata(
                    (struct listnode *)listhead(addr_list));
            zebra_interface_address_delete_update(ifp, ifc);
            UNSET_FLAG(ifc->flags, ZEBRA_IFA_SECONDARY);
            /* XXX: Linux kernel removes all the secondary addresses
			 * when the primary
			 * address is removed. We could try to work around that,
			 * though this is
			 * non-trivial. */
            zebra_interface_address_add_update(ifp, ifc);
        }

        return addr_list->count;
    }

    /* Otherwise, free list and route node. */
    list_delete_and_null(&addr_list);
    rn->info = NULL;
    route_unlock_node(rn);

    return 0;
}

/* if_flags_mangle: A place for hacks that require mangling
 * or tweaking the interface flags.
 *
 * ******************** Solaris flags hacks **************************
 *
 * Solaris IFF_UP flag reflects only the primary interface as the
 * routing socket only sends IFINFO for the primary interface.  Hence
 * ~IFF_UP does not per se imply all the logical interfaces are also
 * down - which we only know of as addresses. Instead we must determine
 * whether the interface really is up or not according to how many
 * addresses are still attached. (Solaris always sends RTM_DELADDR if
 * an interface, logical or not, goes ~IFF_UP).
 *
 * Ie, we mangle IFF_UP to *additionally* reflect whether or not there
 * are addresses left in struct connected, not just the actual underlying
 * IFF_UP flag.
 *
 * We must hence remember the real state of IFF_UP, which we do in
 * struct zebra_if.primary_state.
 *
 * Setting IFF_UP within zebra to administratively shutdown the
 * interface will affect only the primary interface/address on Solaris.
 ************************End Solaris flags hacks ***********************
 */
static void if_flags_mangle(struct interface *ifp, uint64_t *newflags)
{
#ifdef SUNOS_5
    struct zebra_if *zif = ifp->info;

	zif->primary_state = *newflags & (IFF_UP & 0xff);

	if (CHECK_FLAG(zif->primary_state, IFF_UP)
	    || listcount(ifp->connected) > 0)
		SET_FLAG(*newflags, IFF_UP);
	else
		UNSET_FLAG(*newflags, IFF_UP);
#endif /* SUNOS_5 */
}

/* Update the flags field of the ifp with the new flag set provided.
 * Take whatever actions are required for any changes in flags we care
 * about.
 *
 * newflags should be the raw value, as obtained from the OS.
 */
void if_flags_update(struct interface *ifp, uint64_t newflags)
{
    if_flags_mangle(ifp, &newflags);

    if (if_is_no_ptm_operative(ifp)) {
        /* operative -> inoperative? */
        ifp->flags = newflags;
        if (!if_is_operative(ifp))
            if_down(ifp);
    } else {
        /* inoperative -> operative? */
        ifp->flags = newflags;
        if (if_is_operative(ifp))
            if_up(ifp);
    }
}

/* Wake up configured address if it is not in current kernel
   address. */
static void if_addr_wakeup(struct interface *ifp)
{
    struct listnode *node, *nnode;
    struct connected *ifc;
    struct prefix *p;
    int ret;

    for (ALL_LIST_ELEMENTS(ifp->connected, node, nnode, ifc)) {
        p = ifc->address;

        if (CHECK_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED)
            && !CHECK_FLAG(ifc->conf, ZEBRA_IFC_QUEUED)) {
            /* Address check. */
            if (p->family == AF_INET) {
                if (!if_is_up(ifp)) {
                    /* Assume zebra is configured like
					 * following:
					 *
					 *   interface gre0
					 *    ip addr 192.0.2.1/24
					 *   !
					 *
					 * As soon as zebra becomes first aware
					 * that gre0 exists in the
					 * kernel, it will set gre0 up and
					 * configure its addresses.
					 *
					 * (This may happen at startup when the
					 * interface already exists
					 * or during runtime when the interface
					 * is added to the kernel)
					 *
					 * XXX: IRDP code is calling here via
					 * if_add_update - this seems
					 * somewhat weird.
					 * XXX: RUNNING is not a settable flag
					 * on any system
					 * I (paulj) am aware of.
					*/
                    if_set_flags(ifp, IFF_UP | IFF_RUNNING);
                    if_refresh(ifp);
                }

                ret = if_set_prefix(ifp, ifc);
                if (ret < 0) {
                    zlog_warn(
                            "Can't set interface's address: %s",
                            safe_strerror(errno));
                    continue;
                }

                SET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);
                /* The address will be advertised to zebra
				 * clients when the notification
				 * from the kernel has been received.
				 * It will also be added to the interface's
				 * subnet list then. */
            }
            if (p->family == AF_INET6) {
                if (!if_is_up(ifp)) {
                    /* See long comment above */
                    if_set_flags(ifp, IFF_UP | IFF_RUNNING);
                    if_refresh(ifp);
                }

                ret = if_prefix_add_ipv6(ifp, ifc);
                if (ret < 0) {
                    zlog_warn(
                            "Can't set interface's address: %s",
                            safe_strerror(errno));
                    continue;
                }

                SET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);
                /* The address will be advertised to zebra
				 * clients when the notification
				 * from the kernel has been received. */
            }
        }
    }
}

/* Handle interface addition */
void if_add_update(struct interface *ifp)
{
    struct zebra_if *if_data;
    struct zebra_ns *zns;
    struct zebra_vrf *zvrf = vrf_info_lookup(ifp->vrf_id);

    /* case interface populate before vrf enabled */
    if (zvrf->zns)
        zns = zvrf->zns;
    else
        zns = zebra_ns_lookup(NS_DEFAULT);
    if_link_per_ns(zns, ifp);
    if_data = ifp->info;
    assert(if_data);

    if (if_data->multicast == IF_ZEBRA_MULTICAST_ON)
        if_set_flags(ifp, IFF_MULTICAST);
    else if (if_data->multicast == IF_ZEBRA_MULTICAST_OFF)
        if_unset_flags(ifp, IFF_MULTICAST);

    zebra_ptm_if_set_ptm_state(ifp, if_data);

    zebra_interface_add_update(ifp);

    if (!CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)) {
        SET_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE);

        if (if_data->shutdown == IF_ZEBRA_SHUTDOWN_ON) {
            if (IS_ZEBRA_DEBUG_KERNEL)
                zlog_debug(
                        "interface %s vrf %u index %d is shutdown. "
                                "Won't wake it up.",
                        ifp->name, ifp->vrf_id, ifp->ifindex);
            return;
        }

        if_addr_wakeup(ifp);

        if (IS_ZEBRA_DEBUG_KERNEL)
            zlog_debug(
                    "interface %s vrf %u index %d becomes active.",
                    ifp->name, ifp->vrf_id, ifp->ifindex);

        static_ifindex_update(ifp, true);
    } else {
        if (IS_ZEBRA_DEBUG_KERNEL)
            zlog_debug("interface %s vrf %u index %d is added.",
                       ifp->name, ifp->vrf_id, ifp->ifindex);
    }
}

/* Install connected routes corresponding to an interface. */
static void if_install_connected(struct interface *ifp)
{
    struct listnode *node;
    struct listnode *next;
    struct connected *ifc;

    if (ifp->connected) {
        for (ALL_LIST_ELEMENTS(ifp->connected, node, next, ifc)) {
            if (CHECK_FLAG(ifc->conf, ZEBRA_IFC_REAL))
                zebra_interface_address_add_update(ifp, ifc);

            connected_up(ifp, ifc);
        }
    }
}

/* Uninstall connected routes corresponding to an interface. */
static void if_uninstall_connected(struct interface *ifp)
{
    struct listnode *node;
    struct listnode *next;
    struct connected *ifc;

    if (ifp->connected) {
        for (ALL_LIST_ELEMENTS(ifp->connected, node, next, ifc)) {
            zebra_interface_address_delete_update(ifp, ifc);
            connected_down(ifp, ifc);
        }
    }
}

/* Uninstall and delete connected routes corresponding to an interface. */
/* TODO - Check why IPv4 handling here is different from install or if_down */
static void if_delete_connected(struct interface *ifp)
{
    struct connected *ifc;
    struct prefix cp;
    struct route_node *rn;
    struct zebra_if *zebra_if;
    struct listnode *node;
    struct listnode *last = NULL;

    zebra_if = ifp->info;

    if (!ifp->connected)
        return;

    while ((node = (last ? last->next : listhead(ifp->connected)))) {
        ifc = listgetdata(node);

        cp = *CONNECTED_PREFIX(ifc);
        apply_mask(&cp);

        if (cp.family == AF_INET
            && (rn = route_node_lookup(zebra_if->ipv4_subnets, &cp))) {
            struct listnode *anode;
            struct listnode *next;
            struct listnode *first;
            struct list *addr_list;

            route_unlock_node(rn);
            addr_list = (struct list *)rn->info;

            /* Remove addresses, secondaries first. */
            first = listhead(addr_list);
            if (first)
                for (anode = first->next; anode || first;
                     anode = next) {
                    if (!anode) {
                        anode = first;
                        first = NULL;
                    }
                    next = anode->next;

                    ifc = listgetdata(anode);
                    connected_down(ifp, ifc);

                    /* XXX: We have to send notifications
					 * here explicitly, because we destroy
					 * the ifc before receiving the
					 * notification about the address being
					 * deleted.
					 */
                    zebra_interface_address_delete_update(
                            ifp, ifc);

                    UNSET_FLAG(ifc->conf, ZEBRA_IFC_REAL);
                    UNSET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);

                    /* Remove from subnet chain. */
                    list_delete_node(addr_list, anode);
                    route_unlock_node(rn);

                    /* Remove from interface address list
					 * (unconditionally). */
                    if (!CHECK_FLAG(ifc->conf,
                                    ZEBRA_IFC_CONFIGURED)) {
                        listnode_delete(ifp->connected,
                                        ifc);
                        connected_free(ifc);
                    } else
                        last = node;
                }

            /* Free chain list and respective route node. */
            list_delete_and_null(&addr_list);
            rn->info = NULL;
            route_unlock_node(rn);
        } else if (cp.family == AF_INET6) {
            connected_down(ifp, ifc);

            zebra_interface_address_delete_update(ifp, ifc);

            UNSET_FLAG(ifc->conf, ZEBRA_IFC_REAL);
            UNSET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);

            if (CHECK_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED))
                last = node;
            else {
                listnode_delete(ifp->connected, ifc);
                connected_free(ifc);
            }
        } else {
            last = node;
        }
    }
}

/* Handle an interface delete event */
void if_delete_update(struct interface *ifp)
{
    struct zebra_if *zif;

    if (if_is_up(ifp)) {
        zlog_err(
                "interface %s vrf %u index %d is still up while being deleted.",
                ifp->name, ifp->vrf_id, ifp->ifindex);
        return;
    }

    if (!CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE))
        return;

    /* Mark interface as inactive */
    UNSET_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE);

    if (IS_ZEBRA_DEBUG_KERNEL)
        zlog_debug("interface %s vrf %u index %d is now inactive.",
                   ifp->name, ifp->vrf_id, ifp->ifindex);

    static_ifindex_update(ifp, false);

    /* Delete connected routes from the kernel. */
    if_delete_connected(ifp);

    /* Send out notification on interface delete. */
    zebra_interface_delete_update(ifp);

    if_unlink_per_ns(ifp);

    /* Update ifindex after distributing the delete message.  This is in
	   case any client needs to have the old value of ifindex available
	   while processing the deletion.  Each client daemon is responsible
	   for setting ifindex to IFINDEX_INTERNAL after processing the
	   interface deletion message. */
    if_set_index(ifp, IFINDEX_INTERNAL);
    ifp->node = NULL;

    /* if the ifp is in a vrf, move it to default so vrf can be deleted if
	 * desired */
    if (ifp->vrf_id)
        if_handle_vrf_change(ifp, VRF_DEFAULT);

    /* Reset some zebra interface params to default values. */
    zif = ifp->info;
    if (zif) {
        zif->zif_type = ZEBRA_IF_OTHER;
        zif->zif_slave_type = ZEBRA_IF_SLAVE_NONE;
        memset(&zif->l2info, 0, sizeof(union zebra_l2if_info));
        memset(&zif->brslave_info, 0,
               sizeof(struct zebra_l2info_brslave));
    }
}

/* VRF change for an interface */
void if_handle_vrf_change(struct interface *ifp, vrf_id_t vrf_id)
{
    vrf_id_t old_vrf_id;

    old_vrf_id = ifp->vrf_id;

    static_ifindex_update(ifp, false);

    /* Uninstall connected routes. */
    if_uninstall_connected(ifp);

    /* Delete any IPv4 neighbors created to implement RFC 5549 */
    if_nbr_ipv6ll_to_ipv4ll_neigh_del_all(ifp);

    /* Delete all neighbor addresses learnt through IPv6 RA */
    if_down_del_nbr_connected(ifp);

    /* Send out notification on interface VRF change. */
    /* This is to issue an UPDATE or a DELETE, as appropriate. */
    zebra_interface_vrf_update_del(ifp, vrf_id);

    /* update VRF */
    if_update_to_new_vrf(ifp, vrf_id);

    /* Send out notification on interface VRF change. */
    /* This is to issue an ADD, if needed. */
    zebra_interface_vrf_update_add(ifp, old_vrf_id);

    /* Install connected routes (in new VRF). */
    if (if_is_operative(ifp))
        if_install_connected(ifp);

    static_ifindex_update(ifp, true);

    /* Due to connected route change, schedule RIB processing for both old
	 * and new VRF.
	 */
    if (IS_ZEBRA_DEBUG_RIB_DETAILED)
        zlog_debug("%u: IF %s VRF change, scheduling RIB processing",
                   ifp->vrf_id, ifp->name);
    rib_update(old_vrf_id, RIB_UPDATE_IF_CHANGE);
    rib_update(ifp->vrf_id, RIB_UPDATE_IF_CHANGE);
}

static void ipv6_ll_address_to_mac(struct in6_addr *address, uint8_t *mac)
{
    mac[0] = address->s6_addr[8] ^ 0x02;
    mac[1] = address->s6_addr[9];
    mac[2] = address->s6_addr[10];
    mac[3] = address->s6_addr[13];
    mac[4] = address->s6_addr[14];
    mac[5] = address->s6_addr[15];
}

void if_nbr_ipv6ll_to_ipv4ll_neigh_update(struct interface *ifp,
                                          struct in6_addr *address, int add)
{
    struct zebra_vrf *zvrf = vrf_info_lookup(ifp->vrf_id);
    struct zebra_if *zif = ifp->info;
    char buf[16] = "169.254.0.1";
    struct in_addr ipv4_ll;
    char mac[6];
    ns_id_t ns_id;

    inet_pton(AF_INET, buf, &ipv4_ll);

    ipv6_ll_address_to_mac(address, (uint8_t *)mac);
    ns_id = zvrf->zns->ns_id;

    /*
	 * Remove existed arp record for the interface as netlink
	 * protocol does not have update message types
	 *
	 * supported message types are RTM_NEWNEIGH and RTM_DELNEIGH
	 */
    kernel_neigh_update(0, ifp->ifindex, ipv4_ll.s_addr, mac, 6, ns_id);

    /* Add arp record */
    kernel_neigh_update(add, ifp->ifindex, ipv4_ll.s_addr, mac, 6, ns_id);

    /*
	 * We need to note whether or not we originated a v6
	 * neighbor entry for this interface.  So that when
	 * someone unwisely accidently deletes this entry
	 * we can shove it back in.
	 */
    zif->v6_2_v4_ll_neigh_entry = !!add;
    memcpy(&zif->v6_2_v4_ll_addr6, address, sizeof(*address));

    zvrf->neigh_updates++;
}

static void if_nbr_ipv6ll_to_ipv4ll_neigh_add_all(struct interface *ifp)
{
    if (listhead(ifp->nbr_connected)) {
        struct nbr_connected *nbr_connected;
        struct listnode *node;

        for (ALL_LIST_ELEMENTS_RO(ifp->nbr_connected, node,
                                  nbr_connected))
            if_nbr_ipv6ll_to_ipv4ll_neigh_update(
                    ifp, &nbr_connected->address->u.prefix6, 1);
    }
}

void if_nbr_ipv6ll_to_ipv4ll_neigh_del_all(struct interface *ifp)
{
    if (listhead(ifp->nbr_connected)) {
        struct nbr_connected *nbr_connected;
        struct listnode *node;

        for (ALL_LIST_ELEMENTS_RO(ifp->nbr_connected, node,
                                  nbr_connected))
            if_nbr_ipv6ll_to_ipv4ll_neigh_update(
                    ifp, &nbr_connected->address->u.prefix6, 0);
    }
}

static void if_down_del_nbr_connected(struct interface *ifp)
{
    struct nbr_connected *nbr_connected;
    struct listnode *node, *nnode;

    for (ALL_LIST_ELEMENTS(ifp->nbr_connected, node, nnode,
                           nbr_connected)) {
        listnode_delete(ifp->nbr_connected, nbr_connected);
        nbr_connected_free(nbr_connected);
    }
}

/* Interface is up. */
void if_up(struct interface *ifp)
{
    struct zebra_if *zif;
    struct interface *link_if;
    struct zebra_vrf *zvrf = vrf_info_lookup(ifp->vrf_id);

    zif = ifp->info;
    zif->up_count++;
    quagga_timestamp(2, zif->up_last, sizeof(zif->up_last));

    /* Notify the protocol daemons. */
    if (ifp->ptm_enable && (ifp->ptm_status == ZEBRA_PTM_STATUS_DOWN)) {
        zlog_warn("%s: interface %s hasn't passed ptm check\n",
                  __func__, ifp->name);
        return;
    }
    zebra_interface_up_update(ifp);

    if_nbr_ipv6ll_to_ipv4ll_neigh_add_all(ifp);

#if defined(HAVE_RTADV)
    /* Enable fast tx of RA if enabled && RA interval is not in msecs */
	if (zif->rtadv.AdvSendAdvertisements
	    && (zif->rtadv.MaxRtrAdvInterval >= 1000)) {
		zif->rtadv.inFastRexmit = 1;
		zif->rtadv.NumFastReXmitsRemain = RTADV_NUM_FAST_REXMITS;
	}
#endif

    /* Install connected routes to the kernel. */
    if_install_connected(ifp);

    /* Install any static routes using this vrf interface */
    if (IS_ZEBRA_IF_VRF(ifp)) {
        static_fixup_vrf_ids(zvrf);
        static_config_install_delayed_routes(zvrf);
    }

    if (IS_ZEBRA_DEBUG_RIB_DETAILED)
        zlog_debug("%u: IF %s up, scheduling RIB processing",
                   ifp->vrf_id, ifp->name);
    rib_update(ifp->vrf_id, RIB_UPDATE_IF_CHANGE);

    /* Handle interface up for specific types for EVPN. Non-VxLAN interfaces
	 * are checked to see if (remote) neighbor entries need to be installed
	 * on them for ARP suppression.
	 */
    if (IS_ZEBRA_IF_VXLAN(ifp))
        zebra_vxlan_if_up(ifp);
    else if (IS_ZEBRA_IF_BRIDGE(ifp)) {
        link_if = ifp;
        zebra_vxlan_svi_up(ifp, link_if);
    } else if (IS_ZEBRA_IF_VLAN(ifp)) {
        link_if = if_lookup_by_index_per_ns(zvrf->zns,
                                            zif->link_ifindex);
        if (link_if)
            zebra_vxlan_svi_up(ifp, link_if);
    }
}

/* Interface goes down.  We have to manage different behavior of based
   OS. */
void if_down(struct interface *ifp)
{
    struct zebra_if *zif;
    struct interface *link_if;
    struct zebra_vrf *zvrf = vrf_info_lookup(ifp->vrf_id);

    zif = ifp->info;
    zif->down_count++;
    quagga_timestamp(2, zif->down_last, sizeof(zif->down_last));

    /* Handle interface down for specific types for EVPN. Non-VxLAN
	 * interfaces
	 * are checked to see if (remote) neighbor entries need to be purged
	 * for ARP suppression.
	 */
    if (IS_ZEBRA_IF_VXLAN(ifp))
        zebra_vxlan_if_down(ifp);
    else if (IS_ZEBRA_IF_BRIDGE(ifp)) {
        link_if = ifp;
        zebra_vxlan_svi_down(ifp, link_if);
    } else if (IS_ZEBRA_IF_VLAN(ifp)) {
        link_if = if_lookup_by_index_per_ns(zvrf->zns,
                                            zif->link_ifindex);
        if (link_if)
            zebra_vxlan_svi_down(ifp, link_if);
    }


    /* Notify to the protocol daemons. */
    zebra_interface_down_update(ifp);

    /* Uninstall connected routes from the kernel. */
    if_uninstall_connected(ifp);

    if (IS_ZEBRA_DEBUG_RIB_DETAILED)
        zlog_debug("%u: IF %s down, scheduling RIB processing",
                   ifp->vrf_id, ifp->name);
    rib_update(ifp->vrf_id, RIB_UPDATE_IF_CHANGE);

    if_nbr_ipv6ll_to_ipv4ll_neigh_del_all(ifp);

    /* Delete all neighbor addresses learnt through IPv6 RA */
    if_down_del_nbr_connected(ifp);
}

void if_refresh(struct interface *ifp)
{
    if_get_flags(ifp);
}

void zebra_if_update_link(struct interface *ifp, ifindex_t link_ifindex)
{
    struct zebra_if *zif;

    zif = (struct zebra_if *)ifp->info;
    zif->link_ifindex = link_ifindex;
    zif->link = if_lookup_by_index_per_ns(zebra_ns_lookup(NS_DEFAULT),
                                          link_ifindex);
}


/* Output prefix string to vty. */
static int prefix_vty_out(struct vty *vty, struct prefix *p)
{
    char str[INET6_ADDRSTRLEN];

    inet_ntop(p->family, &p->u.prefix, str, sizeof(str));
    vty_out(vty, "%s", str);
    return strlen(str);
}

/* Dump if address information to vty. */
static void connected_dump_vty(struct vty *vty, struct connected *connected)
{
    struct prefix *p;

    /* Print interface address. */
    p = connected->address;
    vty_out(vty, "  %s ", prefix_family_str(p));
    prefix_vty_out(vty, p);
    vty_out(vty, "/%d", p->prefixlen);

    /* If there is destination address, print it. */
    if (connected->destination) {
        vty_out(vty,
                (CONNECTED_PEER(connected) ? " peer " : " broadcast "));
        prefix_vty_out(vty, connected->destination);
        if (CONNECTED_PEER(connected))
            vty_out(vty, "/%d", connected->destination->prefixlen);
    }

    if (CHECK_FLAG(connected->flags, ZEBRA_IFA_SECONDARY))
        vty_out(vty, " secondary");

    if (CHECK_FLAG(connected->flags, ZEBRA_IFA_UNNUMBERED))
        vty_out(vty, " unnumbered");

    if (connected->label)
        vty_out(vty, " %s", connected->label);

    vty_out(vty, "\n");
}

/* Dump interface neighbor address information to vty. */
static void nbr_connected_dump_vty(struct vty *vty,
                                   struct nbr_connected *connected)
{
    struct prefix *p;

    /* Print interface address. */
    p = connected->address;
    vty_out(vty, "  %s ", prefix_family_str(p));
    prefix_vty_out(vty, p);
    vty_out(vty, "/%d", p->prefixlen);

    vty_out(vty, "\n");
}

static const char *zebra_ziftype_2str(zebra_iftype_t zif_type)
{
    switch (zif_type) {
        case ZEBRA_IF_OTHER:
            return "Other";
            break;

        case ZEBRA_IF_BRIDGE:
            return "Bridge";
            break;

        case ZEBRA_IF_VLAN:
            return "Vlan";
            break;

        case ZEBRA_IF_VXLAN:
            return "Vxlan";
            break;

        case ZEBRA_IF_VRF:
            return "VRF";
            break;

        default:
            return "Unknown";
            break;
    }
}

/* Interface's information print out to vty interface. */
static void if_dump_vty(struct vty *vty, struct interface *ifp)
{
    struct connected *connected;
    struct nbr_connected *nbr_connected;
    struct listnode *node;
    struct route_node *rn;
    struct zebra_if *zebra_if;
    struct vrf *vrf;

    zebra_if = ifp->info;

    vty_out(vty, "Interface %s is ", ifp->name);
    if (if_is_up(ifp)) {
        vty_out(vty, "up, line protocol ");

        if (CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_LINKDETECTION)) {
            if (if_is_running(ifp))
                vty_out(vty, "is up\n");
            else
                vty_out(vty, "is down\n");
        } else {
            vty_out(vty, "detection is disabled\n");
        }
    } else {
        vty_out(vty, "down\n");
    }

    vty_out(vty, "  Link ups:   %5u    last: %s\n", zebra_if->up_count,
            zebra_if->up_last[0] ? zebra_if->up_last : "(never)");
    vty_out(vty, "  Link downs: %5u    last: %s\n", zebra_if->down_count,
            zebra_if->down_last[0] ? zebra_if->down_last : "(never)");

    zebra_ptm_show_status(vty, ifp);

    vrf = vrf_lookup_by_id(ifp->vrf_id);
    vty_out(vty, "  vrf: %s\n", vrf->name);

    if (ifp->desc)
        vty_out(vty, "  Description: %s\n", ifp->desc);
    if (ifp->ifindex == IFINDEX_INTERNAL) {
        vty_out(vty, "  pseudo interface\n");
        return;
    } else if (!CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)) {
        vty_out(vty, "  index %d inactive interface\n", ifp->ifindex);
        return;
    }

    vty_out(vty, "  index %d metric %d mtu %d speed %u ", ifp->ifindex,
            ifp->metric, ifp->mtu, ifp->speed);
    if (ifp->mtu6 != ifp->mtu)
        vty_out(vty, "mtu6 %d ", ifp->mtu6);
    vty_out(vty, "\n  flags: %s\n", if_flag_dump(ifp->flags));

    /* Hardware address. */
    vty_out(vty, "  Type: %s\n", if_link_type_str(ifp->ll_type));
    if (ifp->hw_addr_len != 0) {
        int i;

        vty_out(vty, "  HWaddr: ");
        for (i = 0; i < ifp->hw_addr_len; i++)
            vty_out(vty, "%s%02x", i == 0 ? "" : ":",
                    ifp->hw_addr[i]);
        vty_out(vty, "\n");
    }

    /* Bandwidth in Mbps */
    if (ifp->bandwidth != 0) {
        vty_out(vty, "  bandwidth %u Mbps", ifp->bandwidth);
        vty_out(vty, "\n");
    }

    for (rn = route_top(zebra_if->ipv4_subnets); rn; rn = route_next(rn)) {
        if (!rn->info)
            continue;

        for (ALL_LIST_ELEMENTS_RO((struct list *)rn->info, node,
                                  connected))
            connected_dump_vty(vty, connected);
    }

    for (ALL_LIST_ELEMENTS_RO(ifp->connected, node, connected)) {
        if (CHECK_FLAG(connected->conf, ZEBRA_IFC_REAL)
            && (connected->address->family == AF_INET6))
            connected_dump_vty(vty, connected);
    }

    vty_out(vty, "  Interface Type %s\n",
            zebra_ziftype_2str(zebra_if->zif_type));
    if (IS_ZEBRA_IF_BRIDGE(ifp)) {
        struct zebra_l2info_bridge *bridge_info;

        bridge_info = &zebra_if->l2info.br;
        vty_out(vty, "  Bridge VLAN-aware: %s\n",
                bridge_info->vlan_aware ? "yes" : "no");
    } else if (IS_ZEBRA_IF_VLAN(ifp)) {
        struct zebra_l2info_vlan *vlan_info;

        vlan_info = &zebra_if->l2info.vl;
        vty_out(vty, "  VLAN Id %u\n", vlan_info->vid);
    } else if (IS_ZEBRA_IF_VXLAN(ifp)) {
        struct zebra_l2info_vxlan *vxlan_info;

        vxlan_info = &zebra_if->l2info.vxl;
        vty_out(vty, "  VxLAN Id %u", vxlan_info->vni);
        if (vxlan_info->vtep_ip.s_addr != INADDR_ANY)
            vty_out(vty, " VTEP IP: %s",
                    inet_ntoa(vxlan_info->vtep_ip));
        if (vxlan_info->access_vlan)
            vty_out(vty, " Access VLAN Id %u",
                    vxlan_info->access_vlan);
        vty_out(vty, "\n");
    }

    if (IS_ZEBRA_IF_BRIDGE_SLAVE(ifp)) {
        struct zebra_l2info_brslave *br_slave;

        br_slave = &zebra_if->brslave_info;
        if (br_slave->bridge_ifindex != IFINDEX_INTERNAL)
            vty_out(vty, "  Master (bridge) ifindex %u\n",
                    br_slave->bridge_ifindex);
    }

    if (zebra_if->link_ifindex != IFINDEX_INTERNAL) {
        vty_out(vty, "  Link ifindex %u", zebra_if->link_ifindex);
        if (zebra_if->link)
            vty_out(vty, "(%s)\n", zebra_if->link->name);
        else
            vty_out(vty, "(Unknown)\n");
    }

    if (HAS_LINK_PARAMS(ifp)) {
        int i;
        struct if_link_params *iflp = ifp->link_params;
        vty_out(vty, "  Traffic Engineering Link Parameters:\n");
        if (IS_PARAM_SET(iflp, LP_TE_METRIC))
            vty_out(vty, "    TE metric %u\n", iflp->te_metric);
        if (IS_PARAM_SET(iflp, LP_MAX_BW))
            vty_out(vty, "    Maximum Bandwidth %g (Byte/s)\n",
                    iflp->max_bw);
        if (IS_PARAM_SET(iflp, LP_MAX_RSV_BW))
            vty_out(vty,
                    "    Maximum Reservable Bandwidth %g (Byte/s)\n",
                    iflp->max_rsv_bw);
        if (IS_PARAM_SET(iflp, LP_UNRSV_BW)) {
            vty_out(vty,
                    "    Unreserved Bandwidth per Class Type in Byte/s:\n");
            for (i = 0; i < MAX_CLASS_TYPE; i += 2)
                vty_out(vty,
                        "      [%d]: %g (Bytes/sec),\t[%d]: %g (Bytes/sec)\n",
                        i, iflp->unrsv_bw[i], i + 1,
                        iflp->unrsv_bw[i + 1]);
        }

        if (IS_PARAM_SET(iflp, LP_ADM_GRP))
            vty_out(vty, "    Administrative Group:%u\n",
                    iflp->admin_grp);
        if (IS_PARAM_SET(iflp, LP_DELAY)) {
            vty_out(vty, "    Link Delay Average: %u (micro-sec.)",
                    iflp->av_delay);
            if (IS_PARAM_SET(iflp, LP_MM_DELAY)) {
                vty_out(vty, " Min:  %u (micro-sec.)",
                        iflp->min_delay);
                vty_out(vty, " Max:  %u (micro-sec.)",
                        iflp->max_delay);
            }
            vty_out(vty, "\n");
        }
        if (IS_PARAM_SET(iflp, LP_DELAY_VAR))
            vty_out(vty,
                    "    Link Delay Variation %u (micro-sec.)\n",
                    iflp->delay_var);
        if (IS_PARAM_SET(iflp, LP_PKT_LOSS))
            vty_out(vty, "    Link Packet Loss %g (in %%)\n",
                    iflp->pkt_loss);
        if (IS_PARAM_SET(iflp, LP_AVA_BW))
            vty_out(vty, "    Available Bandwidth %g (Byte/s)\n",
                    iflp->ava_bw);
        if (IS_PARAM_SET(iflp, LP_RES_BW))
            vty_out(vty, "    Residual Bandwidth %g (Byte/s)\n",
                    iflp->res_bw);
        if (IS_PARAM_SET(iflp, LP_USE_BW))
            vty_out(vty, "    Utilized Bandwidth %g (Byte/s)\n",
                    iflp->use_bw);
        if (IS_PARAM_SET(iflp, LP_RMT_AS))
            vty_out(vty, "    Neighbor ASBR IP: %s AS: %u \n",
                    inet_ntoa(iflp->rmt_ip), iflp->rmt_as);
    }

    hook_call(zebra_if_extra_info, vty, ifp);

    if (listhead(ifp->nbr_connected))
        vty_out(vty, "  Neighbor address(s):\n");
    for (ALL_LIST_ELEMENTS_RO(ifp->nbr_connected, node, nbr_connected))
        nbr_connected_dump_vty(vty, nbr_connected);

#ifdef HAVE_PROC_NET_DEV
    /* Statistics print out using proc file system. */
	vty_out(vty,
		"    %lu input packets (%lu multicast), %lu bytes, "
		"%lu dropped\n",
		ifp->stats.rx_packets, ifp->stats.rx_multicast,
		ifp->stats.rx_bytes, ifp->stats.rx_dropped);

	vty_out(vty,
		"    %lu input errors, %lu length, %lu overrun,"
		" %lu CRC, %lu frame\n",
		ifp->stats.rx_errors, ifp->stats.rx_length_errors,
		ifp->stats.rx_over_errors, ifp->stats.rx_crc_errors,
		ifp->stats.rx_frame_errors);

	vty_out(vty, "    %lu fifo, %lu missed\n", ifp->stats.rx_fifo_errors,
		ifp->stats.rx_missed_errors);

	vty_out(vty, "    %lu output packets, %lu bytes, %lu dropped\n",
		ifp->stats.tx_packets, ifp->stats.tx_bytes,
		ifp->stats.tx_dropped);

	vty_out(vty,
		"    %lu output errors, %lu aborted, %lu carrier,"
		" %lu fifo, %lu heartbeat\n",
		ifp->stats.tx_errors, ifp->stats.tx_aborted_errors,
		ifp->stats.tx_carrier_errors, ifp->stats.tx_fifo_errors,
		ifp->stats.tx_heartbeat_errors);

	vty_out(vty, "    %lu window, %lu collisions\n",
		ifp->stats.tx_window_errors, ifp->stats.collisions);
#endif /* HAVE_PROC_NET_DEV */

#ifdef HAVE_NET_RT_IFLIST
    #if defined(__bsdi__) || defined(__NetBSD__)
	/* Statistics print out using sysctl (). */
	vty_out(vty,
		"    input packets %llu, bytes %llu, dropped %llu,"
		" multicast packets %llu\n",
		(unsigned long long)ifp->stats.ifi_ipackets,
		(unsigned long long)ifp->stats.ifi_ibytes,
		(unsigned long long)ifp->stats.ifi_iqdrops,
		(unsigned long long)ifp->stats.ifi_imcasts);

	vty_out(vty, "    input errors %llu\n",
		(unsigned long long)ifp->stats.ifi_ierrors);

	vty_out(vty,
		"    output packets %llu, bytes %llu,"
		" multicast packets %llu\n",
		(unsigned long long)ifp->stats.ifi_opackets,
		(unsigned long long)ifp->stats.ifi_obytes,
		(unsigned long long)ifp->stats.ifi_omcasts);

	vty_out(vty, "    output errors %llu\n",
		(unsigned long long)ifp->stats.ifi_oerrors);

	vty_out(vty, "    collisions %llu\n",
		(unsigned long long)ifp->stats.ifi_collisions);
#else
	/* Statistics print out using sysctl (). */
	vty_out(vty,
		"    input packets %lu, bytes %lu, dropped %lu,"
		" multicast packets %lu\n",
		ifp->stats.ifi_ipackets, ifp->stats.ifi_ibytes,
		ifp->stats.ifi_iqdrops, ifp->stats.ifi_imcasts);

	vty_out(vty, "    input errors %lu\n", ifp->stats.ifi_ierrors);

	vty_out(vty,
		"    output packets %lu, bytes %lu, multicast packets %lu\n",
		ifp->stats.ifi_opackets, ifp->stats.ifi_obytes,
		ifp->stats.ifi_omcasts);

	vty_out(vty, "    output errors %lu\n", ifp->stats.ifi_oerrors);

	vty_out(vty, "    collisions %lu\n", ifp->stats.ifi_collisions);
#endif /* __bsdi__ || __NetBSD__ */
#endif /* HAVE_NET_RT_IFLIST */
}

static void interface_update_stats(void)
{
#ifdef HAVE_PROC_NET_DEV
    /* If system has interface statistics via proc file system, update
	   statistics. */
	ifstat_update_proc();
#endif /* HAVE_PROC_NET_DEV */
#ifdef HAVE_NET_RT_IFLIST
    ifstat_update_sysctl();
#endif /* HAVE_NET_RT_IFLIST */
}

struct cmd_node interface_node = {INTERFACE_NODE, "%s(config-if)# ", 1};


//Routerlab

/* Return string of filter_type. */
static const char *filter_type_str(struct filter *filter)
{
    switch (filter->type) {
        case FILTER_PERMIT:
            return "permit";
            break;
        case FILTER_DENY:
            return "deny";
            break;
        case FILTER_DYNAMIC:
            return "dynamic";
            break;
        default:
            return "";
            break;
    }
}

static int bit_count(uint32_t i)
{
    int c = 0;
    unsigned int seen_one = 0;

    while (i > 0) {
        if (i & 1) {
            seen_one = 1;
            c++;
        } else {
            if (seen_one) {
                return -1;
            }
        }
        i >>= 1;
    }

    return c;
}

static int mask2prefix(struct in_addr mask)
{
    return bit_count(ntohl(mask.s_addr));
}

DEFUN (show_access_group,
        show_access_group_cmd,
"show access-group",
SHOW_STR
"Configure access-groups the binding of access-list to interfaces\n")
{
char str[200];
sprintf(str,"sudo sh /root/acg.sh");
pipe_cmd(vty,str);
}

DEFUN (show_mac_access_group,
        show_mac_access_group_cmd,
"show mac access-group",
SHOW_STR
"Medium Access Control\n"
"Configure access-groups the binding of access-list to interfaces\n")
{
char str[200];
sprintf(str,"sudo sh /root/mac_acg.sh");
pipe_cmd(vty,str);
}

DEFUN (access_group,
        access_group_cmd,
"access-group WORD <input|output>",
"Apply an access list on interface\n"
"name of the access-list\n"
"Apply on input traffic\n"
"Apply on output traffic\n")

{
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
struct access_list *access;
struct access_master *master;
struct filter *mfilter;
struct filter_cisco *filter;
int write = 0;
char str[400];
char* interface_name;
FILE *pipe;

VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name=nic_names_get_vpp_from_tap(ifp->name);

if(interface_name==NULL){
	vty_out(vty,"no such interface\n");
return CMD_WARNING;
}

access = access_list_lookup(AFI_IP,argv[1]->arg);
if(access->head == NULL || access == NULL){
	vty_out(vty,"no such acl\n");
	return CMD_WARNING;
}
for (mfilter = access->head; mfilter; mfilter = mfilter->next) {

sprintf(str,"sudo vppctl acl add ipv4 ");
filter = &mfilter->u.cfilter;
if (!mfilter->cisco){
	
	char buf[300];
	struct prefix *p = &mfilter->u.zfilter.prefix;
	sprintf(str,"%s %s",str, filter_type_str(mfilter));
	sprintf(str,"%s src %s/%d tag %s",str, inet_ntop(p->family, &p->u.prefix, buf, BUFSIZ),p->prefixlen,access->name);
	vty_out(vty,"%s\n",str);
	pipe_cmd(vty,str);
}
else if (filter->extended){
	
	struct filter_cisco *filt;
	filt = &mfilter->u.cfilter;
	sprintf(str,"%s %s",str,filter_type_str(mfilter));
	
	sprintf(str,"%s src %s/d",str, inet_ntoa(filt->addr));
	sprintf(str,"%s dst %s/d tag %s",str,inet_ntoa(filt->mask), access->name);
	vty_out(vty,"%s\n",str);
	pipe_cmd(vty,str);
}
else {	
	if (filter->addr_mask.s_addr == 0xffffffff){
//		vty_out(vty, " any\n");
		sprintf(str,"%s %s tag %s",str,filter_type_str(mfilter),access->name);
		//vty_out(vty,"%s\n",str);
		pipe_cmd(vty,str);
	}
	else {
		vty_out(vty, " %s",inet_ntoa(filter->addr));
		if (filter->addr_mask.s_addr != 0)
			vty_out(vty,", wildcard bits %s",inet_ntoa(filter->addr_mask));
			vty_out(vty, "\n");
		}
	}
}

FILE *pipe2;
char str2[200];

sprintf(str,"sudo vppctl show acl tag table| tail -n +2 | grep %s | awk '{print $1}' | cut -d '_' -f2",argv[1]->arg);
vty_out(vty,"%s\n",str);
pipe2 = popen(str, "r");
if (pipe2 == NULL) {
return CMD_WARNING;
}

while (fgets(line, LINE_BUFSIZE, pipe2) != NULL) {
	line[strlen(line)-1]='\0';
	vty_out(vty,"%s", line);
	if(strcmp(argv[2]->arg,"input")==0 || strcmp(argv[2]->arg,"in")==0||strcmp(argv[2]->arg,"i")==0||strcmp(argv[2]->arg,"inp")==0 || strcmp(argv[2]->arg,"inpu")==0)
            sprintf(str2,"sudo vppctl acl interface %s apply input %s",interface_name,line);
	if(strcmp(argv[2]->arg,"output")==0||strcmp(argv[2]->arg,"o")==0||strcmp(argv[2]->arg,"ou")==0||strcmp(argv[2]->arg,"out")==0||strcmp(argv[2]->arg,"outp")==0||strcmp(argv[2]->arg,"outpu")==0)
            sprintf(str2,"sudo vppctl acl interface %s apply output %s",interface_name,line);
	vty_out(vty,str2);
	pipe_cmd(vty,str2);
	++linenr;
}

return CMD_SUCCESS;
}


DEFUN (no_access_group,
        no_access_group_cmd,
        "no access-group",
        NO_STR
        "Configure MAC filters\n")
{
char str[400];
char* interface_name;
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name=nic_names_get_vpp_from_tap(ifp->name);
if(interface_name==NULL){
vty_out(vty,"no such interface\n");
return CMD_WARNING;
}

sprintf(str,"sudo vppctl acl interface %s unapply",interface_name);
vty_out(vty,"%s\n",str);
system(str);
}

DEFUN (mac_access_group,
        mac_access_group_cmd,
"mac access-group WORD",
"Configure MAC filters\n"
"Apply an access list on interface\n"
"Specify an access list entry to apply on interface\n")
{
struct access_list *access;
access = access_list_lookup(AFI_L2VPN,argv[2]->arg);
struct filter_cisco ccc;
struct filter *filter;
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;
char* interface_name;
char str[400];

VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name=nic_names_get_vpp_from_tap(ifp->name);
if(interface_name==NULL){
vty_out(vty,"no such interface\n");
return CMD_WARNING;
}

for (filter = access->head; filter; filter = filter->next) {
//filter->u.cfilter.addr_mask.s_addr

struct filter_cisco *cfilter = &filter->u.cfilter;

if (!filter->cisco){
struct filter_zebra *zfilter;
struct prefix *p;
char buf[BUFSIZ];
sprintf(str,"sudo vppctl macip acl add ");
zfilter = &filter->u.zfilter;
p = &zfilter->prefix;

if (p->family == AF_INET6 || p->family == AF_INET)
    vty_out(vty,"Not a MAC Access-list\n");
else if (p->family == AF_ETHERNET) {
    if (p->prefixlen == 0){
        sprintf(str,"%s %s mac ff:ff:ff:ff:ff:ff mask ff:ff:ff:ff:ff:ff tag %s",str,filter_type_str(filter),argv[2]->arg);
        system(str);
        vty_out(vty, "%s\n",str);
    }
    else{
        sprintf(str,"%s %s mac %s mask ff:ff:ff:ff:ff:ff tag %s",str,filter_type_str(filter),prefix_mac2str(&(p->u.prefix_eth),buf,sizeof(buf)),argv[2]->arg);
        system(str);
        vty_out(vty," %s\n",str);
    }
}
//vty_out(vty, "\n");
}
}
vty_out(vty,"acl defined\n");

sprintf(str,"sudo vppctl show macip acl tag table| tail -n +2 | grep %s | awk '{print $1}' | cut -d '_' -f2",argv[2]->arg);
pipe = popen(str, "r");
if (pipe == NULL) {  /* check for errors */
return 1;        /* return with exit code indicating error */
}
vty_out(vty,"pipe is done\n");
while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
line[strlen(line)-1]='\0';
vty_out(vty,"%s", line);
sprintf(str,"vppctl macip acl interface %s apply acl %s\n",interface_name,line);
vty_out(vty,str);
pipe_cmd(vty,str);
++linenr;
}

}

DEFUN (no_mac_access_group,
        no_mac_access_group_cmd,
"no mac access-group",
NO_STR
"Configure MAC filters\n"
"Specify an access list entry to apply on interface\n")
{
char str[400];
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
char* interface_name;
int linenr;
FILE *pipe;


VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name=nic_names_get_vpp_from_tap(ifp->name);
if(interface_name==NULL){
vty_out(vty,"no such interface\n");
return CMD_WARNING;
}
sprintf(str,"vppctl macip acl interface %s unapply ",interface_name);
vty_out(vty,"%s\n",str);
system(str);
}

//Routerlab

/* Show all interfaces to vty. */
DEFUN (show_interface,
        show_interface_cmd,
"show interface [vrf NAME]",
SHOW_STR
"Interface status and configuration\n"
VRF_CMD_HELP_STR)
{
struct vrf *vrf;
struct interface *ifp;
vrf_id_t vrf_id = VRF_DEFAULT;

interface_update_stats();

if (argc > 2)
VRF_GET_ID(vrf_id, argv[3]->arg);

/* All interface print. */
vrf = vrf_lookup_by_id(vrf_id);
FOR_ALL_INTERFACES (vrf, ifp)
if_dump_vty(vty, ifp);

return CMD_SUCCESS;
}


/* Show all interfaces to vty. */
DEFUN (show_interface_vrf_all,
        show_interface_vrf_all_cmd,
"show interface vrf all",
SHOW_STR
"Interface status and configuration\n"
VRF_ALL_CMD_HELP_STR)
{
struct vrf *vrf;
struct interface *ifp;

interface_update_stats();

/* All interface print. */
RB_FOREACH (vrf, vrf_name_head, &vrfs_by_name)
FOR_ALL_INTERFACES (vrf, ifp)
if_dump_vty(vty, ifp);

return CMD_SUCCESS;
}

/* Show specified interface to vty. */

DEFUN (show_interface_name_vrf,
        show_interface_name_vrf_cmd,
"show interface IFNAME vrf NAME",
SHOW_STR
"Interface status and configuration\n"
"Interface name\n"
VRF_CMD_HELP_STR)
{
int idx_ifname = 2;
int idx_name = 4;
struct interface *ifp;
vrf_id_t vrf_id;

interface_update_stats();

VRF_GET_ID(vrf_id, argv[idx_name]->arg);

/* Specified interface print. */
ifp = if_lookup_by_name(argv[idx_ifname]->arg, vrf_id);
if (ifp == NULL) {
vty_out(vty, "%% Can't find interface %s\n",
argv[idx_ifname]->arg);
return CMD_WARNING;
}
if_dump_vty(vty, ifp);

return CMD_SUCCESS;
}

/* Show specified interface to vty. */
DEFUN (show_interface_name_vrf_all,
        show_interface_name_vrf_all_cmd,
"show interface IFNAME [vrf all]",
SHOW_STR
"Interface status and configuration\n"
"Interface name\n"
VRF_ALL_CMD_HELP_STR)
{
int idx_ifname = 2;
struct vrf *vrf;
struct interface *ifp;
int found = 0;

interface_update_stats();

/* All interface print. */
RB_FOREACH (vrf, vrf_name_head, &vrfs_by_name) {
/* Specified interface print. */
ifp = if_lookup_by_name(argv[idx_ifname]->arg, vrf->vrf_id);
if (ifp) {
if_dump_vty(vty, ifp);
found++;
}
}

if (!found) {
vty_out(vty, "%% Can't find interface %s\n",
argv[idx_ifname]->arg);
return CMD_WARNING;
}

return CMD_SUCCESS;
}


static void if_show_description(struct vty *vty, vrf_id_t vrf_id)
{
    struct vrf *vrf = vrf_lookup_by_id(vrf_id);
    struct interface *ifp;

    vty_out(vty, "Interface       Status  Protocol  Description\n");
    FOR_ALL_INTERFACES (vrf, ifp) {
        int len;

        len = vty_out(vty, "%s", ifp->name);
        vty_out(vty, "%*s", (16 - len), " ");

        if (if_is_up(ifp)) {
            vty_out(vty, "up      ");
            if (CHECK_FLAG(ifp->status,
                           ZEBRA_INTERFACE_LINKDETECTION)) {
                if (if_is_running(ifp))
                    vty_out(vty, "up        ");
                else
                    vty_out(vty, "down      ");
            } else {
                vty_out(vty, "unknown   ");
            }
        } else {
            vty_out(vty, "down    down      ");
        }

        if (ifp->desc)
            vty_out(vty, "%s", ifp->desc);
        vty_out(vty, "\n");
    }
}

DEFUN (show_interface_desc,
        show_interface_desc_cmd,
"show interface description [vrf NAME]",
SHOW_STR
"Interface status and configuration\n"
"Interface description\n"
VRF_CMD_HELP_STR)
{
vrf_id_t vrf_id = VRF_DEFAULT;

if (argc > 3)
VRF_GET_ID(vrf_id, argv[4]->arg);

if_show_description(vty, vrf_id);

return CMD_SUCCESS;
}


DEFUN (show_interface_desc_vrf_all,
        show_interface_desc_vrf_all_cmd,
"show interface description vrf all",
SHOW_STR
"Interface status and configuration\n"
"Interface description\n"
VRF_ALL_CMD_HELP_STR)
{
struct vrf *vrf;

RB_FOREACH (vrf, vrf_name_head, &vrfs_by_name)
if (!RB_EMPTY(if_name_head, &vrf->ifaces_by_name)) {
vty_out(vty, "\n\tVRF %u\n\n", vrf->vrf_id);
if_show_description(vty, vrf->vrf_id);
}

return CMD_SUCCESS;
}

DEFUN (multicast,
        multicast_cmd,
"multicast",
"Set multicast flag to interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
int ret;
struct zebra_if *if_data;

if (CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)) {
ret = if_set_flags(ifp, IFF_MULTICAST);
if (ret < 0) {
vty_out(vty, "Can't set multicast flag\n");
return CMD_WARNING_CONFIG_FAILED;
}
if_refresh(ifp);
}
if_data = ifp->info;
if_data->multicast = IF_ZEBRA_MULTICAST_ON;

return CMD_SUCCESS;
}

DEFUN (no_multicast,
        no_multicast_cmd,
"no multicast",
NO_STR
"Unset multicast flag to interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
int ret;
struct zebra_if *if_data;

if (CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)) {
ret = if_unset_flags(ifp, IFF_MULTICAST);
if (ret < 0) {
vty_out(vty, "Can't unset multicast flag\n");
return CMD_WARNING_CONFIG_FAILED;
}
if_refresh(ifp);
}
if_data = ifp->info;
if_data->multicast = IF_ZEBRA_MULTICAST_OFF;

return CMD_SUCCESS;
}

DEFUN (linkdetect,
        linkdetect_cmd,
"link-detect",
"Enable link detection on interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
int if_was_operative;

if_was_operative = if_is_no_ptm_operative(ifp);
SET_FLAG(ifp->status, ZEBRA_INTERFACE_LINKDETECTION);

/* When linkdetection is enabled, if might come down */
if (!if_is_no_ptm_operative(ifp) && if_was_operative)
if_down(ifp);

/* FIXME: Will defer status change forwarding if interface
	   does not come down! */

return CMD_SUCCESS;
}


DEFUN (no_linkdetect,
        no_linkdetect_cmd,
"no link-detect",
NO_STR
"Disable link detection on interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
int if_was_operative;

if_was_operative = if_is_no_ptm_operative(ifp);
UNSET_FLAG(ifp->status, ZEBRA_INTERFACE_LINKDETECTION);

/* Interface may come up after disabling link detection */
if (if_is_operative(ifp) && !if_was_operative)
if_up(ifp);

/* FIXME: see linkdetect_cmd */

return CMD_SUCCESS;
}

DEFUN (shutdown_if,
        shutdown_if_cmd,
"shutdown",
"Shutdown the selected interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
int ret;
struct zebra_if *if_data;

if (ifp->ifindex != IFINDEX_INTERNAL) {
ret = if_unset_flags(ifp, IFF_UP);
if (ret < 0) {
vty_out(vty, "Can't shutdown interface\n");
return CMD_WARNING_CONFIG_FAILED;
}
if_refresh(ifp);
}
if_data = ifp->info;
if_data->shutdown = IF_ZEBRA_SHUTDOWN_ON;

return CMD_SUCCESS;
}

DEFUN (no_shutdown_if,
        no_shutdown_if_cmd,
"no shutdown",
NO_STR
"Shutdown the selected interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
int ret;
struct zebra_if *if_data;

if (ifp->ifindex != IFINDEX_INTERNAL) {
ret = if_set_flags(ifp, IFF_UP | IFF_RUNNING);
if (ret < 0) {
vty_out(vty, "Can't up interface\n");
return CMD_WARNING_CONFIG_FAILED;
}
if_refresh(ifp);

/* Some addresses (in particular, IPv6 addresses on Linux) get
		 * removed when the interface goes down. They need to be
		 * readded.
		 */
if_addr_wakeup(ifp);
}

if_data = ifp->info;
if_data->shutdown = IF_ZEBRA_SHUTDOWN_OFF;

return CMD_SUCCESS;
}

DEFUN (bandwidth_if,
        bandwidth_if_cmd,
"bandwidth (1-100000)",
"Set bandwidth informational parameter\n"
"Bandwidth in megabits\n")
{
int idx_number = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
unsigned int bandwidth;

bandwidth = strtol(argv[idx_number]->arg, NULL, 10);

/* bandwidth range is <1-100000> */
if (bandwidth < 1 || bandwidth > 100000) {
vty_out(vty, "Bandwidth is invalid\n");
return CMD_WARNING_CONFIG_FAILED;
}

ifp->bandwidth = bandwidth;

/* force protocols to recalculate routes due to cost change */
if (if_is_operative(ifp))
zebra_interface_up_update(ifp);

return CMD_SUCCESS;
}

DEFUN (no_bandwidth_if,
        no_bandwidth_if_cmd,
"no bandwidth [(1-100000)]",
NO_STR
"Set bandwidth informational parameter\n"
"Bandwidth in megabits\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

ifp->bandwidth = 0;

/* force protocols to recalculate routes due to cost change */
if (if_is_operative(ifp))
zebra_interface_up_update(ifp);

return CMD_SUCCESS;
}


struct cmd_node link_params_node = {
        LINK_PARAMS_NODE, "%s(config-link-params)# ", 1,
};

static void link_param_cmd_set_uint32(struct interface *ifp, uint32_t *field,
                                      uint32_t type, uint32_t value)
{
    /* Update field as needed */
    if (IS_PARAM_UNSET(ifp->link_params, type) || *field != value) {
        *field = value;
        SET_PARAM(ifp->link_params, type);

        /* force protocols to update LINK STATE due to parameters change
		 */
        if (if_is_operative(ifp))
            zebra_interface_parameters_update(ifp);
    }
}
static void link_param_cmd_set_float(struct interface *ifp, float *field,
                                     uint32_t type, float value)
{

    /* Update field as needed */
    if (IS_PARAM_UNSET(ifp->link_params, type) || *field != value) {
        *field = value;
        SET_PARAM(ifp->link_params, type);

        /* force protocols to update LINK STATE due to parameters change
		 */
        if (if_is_operative(ifp))
            zebra_interface_parameters_update(ifp);
    }
}

static void link_param_cmd_unset(struct interface *ifp, uint32_t type)
{
    if (ifp->link_params == NULL)
        return;

    /* Unset field */
    UNSET_PARAM(ifp->link_params, type);

    /* force protocols to update LINK STATE due to parameters change */
    if (if_is_operative(ifp))
        zebra_interface_parameters_update(ifp);
}

DEFUN_NOSH (link_params,
        link_params_cmd,
"link-params",
LINK_PARAMS_STR)
{
/* vty->qobj_index stays the same @ interface pointer */
vty->node = LINK_PARAMS_NODE;

return CMD_SUCCESS;
}

DEFUN_NOSH (exit_link_params,
        exit_link_params_cmd,
"exit-link-params",
"Exit from Link Params configuration mode\n")
{
if (vty->node == LINK_PARAMS_NODE)
vty->node = INTERFACE_NODE;
return CMD_SUCCESS;
}

/* Specific Traffic Engineering parameters commands */
DEFUN (link_params_enable,
        link_params_enable_cmd,
"enable",
"Activate link parameters on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* This command could be issue at startup, when activate MPLS TE */
/* on a new interface or after a ON / OFF / ON toggle */
/* In all case, TE parameters are reset to their default factory */
if (IS_ZEBRA_DEBUG_EVENT)
zlog_debug(
"Link-params: enable TE link parameters on interface %s",
ifp->name);

if (!if_link_params_get(ifp)) {
if (IS_ZEBRA_DEBUG_EVENT)
zlog_debug(
"Link-params: failed to init TE link parameters  %s",
ifp->name);

return CMD_WARNING_CONFIG_FAILED;
}

/* force protocols to update LINK STATE due to parameters change */
if (if_is_operative(ifp))
zebra_interface_parameters_update(ifp);

return CMD_SUCCESS;
}

DEFUN (no_link_params_enable,
        no_link_params_enable_cmd,
"no enable",
NO_STR
"Disable link parameters on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

zlog_debug("MPLS-TE: disable TE link parameters on interface %s",
ifp->name);

if_link_params_free(ifp);

/* force protocols to update LINK STATE due to parameters change */
if (if_is_operative(ifp))
zebra_interface_parameters_update(ifp);

return CMD_SUCCESS;
}

/* STANDARD TE metrics */
DEFUN (link_params_metric,
        link_params_metric_cmd,
"metric (0-4294967295)",
"Link metric for MPLS-TE purpose\n"
"Metric value in decimal\n")
{
int idx_number = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
uint32_t metric;

metric = strtoul(argv[idx_number]->arg, NULL, 10);

/* Update TE metric if needed */
link_param_cmd_set_uint32(ifp, &iflp->te_metric, LP_TE_METRIC, metric);

return CMD_SUCCESS;
}

DEFUN (no_link_params_metric,
        no_link_params_metric_cmd,
"no metric",
NO_STR
"Disable Link Metric on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* Unset TE Metric */
link_param_cmd_unset(ifp, LP_TE_METRIC);

return CMD_SUCCESS;
}

DEFUN (link_params_maxbw,
        link_params_maxbw_cmd,
"max-bw BANDWIDTH",
"Maximum bandwidth that can be used\n"
"Bytes/second (IEEE floating point format)\n")
{
int idx_bandwidth = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);

float bw;

if (sscanf(argv[idx_bandwidth]->arg, "%g", &bw) != 1) {
vty_out(vty, "link_params_maxbw: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

/* Check that Maximum bandwidth is not lower than other bandwidth
	 * parameters */
if ((bw <= iflp->max_rsv_bw) || (bw <= iflp->unrsv_bw[0])
|| (bw <= iflp->unrsv_bw[1]) || (bw <= iflp->unrsv_bw[2])
|| (bw <= iflp->unrsv_bw[3]) || (bw <= iflp->unrsv_bw[4])
|| (bw <= iflp->unrsv_bw[5]) || (bw <= iflp->unrsv_bw[6])
|| (bw <= iflp->unrsv_bw[7]) || (bw <= iflp->ava_bw)
|| (bw <= iflp->res_bw) || (bw <= iflp->use_bw)) {
vty_out(vty,
"Maximum Bandwidth could not be lower than others bandwidth\n");
return CMD_WARNING_CONFIG_FAILED;
}

/* Update Maximum Bandwidth if needed */
link_param_cmd_set_float(ifp, &iflp->max_bw, LP_MAX_BW, bw);

return CMD_SUCCESS;
}

DEFUN (link_params_max_rsv_bw,
        link_params_max_rsv_bw_cmd,
"max-rsv-bw BANDWIDTH",
"Maximum bandwidth that may be reserved\n"
"Bytes/second (IEEE floating point format)\n")
{
int idx_bandwidth = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
float bw;

if (sscanf(argv[idx_bandwidth]->arg, "%g", &bw) != 1) {
vty_out(vty, "link_params_max_rsv_bw: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

/* Check that bandwidth is not greater than maximum bandwidth parameter
	 */
if (bw > iflp->max_bw) {
vty_out(vty,
"Maximum Reservable Bandwidth could not be greater than Maximum Bandwidth (%g)\n",
iflp->max_bw);
return CMD_WARNING_CONFIG_FAILED;
}

/* Update Maximum Reservable Bandwidth if needed */
link_param_cmd_set_float(ifp, &iflp->max_rsv_bw, LP_MAX_RSV_BW, bw);

return CMD_SUCCESS;
}

DEFUN (link_params_unrsv_bw,
        link_params_unrsv_bw_cmd,
"unrsv-bw (0-7) BANDWIDTH",
"Unreserved bandwidth at each priority level\n"
"Priority\n"
"Bytes/second (IEEE floating point format)\n")
{
int idx_number = 1;
int idx_bandwidth = 2;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
int priority;
float bw;

/* We don't have to consider about range check here. */
if (sscanf(argv[idx_number]->arg, "%d", &priority) != 1) {
vty_out(vty, "link_params_unrsv_bw: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

if (sscanf(argv[idx_bandwidth]->arg, "%g", &bw) != 1) {
vty_out(vty, "link_params_unrsv_bw: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

/* Check that bandwidth is not greater than maximum bandwidth parameter
	 */
if (bw > iflp->max_bw) {
vty_out(vty,
"UnReserved Bandwidth could not be greater than Maximum Bandwidth (%g)\n",
iflp->max_bw);
return CMD_WARNING_CONFIG_FAILED;
}

/* Update Unreserved Bandwidth if needed */
link_param_cmd_set_float(ifp, &iflp->unrsv_bw[priority], LP_UNRSV_BW,
bw);

return CMD_SUCCESS;
}

DEFUN (link_params_admin_grp,
        link_params_admin_grp_cmd,
"admin-grp BITPATTERN",
"Administrative group membership\n"
"32-bit Hexadecimal value (e.g. 0xa1)\n")
{
int idx_bitpattern = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
unsigned long value;

if (sscanf(argv[idx_bitpattern]->arg, "0x%lx", &value) != 1) {
vty_out(vty, "link_params_admin_grp: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

/* Update Administrative Group if needed */
link_param_cmd_set_uint32(ifp, &iflp->admin_grp, LP_ADM_GRP, value);

return CMD_SUCCESS;
}

DEFUN (no_link_params_admin_grp,
        no_link_params_admin_grp_cmd,
"no admin-grp",
NO_STR
"Disable Administrative group membership on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* Unset Admin Group */
link_param_cmd_unset(ifp, LP_ADM_GRP);

return CMD_SUCCESS;
}

/* RFC5392 & RFC5316: INTER-AS */
DEFUN (link_params_inter_as,
        link_params_inter_as_cmd,
"neighbor A.B.C.D as (1-4294967295)",
"Configure remote ASBR information (Neighbor IP address and AS number)\n"
"Remote IP address in dot decimal A.B.C.D\n"
"Remote AS number\n"
"AS number in the range <1-4294967295>\n")
{
int idx_ipv4 = 1;
int idx_number = 3;

VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
struct in_addr addr;
uint32_t as;

if (!inet_aton(argv[idx_ipv4]->arg, &addr)) {
vty_out(vty, "Please specify Router-Addr by A.B.C.D\n");
return CMD_WARNING_CONFIG_FAILED;
}

as = strtoul(argv[idx_number]->arg, NULL, 10);

/* Update Remote IP and Remote AS fields if needed */
if (IS_PARAM_UNSET(iflp, LP_RMT_AS) || iflp->rmt_as != as
|| iflp->rmt_ip.s_addr != addr.s_addr) {

iflp->rmt_as = as;
iflp->rmt_ip.s_addr = addr.s_addr;
SET_PARAM(iflp, LP_RMT_AS);

/* force protocols to update LINK STATE due to parameters change
		 */
if (if_is_operative(ifp))
zebra_interface_parameters_update(ifp);
}
return CMD_SUCCESS;
}

DEFUN (no_link_params_inter_as,
        no_link_params_inter_as_cmd,
"no neighbor",
NO_STR
"Remove Neighbor IP address and AS number for Inter-AS TE\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);

/* Reset Remote IP and AS neighbor */
iflp->rmt_as = 0;
iflp->rmt_ip.s_addr = 0;
UNSET_PARAM(iflp, LP_RMT_AS);

/* force protocols to update LINK STATE due to parameters change */
if (if_is_operative(ifp))
zebra_interface_parameters_update(ifp);

return CMD_SUCCESS;
}

/* RFC7471: OSPF Traffic Engineering (TE) Metric extensions &
 * draft-ietf-isis-metric-extensions-07.txt */
DEFUN (link_params_delay,
        link_params_delay_cmd,
"delay (0-16777215) [min (0-16777215) max (0-16777215)]",
"Unidirectional Average Link Delay\n"
"Average delay in micro-second as decimal (0...16777215)\n"
"Minimum delay\n"
"Minimum delay in micro-second as decimal (0...16777215)\n"
"Maximum delay\n"
"Maximum delay in micro-second as decimal (0...16777215)\n")
{
/* Get and Check new delay values */
uint32_t delay = 0, low = 0, high = 0;
delay = strtoul(argv[1]->arg, NULL, 10);
if (argc == 6) {
low = strtoul(argv[3]->arg, NULL, 10);
high = strtoul(argv[5]->arg, NULL, 10);
}

VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
uint8_t update = 0;

if (argc == 2) {
/* Check new delay value against old Min and Max delays if set
		 */
if (IS_PARAM_SET(iflp, LP_MM_DELAY)
&& (delay <= iflp->min_delay || delay >= iflp->max_delay)) {
vty_out(vty,
"Average delay should be comprise between Min (%d) and Max (%d) delay\n",
iflp->min_delay, iflp->max_delay);
return CMD_WARNING_CONFIG_FAILED;
}
/* Update delay if value is not set or change */
if (IS_PARAM_UNSET(iflp, LP_DELAY) || iflp->av_delay != delay) {
iflp->av_delay = delay;
SET_PARAM(iflp, LP_DELAY);
update = 1;
}
/* Unset Min and Max delays if already set */
if (IS_PARAM_SET(iflp, LP_MM_DELAY)) {
iflp->min_delay = 0;
iflp->max_delay = 0;
UNSET_PARAM(iflp, LP_MM_DELAY);
update = 1;
}
} else {
/* Check new delays value coherency */
if (delay <= low || delay >= high) {
vty_out(vty,
"Average delay should be comprise between Min (%d) and Max (%d) delay\n",
low, high);
return CMD_WARNING_CONFIG_FAILED;
}
/* Update Delays if needed */
if (IS_PARAM_UNSET(iflp, LP_DELAY)
|| IS_PARAM_UNSET(iflp, LP_MM_DELAY)
|| iflp->av_delay != delay || iflp->min_delay != low
|| iflp->max_delay != high) {
iflp->av_delay = delay;
SET_PARAM(iflp, LP_DELAY);
iflp->min_delay = low;
iflp->max_delay = high;
SET_PARAM(iflp, LP_MM_DELAY);
update = 1;
}
}

/* force protocols to update LINK STATE due to parameters change */
if (update == 1 && if_is_operative(ifp))
zebra_interface_parameters_update(ifp);

return CMD_SUCCESS;
}

DEFUN (no_link_params_delay,
        no_link_params_delay_cmd,
"no delay",
NO_STR
"Disable Unidirectional Average, Min & Max Link Delay on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);

/* Unset Delays */
iflp->av_delay = 0;
UNSET_PARAM(iflp, LP_DELAY);
iflp->min_delay = 0;
iflp->max_delay = 0;
UNSET_PARAM(iflp, LP_MM_DELAY);

/* force protocols to update LINK STATE due to parameters change */
if (if_is_operative(ifp))
zebra_interface_parameters_update(ifp);

return CMD_SUCCESS;
}

DEFUN (link_params_delay_var,
        link_params_delay_var_cmd,
"delay-variation (0-16777215)",
"Unidirectional Link Delay Variation\n"
"delay variation in micro-second as decimal (0...16777215)\n")
{
int idx_number = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
uint32_t value;

value = strtoul(argv[idx_number]->arg, NULL, 10);

/* Update Delay Variation if needed */
link_param_cmd_set_uint32(ifp, &iflp->delay_var, LP_DELAY_VAR, value);

return CMD_SUCCESS;
}

DEFUN (no_link_params_delay_var,
        no_link_params_delay_var_cmd,
"no delay-variation",
NO_STR
"Disable Unidirectional Delay Variation on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* Unset Delay Variation */
link_param_cmd_unset(ifp, LP_DELAY_VAR);

return CMD_SUCCESS;
}

DEFUN (link_params_pkt_loss,
        link_params_pkt_loss_cmd,
"packet-loss PERCENTAGE",
"Unidirectional Link Packet Loss\n"
"percentage of total traffic by 0.000003% step and less than 50.331642%\n")
{
int idx_percentage = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
float fval;

if (sscanf(argv[idx_percentage]->arg, "%g", &fval) != 1) {
vty_out(vty, "link_params_pkt_loss: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

if (fval > MAX_PKT_LOSS)
fval = MAX_PKT_LOSS;

/* Update Packet Loss if needed */
link_param_cmd_set_float(ifp, &iflp->pkt_loss, LP_PKT_LOSS, fval);

return CMD_SUCCESS;
}

DEFUN (no_link_params_pkt_loss,
        no_link_params_pkt_loss_cmd,
"no packet-loss",
NO_STR
"Disable Unidirectional Link Packet Loss on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* Unset Packet Loss */
link_param_cmd_unset(ifp, LP_PKT_LOSS);

return CMD_SUCCESS;
}

DEFUN (link_params_res_bw,
        link_params_res_bw_cmd,
"res-bw BANDWIDTH",
"Unidirectional Residual Bandwidth\n"
"Bytes/second (IEEE floating point format)\n")
{
int idx_bandwidth = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
float bw;

if (sscanf(argv[idx_bandwidth]->arg, "%g", &bw) != 1) {
vty_out(vty, "link_params_res_bw: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

/* Check that bandwidth is not greater than maximum bandwidth parameter
	 */
if (bw > iflp->max_bw) {
vty_out(vty,
"Residual Bandwidth could not be greater than Maximum Bandwidth (%g)\n",
iflp->max_bw);
return CMD_WARNING_CONFIG_FAILED;
}

/* Update Residual Bandwidth if needed */
link_param_cmd_set_float(ifp, &iflp->res_bw, LP_RES_BW, bw);

return CMD_SUCCESS;
}

DEFUN (no_link_params_res_bw,
        no_link_params_res_bw_cmd,
"no res-bw",
NO_STR
"Disable Unidirectional Residual Bandwidth on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* Unset Residual Bandwidth */
link_param_cmd_unset(ifp, LP_RES_BW);

return CMD_SUCCESS;
}

DEFUN (link_params_ava_bw,
        link_params_ava_bw_cmd,
"ava-bw BANDWIDTH",
"Unidirectional Available Bandwidth\n"
"Bytes/second (IEEE floating point format)\n")
{
int idx_bandwidth = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
float bw;

if (sscanf(argv[idx_bandwidth]->arg, "%g", &bw) != 1) {
vty_out(vty, "link_params_ava_bw: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

/* Check that bandwidth is not greater than maximum bandwidth parameter
	 */
if (bw > iflp->max_bw) {
vty_out(vty,
"Available Bandwidth could not be greater than Maximum Bandwidth (%g)\n",
iflp->max_bw);
return CMD_WARNING_CONFIG_FAILED;
}

/* Update Residual Bandwidth if needed */
link_param_cmd_set_float(ifp, &iflp->ava_bw, LP_AVA_BW, bw);

return CMD_SUCCESS;
}

DEFUN (no_link_params_ava_bw,
        no_link_params_ava_bw_cmd,
"no ava-bw",
NO_STR
"Disable Unidirectional Available Bandwidth on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* Unset Available Bandwidth */
link_param_cmd_unset(ifp, LP_AVA_BW);

return CMD_SUCCESS;
}

DEFUN (link_params_use_bw,
        link_params_use_bw_cmd,
"use-bw BANDWIDTH",
"Unidirectional Utilised Bandwidth\n"
"Bytes/second (IEEE floating point format)\n")
{
int idx_bandwidth = 1;
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);
float bw;

if (sscanf(argv[idx_bandwidth]->arg, "%g", &bw) != 1) {
vty_out(vty, "link_params_use_bw: fscanf: %s\n",
safe_strerror(errno));
return CMD_WARNING_CONFIG_FAILED;
}

/* Check that bandwidth is not greater than maximum bandwidth parameter
	 */
if (bw > iflp->max_bw) {
vty_out(vty,
"Utilised Bandwidth could not be greater than Maximum Bandwidth (%g)\n",
iflp->max_bw);
return CMD_WARNING_CONFIG_FAILED;
}

/* Update Utilized Bandwidth if needed */
link_param_cmd_set_float(ifp, &iflp->use_bw, LP_USE_BW, bw);

return CMD_SUCCESS;
}

DEFUN (no_link_params_use_bw,
        no_link_params_use_bw_cmd,
"no use-bw",
NO_STR
"Disable Unidirectional Utilised Bandwidth on this interface\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);

/* Unset Utilised Bandwidth */
link_param_cmd_unset(ifp, LP_USE_BW);

return CMD_SUCCESS;
}

static int ip_address_install(struct vty *vty, struct interface *ifp,
                              const char *addr_str, const char *peer_str,
                              const char *label)
{
    struct zebra_if *if_data;
    struct prefix_ipv4 lp, pp;
    struct connected *ifc;
    struct prefix_ipv4 *p;
    int ret;

    if_data = ifp->info;

    ret = str2prefix_ipv4(addr_str, &lp);
    if (ret <= 0) {
        vty_out(vty, "%% Malformed address \n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    if (ipv4_martian(&lp.prefix)) {
        vty_out(vty, "%% Invalid address\n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    if (peer_str) {
        if (lp.prefixlen != 32) {
            vty_out(vty,
                    "%% Local prefix length for P-t-P address must be /32\n");
            return CMD_WARNING_CONFIG_FAILED;
        }

        ret = str2prefix_ipv4(peer_str, &pp);
        if (ret <= 0) {
            vty_out(vty, "%% Malformed peer address\n");
            return CMD_WARNING_CONFIG_FAILED;
        }
    }

    ifc = connected_check_ptp(ifp, &lp, peer_str ? &pp : NULL);
    if (!ifc) {
        ifc = connected_new();
        ifc->ifp = ifp;

        /* Address. */
        p = prefix_ipv4_new();
        *p = lp;
        ifc->address = (struct prefix *)p;

        if (peer_str) {
            SET_FLAG(ifc->flags, ZEBRA_IFA_PEER);
            p = prefix_ipv4_new();
            *p = pp;
            ifc->destination = (struct prefix *)p;
        } else if (p->prefixlen <= IPV4_MAX_PREFIXLEN - 2) {
            p = prefix_ipv4_new();
            *p = lp;
            p->prefix.s_addr = ipv4_broadcast_addr(p->prefix.s_addr,
                                                   p->prefixlen);
            ifc->destination = (struct prefix *)p;
        }

        /* Label. */
        if (label)
            ifc->label = XSTRDUP(MTYPE_CONNECTED_LABEL, label);

        /* Add to linked list. */
        listnode_add(ifp->connected, ifc);
    }

    /* This address is configured from zebra. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED))
        SET_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED);

    /* In case of this route need to install kernel. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_QUEUED)
        && CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)
        && !(if_data && if_data->shutdown == IF_ZEBRA_SHUTDOWN_ON)) {
        /* Some system need to up the interface to set IP address. */
        if (!if_is_up(ifp)) {
            if_set_flags(ifp, IFF_UP | IFF_RUNNING);
            if_refresh(ifp);
        }

        ret = if_set_prefix(ifp, ifc);
        if (ret < 0) {
            vty_out(vty, "%% Can't set interface IP address: %s.\n",
                    safe_strerror(errno));
            return CMD_WARNING_CONFIG_FAILED;
        }

        SET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);
        /* The address will be advertised to zebra clients when the
		 * notification
		 * from the kernel has been received.
		 * It will also be added to the subnet chain list, then. */
    }

    return CMD_SUCCESS;
}

static int ip_address_uninstall(struct vty *vty, struct interface *ifp,
                                const char *addr_str, const char *peer_str,
                                const char *label)
{
    struct prefix_ipv4 lp, pp;
    struct connected *ifc;
    int ret;

    /* Convert to prefix structure. */
    ret = str2prefix_ipv4(addr_str, &lp);
    if (ret <= 0) {
        vty_out(vty, "%% Malformed address \n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    if (peer_str) {
        if (lp.prefixlen != 32) {
            vty_out(vty,
                    "%% Local prefix length for P-t-P address must be /32\n");
            return CMD_WARNING_CONFIG_FAILED;
        }

        ret = str2prefix_ipv4(peer_str, &pp);
        if (ret <= 0) {
            vty_out(vty, "%% Malformed peer address\n");
            return CMD_WARNING_CONFIG_FAILED;
        }
    }

    /* Check current interface address. */
    ifc = connected_check_ptp(ifp, &lp, peer_str ? &pp : NULL);
    if (!ifc) {
        vty_out(vty, "%% Can't find address\n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    /* This is not configured address. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED))
        return CMD_WARNING_CONFIG_FAILED;

    UNSET_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED);

    /* This is not real address or interface is not active. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_QUEUED)
        || !CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)) {
        listnode_delete(ifp->connected, ifc);
        connected_free(ifc);
        return CMD_WARNING_CONFIG_FAILED;
    }

    /* This is real route. */
    ret = if_unset_prefix(ifp, ifc);
    if (ret < 0) {
        vty_out(vty, "%% Can't unset interface IP address: %s.\n",
                safe_strerror(errno));
        return CMD_WARNING_CONFIG_FAILED;
    }
    UNSET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);
    /* we will receive a kernel notification about this route being removed.
	 * this will trigger its removal from the connected list. */
    return CMD_SUCCESS;
}

DEFUN (ip_address,
        ip_address_cmd,
"ip address A.B.C.D/M",
"Interface Internet Protocol config commands\n"
"Set the IP address of an interface\n"
"IP address (e.g. 10.0.0.1/8)\n")
{
int idx_ipv4_prefixlen = 2;
VTY_DECLVAR_CONTEXT(interface, ifp);
return ip_address_install(vty, ifp, argv[idx_ipv4_prefixlen]->arg, NULL,
NULL);
}

DEFUN (no_ip_address,
        no_ip_address_cmd,
"no ip address A.B.C.D/M",
NO_STR
"Interface Internet Protocol config commands\n"
"Set the IP address of an interface\n"
"IP Address (e.g. 10.0.0.1/8)\n")
{
int idx_ipv4_prefixlen = 3;
VTY_DECLVAR_CONTEXT(interface, ifp);
return ip_address_uninstall(vty, ifp, argv[idx_ipv4_prefixlen]->arg,
NULL, NULL);
}

DEFUN(ip_address_peer,
        ip_address_peer_cmd,
"ip address A.B.C.D peer A.B.C.D/M",
"Interface Internet Protocol config commands\n"
"Set the IP address of an interface\n"
"Local IP (e.g. 10.0.0.1) for P-t-P address\n"
"Specify P-t-P address\n"
"Peer IP address (e.g. 10.0.0.1/8)\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
return ip_address_install(vty, ifp, argv[2]->arg, argv[4]->arg, NULL);
}

DEFUN(no_ip_address_peer,
        no_ip_address_peer_cmd,
"no ip address A.B.C.D peer A.B.C.D/M",
NO_STR
"Interface Internet Protocol config commands\n"
"Set the IP address of an interface\n"
"Local IP (e.g. 10.0.0.1) for P-t-P address\n"
"Specify P-t-P address\n"
"Peer IP address (e.g. 10.0.0.1/8)\n")
{
VTY_DECLVAR_CONTEXT(interface, ifp);
return ip_address_uninstall(vty, ifp, argv[3]->arg, argv[5]->arg, NULL);
}

#ifdef HAVE_NETLINK
DEFUN (ip_address_label,
       ip_address_label_cmd,
       "ip address A.B.C.D/M label LINE",
       "Interface Internet Protocol config commands\n"
       "Set the IP address of an interface\n"
       "IP address (e.g. 10.0.0.1/8)\n"
       "Label of this address\n"
       "Label\n")
{
	int idx_ipv4_prefixlen = 2;
	int idx_line = 4;
	VTY_DECLVAR_CONTEXT(interface, ifp);
	return ip_address_install(vty, ifp, argv[idx_ipv4_prefixlen]->arg, NULL,
				  argv[idx_line]->arg);
}

DEFUN (no_ip_address_label,
       no_ip_address_label_cmd,
       "no ip address A.B.C.D/M label LINE",
       NO_STR
       "Interface Internet Protocol config commands\n"
       "Set the IP address of an interface\n"
       "IP address (e.g. 10.0.0.1/8)\n"
       "Label of this address\n"
       "Label\n")
{
	int idx_ipv4_prefixlen = 3;
	int idx_line = 5;
	VTY_DECLVAR_CONTEXT(interface, ifp);
	return ip_address_uninstall(vty, ifp, argv[idx_ipv4_prefixlen]->arg,
				    NULL, argv[idx_line]->arg);
}
#endif /* HAVE_NETLINK */

static int ipv6_address_install(struct vty *vty, struct interface *ifp,
                                const char *addr_str, const char *peer_str,
                                const char *label, int secondary)
{
    struct zebra_if *if_data;
    struct prefix_ipv6 cp;
    struct connected *ifc;
    struct prefix_ipv6 *p;
    int ret;

    if_data = ifp->info;

    ret = str2prefix_ipv6(addr_str, &cp);
    if (ret <= 0) {
        vty_out(vty, "%% Malformed address \n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    if (ipv6_martian(&cp.prefix)) {
        vty_out(vty, "%% Invalid address\n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    ifc = connected_check(ifp, (struct prefix *)&cp);
    if (!ifc) {
        ifc = connected_new();
        ifc->ifp = ifp;

        /* Address. */
        p = prefix_ipv6_new();
        *p = cp;
        ifc->address = (struct prefix *)p;

        /* Secondary. */
        if (secondary)
            SET_FLAG(ifc->flags, ZEBRA_IFA_SECONDARY);

        /* Label. */
        if (label)
            ifc->label = XSTRDUP(MTYPE_CONNECTED_LABEL, label);

        /* Add to linked list. */
        listnode_add(ifp->connected, ifc);
    }

    /* This address is configured from zebra. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED))
        SET_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED);

    /* In case of this route need to install kernel. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_QUEUED)
        && CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)
        && !(if_data && if_data->shutdown == IF_ZEBRA_SHUTDOWN_ON)) {
        /* Some system need to up the interface to set IP address. */
        if (!if_is_up(ifp)) {
            if_set_flags(ifp, IFF_UP | IFF_RUNNING);
            if_refresh(ifp);
        }

        ret = if_prefix_add_ipv6(ifp, ifc);

        if (ret < 0) {
            vty_out(vty, "%% Can't set interface IP address: %s.\n",
                    safe_strerror(errno));
            return CMD_WARNING_CONFIG_FAILED;
        }

        SET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);
        /* The address will be advertised to zebra clients when the
		 * notification
		 * from the kernel has been received. */
    }

    return CMD_SUCCESS;
}

/* Return true if an ipv6 address is configured on ifp */
int ipv6_address_configured(struct interface *ifp)
{
    struct connected *connected;
    struct listnode *node;

    for (ALL_LIST_ELEMENTS_RO(ifp->connected, node, connected))
        if (CHECK_FLAG(connected->conf, ZEBRA_IFC_REAL)
            && (connected->address->family == AF_INET6))
            return 1;

    return 0;
}

static int ipv6_address_uninstall(struct vty *vty, struct interface *ifp,
                                  const char *addr_str, const char *peer_str,
                                  const char *label, int secondry)
{
    struct prefix_ipv6 cp;
    struct connected *ifc;
    int ret;

    /* Convert to prefix structure. */
    ret = str2prefix_ipv6(addr_str, &cp);
    if (ret <= 0) {
        vty_out(vty, "%% Malformed address \n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    /* Check current interface address. */
    ifc = connected_check(ifp, (struct prefix *)&cp);
    if (!ifc) {
        vty_out(vty, "%% Can't find address\n");
        return CMD_WARNING_CONFIG_FAILED;
    }

    /* This is not configured address. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED))
        return CMD_WARNING_CONFIG_FAILED;

    UNSET_FLAG(ifc->conf, ZEBRA_IFC_CONFIGURED);

    /* This is not real address or interface is not active. */
    if (!CHECK_FLAG(ifc->conf, ZEBRA_IFC_QUEUED)
        || !CHECK_FLAG(ifp->status, ZEBRA_INTERFACE_ACTIVE)) {
        listnode_delete(ifp->connected, ifc);
        connected_free(ifc);
        return CMD_WARNING_CONFIG_FAILED;
    }

    /* This is real route. */
    ret = if_prefix_delete_ipv6(ifp, ifc);
    if (ret < 0) {
        vty_out(vty, "%% Can't unset interface IP address: %s.\n",
                safe_strerror(errno));
        return CMD_WARNING_CONFIG_FAILED;
    }

    UNSET_FLAG(ifc->conf, ZEBRA_IFC_QUEUED);
    /* This information will be propagated to the zclients when the
	 * kernel notification is received. */
    return CMD_SUCCESS;
}

DEFUN (ipv6_address,
        ipv6_address_cmd,
"ipv6 address X:X::X:X/M",
"Interface IPv6 config commands\n"
"Set the IP address of an interface\n"
"IPv6 address (e.g. 3ffe:506::1/48)\n")
{
int idx_ipv6_prefixlen = 2;
VTY_DECLVAR_CONTEXT(interface, ifp);
return ipv6_address_install(vty, ifp, argv[idx_ipv6_prefixlen]->arg,
NULL, NULL, 0);
}

DEFUN (no_ipv6_address,
        no_ipv6_address_cmd,
"no ipv6 address X:X::X:X/M",
NO_STR
"Interface IPv6 config commands\n"
"Set the IP address of an interface\n"
"IPv6 address (e.g. 3ffe:506::1/48)\n")
{
int idx_ipv6_prefixlen = 3;
VTY_DECLVAR_CONTEXT(interface, ifp);
return ipv6_address_uninstall(vty, ifp, argv[idx_ipv6_prefixlen]->arg,
NULL, NULL, 0);
}








//install_element(INTERFACE_NODE, &config_if_crypto_map_mapname_cmd);
//install_element(INTERFACE_NODE, &config_if_no_crypto_map_mapname_cmd);



static int link_params_config_write(struct vty *vty, struct interface *ifp)
{
    int i;

    if ((ifp == NULL) || !HAS_LINK_PARAMS(ifp))
        return -1;

    struct if_link_params *iflp = ifp->link_params;

    vty_out(vty, " link-params\n");
    vty_out(vty, "  enable\n");
    if (IS_PARAM_SET(iflp, LP_TE_METRIC) && iflp->te_metric != ifp->metric)
        vty_out(vty, "  metric %u\n", iflp->te_metric);
    if (IS_PARAM_SET(iflp, LP_MAX_BW) && iflp->max_bw != iflp->default_bw)
        vty_out(vty, "  max-bw %g\n", iflp->max_bw);
    if (IS_PARAM_SET(iflp, LP_MAX_RSV_BW)
        && iflp->max_rsv_bw != iflp->default_bw)
        vty_out(vty, "  max-rsv-bw %g\n", iflp->max_rsv_bw);
    if (IS_PARAM_SET(iflp, LP_UNRSV_BW)) {
        for (i = 0; i < 8; i++)
            if (iflp->unrsv_bw[i] != iflp->default_bw)
                vty_out(vty, "  unrsv-bw %d %g\n", i,
                        iflp->unrsv_bw[i]);
    }
    if (IS_PARAM_SET(iflp, LP_ADM_GRP))
        vty_out(vty, "  admin-grp 0x%x\n", iflp->admin_grp);
    if (IS_PARAM_SET(iflp, LP_DELAY)) {
        vty_out(vty, "  delay %u", iflp->av_delay);
        if (IS_PARAM_SET(iflp, LP_MM_DELAY)) {
            vty_out(vty, " min %u", iflp->min_delay);
            vty_out(vty, " max %u", iflp->max_delay);
        }
        vty_out(vty, "\n");
    }
    if (IS_PARAM_SET(iflp, LP_DELAY_VAR))
        vty_out(vty, "  delay-variation %u\n", iflp->delay_var);
    if (IS_PARAM_SET(iflp, LP_PKT_LOSS))
        vty_out(vty, "  packet-loss %g\n", iflp->pkt_loss);
    if (IS_PARAM_SET(iflp, LP_AVA_BW))
        vty_out(vty, "  ava-bw %g\n", iflp->ava_bw);
    if (IS_PARAM_SET(iflp, LP_RES_BW))
        vty_out(vty, "  res-bw %g\n", iflp->res_bw);
    if (IS_PARAM_SET(iflp, LP_USE_BW))
        vty_out(vty, "  use-bw %g\n", iflp->use_bw);
    if (IS_PARAM_SET(iflp, LP_RMT_AS))
        vty_out(vty, "  neighbor %s as %u\n", inet_ntoa(iflp->rmt_ip),
                iflp->rmt_as);
    vty_out(vty, "  exit-link-params\n");
    return 0;
}

// routerlab
DEFUN(ip_dhcp_client,
        ip_dhcp_client_cmd,
"ip dhcp client",
"Interface Internet Protocol config commands\n"
"Configure DHCP parameters for this interface\n"
"DHCP client configuration\n"

)
{
VTY_DECLVAR_CONTEXT(interface, ifp);
struct if_link_params *iflp = if_link_params_get(ifp);;
char str[200];
sprintf(str,"sudo dhclient -v  %s",ifp->name);
system(str);
}

void pipe_cmd (struct vty * vty,char str[]){
    int LINE_BUFSIZE = 100;
    char line[LINE_BUFSIZE];
    int linenr;
    FILE *pipe;
    pipe = popen(str, "r");
    if (pipe == NULL) {
        return 1;
    }

    while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
        vty_out(vty,"%s", line);
        line[strlen(line)-1]='\0';
        ++linenr;
    }
    return 1;

}

DEFUN(xconnect,
        xconnect_cmd,
"xconnect l2tp tunnel-ID ID peer-tunnel-ID PEER session-ID SESSION peer-session-ID PEER_SESSION source A.B.C.D destination A.B.C.D destination_port (1-65535) source_port (1-65535)",
"Create a tunnel interface\n"
"Set the encapsulation of the tunnel to L2TP\n"
"Specify the ID of the tunnel\n"
"A number as the ID of the tunnel\n"
"Specify the ID of the peer tunnel\n"
"A number as the ID of the peer tunnel\n"
"Specify the ID of the session\n"
"A number as the ID of the session\n"
"Specify the ID of the peer session\n"
"A number as the ID of the peer session\n"
"Specify the IP address of the source\n"
"The IP address of the source\n"
"Specify the IP address of the destination\n"
"The IP address of the destination\n"
"Specify the Port number of the source\n"
"The Port number of the source\n"
"Specify port number of the destination\n"
"The Port number of the destination\n")
{
char str[600];
sprintf(str,"sudo ip l2tp add tunnel tunnel_id %s peer_tunnel_id %s local %s remote %s udp_sport %s udp_dport %s 2>&1 \n",argv[3]->arg,argv[5]->arg,argv[11]->arg,argv[13]->arg,argv[15]->arg,argv[17]->arg);
//    vty_out(vty,str);
pipe_cmd(vty,str);


sprintf(str,"sudo ip l2tp add session tunnel_id %s peer_tunnel_id %s session_id %s peer_session_id %s\n 2>&1",argv[3]->arg,argv[5]->arg,argv[7]->arg,argv[9]->arg);
//    vty_out(vty,str);
pipe_cmd(vty,str);
return CMD_SUCCESS;
}

DEFUN(no_xconnect,
        no_xconnect_cmd,
"no xconnect id ID",
NO_STR
"Select or create an interface to configure\n"
"Specify the ID of the xconnect\n"
"ID of the xconnect")
{
char str[500];
sprintf(str,"sudo ip l2tp del tunnel tunnel_id %s",argv[3]->arg);
//vty_out(vty,"%s",str);
pipe_cmd(vty,str);
system(str);
return CMD_SUCCESS;
}

DEFUN(show_xconnect,
        show_xconnect_cmd,
"show xconnect",
"Select or create an interface to configure\n"
"tunnel interface\n")
{
pipe_cmd(vty,"sudo ip l2tp show tunnel");
pipe_cmd(vty,"sudo ip l2tp show session");
return CMD_SUCCESS;
}

//GRE add& remove $ show
DEFUN(interface_tunnel,
        interface_tunnel_cmd,
"interface tunnel IFNAME source A.B.C.D destination A.B.C.D",
"Select or create an interface to configure\n"
"tunnel interface\n"
"Name of the interface\n"
"Specify source IP address of the tunnel\n"
"Enter the source IP address of the tunnel\n"
"Specify the destination IP address of the tunnel\n"
"Enter the destination IP address of the tunnel\n")
{
char str[500];
sprintf(str,"sudo ip tunnel add %s mode gre local %s remote %s ttl 255",argv[2]->arg,argv[4]->arg,argv[6]->arg);
pipe_cmd(vty,str);
//vty_out(vty,"%s",str);
return CMD_SUCCESS;
}

DEFUN(no_interface_tunnel,
        no_interface_tunnel_cmd,
"no interface tunnel IFNAME",
NO_STR
"Select or create an interface to configure\n"
"tunnel interface\n"
"Enter name of the tunnel interface\n")
{
char str[500];
sprintf(str,"sudo ip link set %s down",argv[3]->arg);
pipe_cmd(vty,str);
sprintf(str,"sudo ip tunnel del %s",argv[3]->arg);
//    vty_out(vty,str);
pipe_cmd(vty,str);
return CMD_SUCCESS;
}

DEFUN(show_interface_tunnel,
        show_interface_tunnel_cmd,
"show interface tunnel",
SHOW_STR
"interface status & configurations\n"
"tunnels status & configuration\n")
{
pipe_cmd(vty,"sudo ip tunnel show");
return CMD_SUCCESS;
}


//NAT
/*Static NAT*/
//=======================================================
// To save iptables nat commmand in a file to use for boot time
void iptable_save(struct vty *vty)
{
    //save iptables Nat table in a file for changing and restore in boot time
    int ret=system("sudo iptables-save -t nat > iptables.rules");
    if (ret==-1){
        vty_out(vty,"Linux Error in save iptables.rules");
    }
}
//==================================================

void iptable_restore(struct vty *vty)
{
    //see if iptables.rules exist?
    if (file_exist("iptables.rules",vty))
    {
        //restore saved ip table rules if file exist
        int ret=system("sudo iptables-restore iptables.rules");
        if (ret==-1){
            vty_out(vty,"Linux Error in save iptables.rules");
        }
    }
}
//======================================================
int insert_table(char *command,int flag,struct vty *vty)
{
    //flag 0=search, 1=find & insert, 2=find & delete
    FILE *pipe;
    char str[300];
    char line[300];
    int find=0;
    strcpy(str,"sudo grep -w '");
    strcat(str,command);
    strcat(str,"' NAT-PATCommand.txt");
    pipe = popen(str, "r");
    if (pipe == NULL) {  // check for popen error
        vty_out(vty,"\n Error:");
        return 1;        /* return with exit code indicating error */
    }// end check for popen error
    if(fgets(line, 100, pipe)!=NULL)//check to find if the command is defined before
    {
        // vty_out(vty,"command finded");
        int find=1;
        if (flag==0)// if just find this command in file
        {
            return find;
        }
    }
    if (flag==1 && find)//the command is duplicate
    {
        return 3;
    }

    if (flag==1 && find)//the command is duplicate
    {
        return 2;
    }

    if (flag==2 && !find)// command not find - do nothing
    {
        return 5;
    }
    if (flag==2 && find)// delete the command
    {
        return 4;
    }

    //return 0 do not find
    //1 find
    //2 find and insert
    //3 find and is duplicate
    //4 find and delete
    //5 not find and could not delete
}
//=========================================================
char* get_o_interface(struct vty *vty)
{
    int LINE_BUFSIZE = 100;
    char line[100];
    char command[40];
    FILE *pipe;
    char *inter;
    inter="all";
    if(!file_exist(nat_o_interface_file,vty ))
    {
        //not yet defined an output interface
        //and all interfacese are output nat interface
        vty_out(vty,"saaalaaam");
        return inter;
    } //else get the interface name from the file
    //strcpy(command,"sudo head -1 ");
    strcpy(command,"sudo sed -n 1p ");
    strcat(command,nat_o_interface_file);
    pipe = popen(command, "r");
    if (pipe == NULL) {  // check for errors
        vty_out(vty,"Error");
        return 1;        // return with exit code indicating error
    }//end pipe error
    fgets(line, 20 , pipe);
    inter= strtok (line, "\n");
    if (inter==NULL)
    {
        inter="all";
    }
    return inter;
}
//===================================================================================
int file_exist(char file_name[30],struct vty *vty)
{
    //to check if file "file_name" exist

    char command[100];
    char str[100];
    FILE *pipe;
    char line[100];

    // [ -f NATPool.txt ] && echo "Found" || echo "Not found"
    strcpy(command,"[ -f ");
    strcat(command,file_name);
    strcat(command," ] && echo \"Found\" || echo \"Not found\"");
    pipe = popen(command, "r");
    if (pipe == NULL) {
        vty_out(vty,"Error in finding file: ");
        vty_out(vty,file_name);
        return 0;
    }
    fgets(line, 100, pipe);
    if(line=="Found")
    {
        //vty_out(vty,"File exist: ");
        //vty_out(vty,file_name);
        return 1;
    }else if (line=="Not Found")
    {
        return 0;
    }
}
//===============================================================================
DEFUN(show_ip_nat_translation,
        show_ip_nat_translation_cmd,
"show ip nat translation",
"running system information\n"
"IP information\n"
"IP NAT information\n"
"Translation entries\n"
)
{
int LINE_BUFSIZE = 1000;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

/* Get a pipe where the output from the scripts comes in */
pipe = popen("sudo iptables -nL POSTROUTING -t nat --line-number", "r");
if (pipe == NULL) {  /* check for errors */
perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
}
//===========================================================
DEFUN(show_ip_nat_statistics_v,
        show_ip_nat_statistics_v_cmd,
"show ip nat statistics v",
"running system information\n"
"IP information\n"
"IP NAT information\n"
"Number of translated packets\n"
"Enable verbose for more Information\n"
)
{
int LINE_BUFSIZE = 1000;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

/* Get a pipe where the output from the scripts comes in */
pipe = popen("sudo iptables -nvL POSTROUTING -t nat --line-number -v", "r");
if (pipe == NULL) {  /* check for errors */
perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
vty_out(vty,"\n");
}
//=========================================================
DEFUN(show_ip_nat_statistics,
        show_ip_nat_statistics_cmd,
"show ip nat statistics",
"running system information\n"
"IP information\n"
"IP NAT information\n"
"Packets forwarded\n"
)
{
int LINE_BUFSIZE = 1000;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

//get nat dynamic and pat commands from NAT-PAT table
vty_out(vty," Outside Interface: ");
vty_out(vty,get_o_interface(vty));
vty_out(vty,"\n Dynamic mappings: \n");

pipe = popen(" sudo cat NAT-PATCommand.txt", "r");
if (pipe == NULL) {  /* check for errors */
perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}
while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
/* Get a pipe where the output from the scripts comes in */
pipe = popen("sudo iptables -nvL POSTROUTING -t nat --line-number", "r");
if (pipe == NULL) {  /* check for errors */
perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}
while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
}
//===========================================================
DEFUN(ip_nat_pool,
        ip_nat_pool_cmd,
"ip nat pool WORD A.B.C.D A.B.C.D ",
"Internet Protocol\n"
"Network Address Translation\n"
"Define pool of address\n"
"Pool name\n"
"Start IP address\n"
"End IP address\n"

)
{
char str[300];
char command[100];
char line[100],line1[100];
FILE *pipe,*pipe1;

//find by pool name and if is delete it
//add a new one to file
//update table to change who used this pool name
// vty_out(vty,"%s", argv[5]->arg);
strcpy(pool_name,argv[3]->arg);
strcpy(start_ip,argv[4]->arg);
strcpy(end_ip,argv[5]->arg);
pipe1 = popen("[ -f NATPool.txt ] && echo \"Found\" || echo \"Not found\"", "r");
if (pipe1 == NULL) {
return 1;
}
fgets(line1, 100, pipe1);
if(line1=="Found")//if NATPool file exist, search for new pool name to see if is duplicate
{
strcpy(command,"sudo grep -w ");
strcat(command,argv[3]->arg);
strcat(command," NATPool.txt");
pipe = popen(command, "r");
//vty_out(vty,pipe);

if (pipe == NULL) {  // check for popen error
vty_out(vty,"Error:");
return 1;        /* return with exit code indicating error */
}// end check for popen error

//IF POOL IS DUPLICATE SHOW ERROR MESSAGE
fgets(line, 100, pipe);
vty_out(vty,line);
if(line!=NULL)
{
vty_out(vty,"Error: This natpool existing!");
return 1;
}
}else if(line1=="Not Found")//if NATpool not exist
{

}
// The pool is new and add to pool file
//else pool is not duplicate and insert into file
strcpy(str," echo ");
strcat(str,pool_name);
strcat(str," ");
strcat(str,start_ip);
strcat(str," ");
strcat(str,end_ip);
strcat(str," >> NATPool.txt");

//   vty_out(vty,"%s", argv[5]->arg);

//    vty_out(vty,"%s",str);
int ret=system(str);
if (ret==-1){
vty_out(vty,"error");
}
con_pool = con_pool + 1;
// call pool change
}
//==========================================================
DEFUN(ip_nat_inside_source_network,
        ip_nat_inside_source_network_cmd,
"ip nat inside source network A.B.C.D/M pool WORD",
"IP information\n"
"IP NAT information\n"
"Inside address translation\n"
"Source address translation\n"
"Network IP describing local addresses for dynamic NAT\n"
"Specify range IP describing local addresses\n"
"Name pool of global addresses\n"
"Pool name of global addresses\n"
)

{
clear_var();
char str[300];
char table_command[300],temp_command[100];
char command[40];
char line[100];
int ans=0;
FILE *pipe;
FILE *pipe1;
strcpy(source_ip,argv[5]->arg);
char search_command[100];

//if find command is duplicate
strcpy(search_command,"ip nat inside source network ");
strcat(search_command,argv[5]->arg);
strcat(search_command," pool ");
strcat(search_command,argv[7]->arg);
strcpy(temp_command,search_command);// to search if overload of command is defined
strcat(temp_command," overload");

ans=insert_table(temp_command,0,vty);
if (ans==1)//command is defined before as overload and is duplicate
{
vty_out(vty,"Error: similar command existed! \nSimilar command: ");
vty_out(vty,temp_command);
return 1;        /* return with exit code indicating error */
}
ans=insert_table(search_command,0,vty);//to search if command is duplicate and non overload
if (ans==1)//command is defined before and is duplicate
{
vty_out(vty,"Error: Command exist!");
return 1;        /* return with exit code indicating error */
}

if(!file_exist(pool_file,vty))//then search and find the pool
{
vty_out(vty,"File :");
vty_out(vty,pool_file);
vty_out(vty,"pool not exist!  Define a nat pool");
return 1;        /* return with exit code indicating error */
}
//search to find if pool is duplicate
strcpy(command,"sudo grep -w ");
strcat(command,argv[7]->arg);
strcat(command," NATPool.txt");
pipe = popen(command, "r");
//vty_out(vty,command);
if (pipe == NULL) {  // check for popen error
vty_out(vty,"\n Error:");
return 1;        /* return with exit code indicating error */
}// end check for popen error
if(fgets(line, 100, pipe)==NULL)//check to find if pool is defined before?
{
vty_out(vty," Erorr: The pool '");
vty_out(vty,argv[7]->arg);
vty_out(vty,"' is not defined! ");
return 1;
}
char *pch;
pch = strtok (line," ");
pch = strtok (NULL, " ");
strcpy(start_ip,pch);
pch = strtok (NULL, " ");
strcpy(end_ip,pch);
char iface[30];
strcpy(iface,get_o_interface(vty));
pch = strtok (iface, "\n");
//iptables command
strcpy(str,"sudo iptables -t nat -I POSTROUTING -o ");
strcat(str,iface);
strcat(str," -s ");
strcat(str,source_ip);
strcat(str," -j SNAT --to-source ");
strcat(str,start_ip);
strcat(str,"-");
strcat(str,end_ip);
//strcat(iptables_input,str);
//vty_out(vty,"%s", str);
int ret=system(str);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}
//At the end is time for insert nat command into the top of the table
strcpy(table_command,"sudo echo ip nat inside source network ");
strcat(table_command,argv[5]->arg);
strcat(table_command," pool ");
strcat(table_command,argv[7]->arg);
strcat(table_command," D");
strcat(table_command," > tempfile.txt \n cat NAT-PATCommand.txt >> tempfile.txt \n cat tempfile.txt > NAT-PATCommand.txt");
//vty_out(vty,table_command);
ret=system(table_command);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}
iptable_save(vty);
}
//==========================================================
DEFUN(ip_nat_inside_source_network_overload ,
        ip_nat_inside_source_network_overload_cmd,
"ip nat inside source network A.B.C.D/M pool WORD overload",
"IP information\n"
"IP NAT information\n"
"Inside address translation\n"
"Source address translation\n"
"Network IP describing local addresses for dynamic NAT\n"
"Specify range IP describing local addresses\n"
"Name pool of global addresses\n"
"Pool name of global addresses\n"
"overload the pool addresses\n"
)

{
clear_var();
char str[300];
char table_command[300];
char command[40];
char line[100];
int ans=0;
FILE *pipe;
FILE *pipe1;
strcpy(source_ip,argv[5]->arg);
char search_command[100],temp_command[100];
char *prot[p_num];//overloading just for 2 protocol and one none overload for all other protocols
prot[0] = " -p all ";
prot[1] = " -p udp ";
prot[2] = " -p tcp ";
int ret;
//if find command is duplicate
strcpy(search_command,"ip nat inside source network ");
strcat(search_command,argv[ 5]->arg);
strcat(search_command," pool ");
strcat(search_command,argv[7]->arg);
strcpy(temp_command,search_command);//to not add D to end of the search_command because later add overload
strcat(temp_command," D");
ans=insert_table(temp_command,0,vty);
if (ans==1)//command is defined before as not overload and is duplicate
{
vty_out(vty,"Error:Similar Command exist!\nSimilar command: ");
vty_out(vty,search_command);
return 1;        /* return with exit code indicating error */
}
strcat(search_command," ");
strcat(search_command,argv[8]->arg);
//to search if overload command existed
ans=insert_table(search_command,0,vty);
if (ans==1)//command is defined before as not overload and is duplicate
{
vty_out(vty,"Error: Command exist!\n");
return 1;        /* return with exit code indicating error */
}
if(!file_exist(pool_file,vty))//then search and find the pool
{
vty_out(vty,"File :");
vty_out(vty,pool_file);
vty_out(vty,"pool not exist!  Define a nat pool");
return 1;        /* return with exit code indicating error */
}
//search to find if pool is duplicate
strcpy(command,"sudo grep -w ");
strcat(command,argv[7]->arg);
strcat(command," NATPool.txt");
pipe = popen(command, "r");
//vty_out(vty,command);

if (pipe == NULL) {  // check for popen error
vty_out(vty,"\n Error:");
return 1;        /* return with exit code indicating error */
}// end check for popen error


if(fgets(line, 100, pipe)==NULL)//check to find if pool is defined before?
{
vty_out(vty," Erorr: The pool '");
vty_out(vty,argv[7]->arg);
vty_out(vty,"' is not defined! ");
return 1;
}
char *pch;
pch = strtok (line," ");
pch = strtok (NULL, " ");
strcpy(start_ip,pch);
pch = strtok (NULL, " ");
pch = strtok (pch, "\n");
strcpy(end_ip,pch);

char iface[30];
strcpy(iface,get_o_interface(vty));
pch = strtok (iface, "\n");
//iptables command
int i;
for( i=0; i<p_num ;i++)
{
strcpy(str,"sudo iptables -t nat -I POSTROUTING -o ");
strcat(str,iface);
strcat(str,prot[i]);
strcat(str," -s ");
strcat(str,source_ip);
strcat(str," -j SNAT --to-source ");
strcat(str,start_ip);
strcat(str,"-");
strcat(str,end_ip);
if(i)// could not add port nat for all protocols by -p all and should name the
{
strcat(str,":2000-60000");
}
strcat(str,"\n");
vty_out(vty,"%s", str);
}
ret=system(str);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}
//At the end is time for insert nat command into the top of the table
strcpy(table_command,"sudo echo ip nat inside source network ");
strcat(table_command,argv[5]->arg);
strcat(table_command," pool ");
strcat(table_command,argv[7]->arg);
strcat(table_command," overload ");
strcat(table_command," > tempfile.txt \n cat NAT-PATCommand.txt >> tempfile.txt \n cat tempfile.txt > NAT-PATCommand.txt");
//vty_out(vty,table_command);
ret=system(table_command);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}
iptable_save(vty);
}

//============================================================
DEFUN(ip_nat_inside_source_static,
        ip_nat_inside_source_static_cmd,
"ip nat inside source static A.B.C.D A.B.C.D ",
"IP information\n"
"IP NAT information\n"
"Inside address translation\n"
"Source address translation\n"
"Specify static local->global mapping\n"
"Inside local IP address\n"
"Inside global IP address\n")
{
clear_var();
char iface[30];
char str[300];
strcpy(iface,get_o_interface(vty));
strcpy(str,"sudo iptables -t nat -I POSTROUTING  -o ");
strcat(str,iface);
strcat(str," -s ");
strcpy(loc_ip,argv[5]->arg);
strcpy(glo_ip,argv[6]->arg);
strcat(str,loc_ip);
strcat(str," -j SNAT --to-source ");
strcat(str,glo_ip);
//	vty_out(vty,"%s\n",str);
int ret=system(str);
if (ret==-1){
vty_out(vty,"error");
}
iptable_save(vty);
}
//==========================================================================
DEFUN(ip_nat_inside,
        ip_nat_inside_cmd,
"ip nat inside ",
"IP information\n"
"IP NAT information\n"
"Inside interface for address translation\n"

)
{
char str[100];
char *last_iface;
//struct interface *ifp = NULL;
VTY_DECLVAR_CONTEXT(interface, ifp);
//vty_out(vty,"%s", ifp->name);
//change all -o interface in iptables.rules
//Later: search and just change lines which have SNAT
//iptables_replace("ens160" ,ifp->name);
last_iface = get_o_interface(vty);
iptables_replace(last_iface ,ifp->name,vty);
//insert interface name to the file
strcpy(str,"sudo echo ");
strcat(str,ifp->name);
strcat(str," > ");
strcat(str,nat_o_interface_file);
int ret=system(str);
if (ret==-1){
vty_out(vty,"error");
}
iptable_save(vty);
//Problem:
// In cisco both interface is determined but in iptables just
//output interface could be determined by POSTROUTING and for
//determining input interface PREROUTING should be used
}
//=================================================
void iptables_replace(char *s, char *rp, struct vty *vty)
{
    char str[300];
    //for replace
    //sudo sed -i -- 's/-o ens161/-o ens160/g' iptables.rules
    iptable_save(vty);
    strcpy(str,"sudo sed -i -- 's/-o ");
    strcat(str,s);
    strcat(str,"/-o ");
    strcat(str,rp);
    strcat(str,"/g' iptables.rules \n ");
    // vty_out(vty,str);
    int ret=system(str);
    if (ret==-1){
        vty_out(vty,"Linux Error in replace interface");
    }
    iptable_restore(vty);
}
//====================================================
DEFUN(ip_nat_outside,
        ip_nat_outside_cmd,
"ip nat outside ",
"IP information\n"
"IP NAT information\n"
"Outside interface for address translation\n"
)
{
}
//======================================================
void iptables_command(struct vty * vty)
{
    int ret=system(iptables_input);
    if (ret==-1){
        vty_out(vty,"error");
    }
    clear_var();
    iptable_save(vty);
}
//=========================================
void clear_var()
{
    inside_flag=0;
    outside_flag=0;
    iptables_input[0]='\0';


}
//NOoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
DEFUN(no_ip_nat_pool,
        no_ip_nat_pool_cmd,
"no ip nat pool WORD ",
"No Command\n"
"Internet Protocol\n"
"Network Address Translation\n"
"Define pool of address\n"
"Pool name\n"


)
{
//check in which ip nat it is used before delete
char str[200];
char command[50];
char pool_name;
char line[50];
char line1[50];
FILE *pipe;

//search in command_table
//vty_out(vty,"%s","HI");
pipe = popen("[ -f NAT-PATCommand.txt ] && echo \"Found\" || echo \"Not found\"", "r");
if (pipe == NULL) {
return 1;
}
fgets(line1, 100, pipe);

if(line1=="Found")//if NAT-PATCommand file exist, search for  pool to see if is used in a nat_command
{
strcpy(command,"sudo grep -w ");
strcat(command,argv[4]->arg);
strcat(command," NAT-PATCommand.txt");
pipe = popen(command, "r");

if (pipe == NULL) {  // check for popen error
vty_out(vty,"Error:");
return 1;        // return with exit code indicating error
}// end check for popen error

//IF POOL IS USED SHOW ERROR MESSAGE
if(fgets(line, 100, pipe)!=NULL)
{
vty_out(vty,"Error: This natpool is used in a command, could not delete it!");
return 1;
}

}
/*
  else if(line1=="Not Found")//if NAT-PATCommand file is not created so nat pool is not used anywhere and could deleted
    {
        //now search in
        return 1;
    }
    strcpy(pool_name,argv[4]->arg);
    //check to is pool used in any commands

    //add commands to delete pools
    strcpy(str," sudo sed -i -e '/");
    strcat(str,pool_name);
    strcat(str,"/d' NATPool.txt");
//    vty_out(vty,"%s",str);
    int ret=system(str);
          if (ret==-1){
          vty_out(vty,"error");
          }
    con_pool = con_pool + 1;
*/
}
//==========================================================
DEFUN(no_ip_nat_inside_source_network,
        no_ip_nat_inside_source_network_cmd,
"no ip nat inside source network A.B.C.D/M pool WORD",
NO_STR
"IP information\n"
"IP NAT information\n"
"Inside address translation\n"
"Source address translation\n"
"Network IP describing local addresses for dynamic NAT\n"
"Specify range IP describing local addresses\n"
"Name pool of global addresses\n"
"Pool name of global addresses\n"
)

{
char *pch;
clear_var();
char str[300];
char table_command[300];
char command[40];
char line[100];
int ans=0;
FILE *pipe;
FILE *pipe1;
strcpy(source_ip,argv[6]->arg);
char search_command[100];

//if find command is duplicate
strcpy(search_command,"ip nat inside source network ");
strcat(search_command,argv[6]->arg);
strcat(search_command," pool ");
strcat(search_command,argv[8]->arg);
strcat(search_command," D");

//no need to this part
ans=insert_table(search_command,2,vty);//2 for find and delete
if (ans==1)//command is not defined before do nothing
{
return 1;        /* return with exit code indicating error */
}
if(!file_exist(pool_file,vty))//then search and find the pool
{
vty_out(vty,"File :");
vty_out(vty,pool_file);
vty_out(vty,"pool not exist!  Define a nat pool");
return 1;        /* return with exit code indicating error */
}

//search to find if pool is duplicate
strcpy(command,"sudo grep -w ");
strcat(command,argv[8]->arg);
strcat(command," NATPool.txt");
pipe = popen(command, "r");
//vty_out(vty,command);

if (pipe == NULL) {  // check for popen error
vty_out(vty,"\n Error:");
return 1;        /* return with exit code indicating error */
}// end check for popen error


if(fgets(line, 100, pipe)==NULL)//check to find if pool is defined before?
{
vty_out(vty," Erorr: The pool '");
vty_out(vty,argv[8]->arg);
vty_out(vty,"' is not defined! ");
return 1;
}
pch = strtok (line," ");
pch = strtok (NULL, " ");
strcpy(start_ip,pch);
pch = strtok (NULL, " ");
strcpy(end_ip,pch);

char iface[30];
strcpy(iface,get_o_interface(vty));
pch = strtok (iface, "\n");
//iptables command
strcpy(str,"sudo iptables -t nat -D POSTROUTING -o ");
strcat(str,pch);
strcat(str," -s ");
strcat(str,source_ip);
strcat(str," -j SNAT --to-source ");
strcat(str,start_ip);
strcat(str,"-");
strcat(str,end_ip);
//vty_out(vty,"%s", str);

int ret=system(str);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}

//strcpy(str," sudo sed -i -e '/");"ip nat inside source network 61.214.141.181/21"
strcpy(str," sudo grep -v \"");
strcat(str,search_command);
strcat(str,"\"  NAT-PATCommand.txt > tempfile.txt \n cat tempfile.txt > NAT-PATCommand.txt");
//vty_out(vty,"%s", str);
ret=system(str);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}
iptable_save(vty);
}
//==========================================================
DEFUN(no_ip_nat_inside_source_network_overload ,
        no_ip_nat_inside_source_network_overload_cmd,
"no ip nat inside source network A.B.C.D/M pool WORD overload",
NO_STR
"IP information\n"
"IP NAT information\n"
"Inside address translation\n"
"Source address translation\n"
"Network IP describing local addresses for dynamic NAT\n"
"Specify range IP describing local addresses\n"
"Name pool of global addresses\n"
"Pool name of global addresses\n"
"overload the pool addresses\n"
)

{
clear_var();
char iface[30];
char str[300];
char table_command[300];
char command[40];
char line[100];
int ans=0;
FILE *pipe;
FILE *pipe1;
strcpy(source_ip,argv[6]->arg);
char search_command[100];
char *prot[p_num];//overloading just for 2 protocol and one none overload for all other protocols
prot[0] = " -p all ";
prot[1] = " -p udp ";
prot[2] = " -p tcp ";
char *pch;

//if find command is duplicate
strcpy(search_command,"ip nat inside source network ");
strcat(search_command,argv[6]->arg);
strcat(search_command," pool ");
strcat(search_command,argv[8]->arg);
strcat(search_command," overload");

//no need to this part
ans=insert_table(search_command,2,vty);//2 for find and delete
if (ans==1)//command is not defined before do nothing
{
return 1;        /* return with exit code indicating error */
}
if(!file_exist(pool_file,vty))//then search and find the pool
{
vty_out(vty,"File :");
vty_out(vty,pool_file);
vty_out(vty,"pool not exist!  Define a nat pool");
return 1;        /* return with exit code indicating error */
}
//search to find if pool is duplicate
strcpy(command,"sudo grep -w ");
strcat(command,argv[8]->arg);
strcat(command," NATPool.txt");
pipe = popen(command, "r");

if (pipe == NULL) {  // check for popen error
vty_out(vty,"\n Error:");
return 1;        /* return with exit code indicating error */
}// end check for popen error


if(fgets(line, 100, pipe)==NULL)//check to find if pool is defined before?
{
vty_out(vty," Erorr: The pool '");
vty_out(vty,argv[8]->arg);
vty_out(vty,"' is not defined! ");
return 1;
}
pch = strtok (line," ");
pch = strtok (NULL, " ");
strcpy(start_ip,pch);
pch = strtok (NULL, " ");
pch = strtok (pch, "\n");
strcpy(end_ip,pch);
strcpy(iface,get_o_interface(vty));
pch = strtok (iface, "\n");
//iptables command
int i;
for( i=0; i<p_num ;i++)
{
strcpy(str,"sudo iptables -t nat -D POSTROUTING -o ");
strcat(str,iface);
strcat(str,prot[i]);
strcat(str," -s ");
strcat(str,source_ip);
strcat(str," -j SNAT --to-source ");
strcat(str,start_ip);
strcat(str,"-");
strcat(str,end_ip);
if(i)// could not add for all protocols by -p all and should name each protocolas
{
strcat(str,":2000-60000");
}
strcat(str,"\n");
//        vty_out(vty,"%s", str);
//vty_out(vty,"%s", str);
}
int ret=system(str);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}
//strcpy(str," sudo sed -i -e '/");"ip nat inside source network 61.214.141.181/21"
strcpy(str," sudo grep -v \"");
strcat(str,search_command);
//strcat(str,"\" | grep -v \"overload\"  NAT-PATCommand.txt > NAT-PATCommand.txt");
strcat(str,"\"  NAT-PATCommand.txt > tempfile.txt \n cat tempfile.txt > NAT-PATCommand.txt");
//vty_out(vty,"%s", str);
ret=system(str);
if (ret==-1){
vty_out(vty,"Linux error in iptable command!");
return 1;
}

}
//============================================================
DEFUN(no_ip_nat_inside_source_static,
        no_ip_nat_inside_source_static_cmd,
"no ip nat inside source static A.B.C.D A.B.C.D ",
NO_STR
"IP information\n"
"IP NAT information\n"
"Inside address translation\n"
"Source address translation\n"
"Specify static local->global mapping\n"
"Inside local IP address\n"
"Inside global IP address\n")
{
clear_var();
char iface[30];
char str[300];
strcpy(iface,get_o_interface(vty));
strcpy(str,"sudo iptables -t nat -D POSTROUTING  -o ");
strcat(str,iface);
strcat(str," -s ");
strcpy(loc_ip,argv[6]->arg);
strcpy(glo_ip,argv[7]->arg);
strcat(str,loc_ip);
strcat(str," -j SNAT --to-source ");
strcat(str,glo_ip);
//	vty_out(vty,"%s\n",str);
int ret=system(str);
if (ret==-1){
vty_out(vty,"error");
}
iptable_save(vty);
}
//==========================================================================
DEFUN(no_ip_nat_inside,
        no_ip_nat_inside_cmd,
"no ip nat inside ",
NO_STR
"IP information\n"
"IP NAT information\n"
"Inside interface for address translation\n"

)
{
//VTY_DECLVAR_CONTEXT(interface, ifp);
//Problem:
// In cisco both interface is determined but in iptables just
//output interface could be determined by POSTROUTING and for
//determining input interface PREROUTING should be used
}
//====================================================
DEFUN(no_ip_nat_outside,
        no_ip_nat_outside_cmd,
"no ip nat outside ",
NO_STR
"IP information\n"
"IP NAT information\n"
"Outside interface for address translation\n"
)
{
}
/////////////////////////////////////////////////////////


DEFUN (vtysh_aaa_new_model,
        vtysh_aaa_new_model_cmd,
"aaa new-model",
"Authentication, Authorization and Accounting.\n"
"Enable NEW access control commands and functions.(Disables OLD commands.)\n")
{
char str1[300];
num_aaa_radius++;
#ifdef HAVE_TACACS
num_aaa_tacacs++;
#endif
#ifdef HAVE_KERBEROS
num_aaa_kerberos++;
#endif
strcpy(str1,"aaa new-model");
write_cofigure_radius(str1);
return CMD_SUCCESS;
}
DEFUN (vtysh_radius_server_host,
        vtysh_radius_server_host_cmd,
"radius-server host  <HOSTNAME | A.B.C.D> auth-port (0-65535) acct-port (0-65535) timeout (1-1000) key LINE",
"Modify RADIUS query parameters\n"
"Specify a RADIUS server\n"
"Hostname of RADIUS server\n"
"IP address of RADIUS server\n"
"UDP port for RADIUS authentication sever (default is 1645)\n"
" Port number\n"
"UDP port for RADIUS authentication sever (default is 1645)\n"
" Port number\n"
"Time to wait for this RADIUS server to reply (overrides default)\n"
" Timeout value in seconds to wait for server to reply\n"
"per-server encryption key (overrides default)\n "
"The UNENCRYPTED (cleartext) server key\n")
{
if(num_aaa_radius>=1){
insert_aaa(argv[2]->arg,argv[4]->arg,argv[6]->arg,argv[8]->arg,argv[10]->arg);
}
return CMD_SUCCESS;
}
DEFUN (vtysh_aaa_authentication,
        vtysh_aaa_authentication_cmd,
"aaa authentication login default radius local",
"Authentication, Authorization and Accounting.\n"
"Authentication configurations parameters.\n"
"Set authentication lists for logins.\n"
"The default authentication list.\n"
"Use list of all Radius hosts.\n"
"Use local username authentication.\n"
)
{
char str1[300];
char str[200];
char str2[200];
num_auth_radius++;
if(num_auth_radius==1 && num_aaa_radius>=1){
sprintf(str2,"sudo sed '2 a auth  sufficient  pam_radius_auth.so ' /etc/pam.d/sshd > /etc/pam.d/sshd_tmp ");
int a1=system(str2);
if(a1==-1){
vty_out(vty,"error");
}
system("sudo cp /etc/pam.d/sshd_tmp /etc/pam.d/sshd");
system("sudo service ssh restart");
strcpy(str1,"aaa authentication login default radius local");
write_cofigure_radius(str1);
}
return CMD_SUCCESS;
}

DEFUN (vtysh_aaa_authorization,
        vtysh_aaa_authorization_cmd,
"aaa authorization exe default radius local",
"Authentication, Authorization and Accounting.\n"
"Authorization configurations parameters.\n"
"For starting an exec (shell).\n"
"The default authentication list.\n"
"Use list of all Radius hosts.\n"
"Use local username authentication.\n"
)
{
char str1[300];
char str[200];
num_author_radius++;
if(num_author_radius==1 && num_aaa_radius>=1){
system("sudo chmod 777 /etc/pam.d/sshd_tmp");
sprintf(str,"sudo sed '3 a account sufficient pam_radius_auth.so' /etc/pam.d/sshd > /etc/pam.d/sshd_tmp ");
int a=system(str);
if(a==-1){
vty_out(vty,"error");
}
system("sudo cp /etc/pam.d/sshd_tmp /etc/pam.d/sshd");
system("sudo service ssh restart");
strcpy(str1,"aaa authorization exe default radius local");
write_cofigure_radius(str1);
}
return CMD_SUCCESS;
}
DEFUN (vtysh_aaa_accounting,
        vtysh_aaa_accounting_cmd,
"aaa accounting system default start-stop radius local ",
"Authentication, Authorization and Accounting.\n"
"Accounting configurations parameters.\n"
"For system events.\n"
"The default accounting list.\n"
"Record start and stop without waiting\n"
"Use list of all Radius hosts.\n"
"Use local username authentication.\n"
)
{
char str1[300];
char str[200];
num_acct_radius++;
if(num_acct_radius ==1 && num_aaa_radius>=1){
system("sudo chmod 777 /etc/pam.d/sshd_tmp");
sprintf(str,"sudo sed '4 a session  sufficient  pam_radius_auth.so' /etc/pam.d/sshd > /etc/pam.d/sshd_tmp ");
int a=system(str);
if(a==-1){
vty_out(vty,"error");
}
system("sudo cp /etc/pam.d/sshd_tmp /etc/pam.d/sshd");
system("sudo service ssh restart");
strcpy(str1,"aaa accounting system default start-stop group radius local");
write_cofigure_radius(str1);
}
return CMD_SUCCESS;
}

DEFUN (vtysh_no_radius_server_host,
        vtysh_no_radius_server_host_cmd,
"no radius-server host <HOSTNAME | A.B.C.D>",
NO_STR
"Modify RADIUS query parameters\n"
"Specify a RADIUS server\n"
"Hostname of RADIUS server\n"
"IP address of RADIUS server\n"
)
{
num_aaa_radius--;
no_radius(argv[3]->arg);
return CMD_SUCCESS;
}
DEFUN (vtysh_show_radius_server,
        vtysh_show_radius_server_cmd,
"show radius-server",
SHOW_STR
"Shows radius-server information\n"
)
{
show_radius(vty);
return CMD_SUCCESS;
}
DEFUN (vtysh_no_aaa_new_model,
        vtysh_no_aaa_new_model_cmd,
" no aaa new-model",
NO_STR
"Authentication, Authorization and Accounting.\n"
"Enable NEW access control commands and functions.(Disables OLD commands.)\n")
{
char str1[300];
num_aaa_radius=0;
strcpy(str1,"no aaa new-model");
write_cofigure_no_radius(str1);
system("sudo  sed -i '1,$d'  /etc/raddb/log_server/server_log_show_run");
system("sudo cp /etc/pam.d/sshd.bak  /etc/pam.d/sshd");
system("sudo service ssh restart");
num_auth_radius=0;
num_author_radius=0;
num_acct_radius=0;
#ifdef HAVE_TACACS
num_aaa_tacacs =0;
num_auth_tacacs=0;
num_author_tacacs=0;
num_acct_tacacs=0;
system("sudo cp /etc/pam.d/tacacs.bak /etc/pam.d/tacacs");
system("sudo  sed -i '1,$d'  /usr/local/rahyab/show_tacacs_server");
system("sudo  sed -i '1,$d'  /usr/local/rahyab/show_tacacs_server_run");
system("sudo cp /etc/pam.d/common-auth.bak  /etc/pam.d/common-auth");
#endif
#ifdef HAVE_KERBEROS
num_aaa_kerberos=0;
num_auth_telnet_kerberos=0;
num_auth_kerberos=0;
realm_domain[0] ='\0';
realm_domain1[0] = '\0';
enable_execute_insert=0;
enable_write_show=0;
clear_kerberos_creds_realm();
#endif
return CMD_SUCCESS;
}//o.zeynalpour

//r.mirzaei/port_security:
DEFUN (show_l2fib,
        show_l2fib_cmd,
"show l2fib",
SHOW_STR
"Displays layer 2 fib for the switch\n")
{
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

pipe = popen("sudo vppctl show l2fib verbose","r");
if (pipe == NULL) {  /* check for errors */
// perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}

return 1;
}

DEFUN (feature_port_security,
        feature_port_security_cmd,
"feature port-security",
"Set feature\n"
"Enable port security settings for the switch\n")
{
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

pipe = popen("sudo vppctl create bridge-domain 2","r");
if (pipe == NULL) {  /* check for errors */
// perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}


feature_p_sec = 1;

for(int i=0;i<psec_n;i++)
{
strcpy(psec[i].limit,"4194304");
strcpy(psec[i].flag_s,"0");
}

return 1;
}

DEFUN (switchport_port_security,
        switchport_port_security_cmd,
"switchport port-security",
"Set switching mode characteristics\n"
"Enable port security settings for the interface\n")
{

if(feature_p_sec == 0)
{
vty_out(vty,"port_security feature is disabled on switch!");
return 1;
}

int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
char line2[LINE_BUFSIZE];
int linenr;
int linenr2;
FILE *pipe;
FILE *pipe2;

//char psec_int[100];
//strcpy(psec_int,"GigabitEthernet13/0/0");

char *psec_int;
VTY_DECLVAR_CONTEXT(interface, ifp);
psec_int= nic_names_get_vpp_from_tap(ifp->name);
if(psec_int==NULL){	
	vty_out(vty,"no such interface");
	return CMD_WARNING;
}

int ff=-1;

for (int i=0; i<psec_i; i++)
{
if (strcmp(psec[i].interfce,psec_int) == 0)
{
ff=i;
break;
}
}

if(ff == -1)
{
strcpy(psec[psec_i].interfce,psec_int);
ff = psec_i;
psec_i++;
}

char c_rose[200];
strcpy(c_rose,"sudo vppctl set interface l2 bridge ");
strcat(c_rose,psec_int);
strcat(c_rose," 2");

pipe = popen(c_rose,"r");
if (pipe == NULL) {  /* check for errors */
// perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}

psec[ff].p_sec_set = 1;

return 1;
}


DEFUN (switchport_port_security_limit,
        switchport_port_security_limit_cmd,
"switchport port-security maximum (0-4194304)",
"Set switching mode characteristics\n"
"Enable port security settings for the interface\n"
"Maximum number for learnt MACs\n"
"A number between 0 to 4194304 (2^22) (default to 4194304)\n")
{
//int LINE_BUFSIZE = 100;
//char line[LINE_BUFSIZE];
//char line2[LINE_BUFSIZE];
//int linenr;
//int linenr2;
//FILE *pipe;
//FILE *pipe2;

//char psec_int[100];
//strcpy(psec_int,"GigabitEthernet13/0/0");

char *psec_int;
VTY_DECLVAR_CONTEXT(interface, ifp);
psec_int= nic_names_get_vpp_from_tap(ifp->name);
if(psec_int==NULL){	
	vty_out(vty,"no such interface");
	return CMD_WARNING;
}
int ff=-1;

for (int i=0; i<psec_i; i++)
{
if (strcmp(psec[i].interfce,psec_int) == 0)
{
ff=i;
break;
}
}

if(ff == -1)
{
strcpy(psec[psec_i].interfce,psec_int);
ff = psec_i;
psec_i++;
}

/*char c_rose[200];
    strcpy(c_rose,"sudo vppctl set interface l2 bridge ");
    strcat(c_rose,psec_int);
    strcat(c_rose," 2");*/

/*pipe = popen(c_rose,"r");
     if (pipe == NULL) {  // check for errors
        // perror(argv[0]); // report error message
         return 1;        // return with exit code indicating error
     }

     while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
         vty_out(vty,"%s", line);
         line[strlen(line)-1]='\0';
         ++linenr;
     }*/

strcpy(psec[ff].limit,argv[3]->arg);

/*char c_rose2[200];
	strcpy(c_rose2,"sudo vppctl set interface l2 sticky static ");
	strcat(c_rose2,"00:0c:29:3a:39:e1");
	strcat(c_rose2," ");
	strcat(c_rose2,psec_int);
	strcat(c_rose2," 1 ");
	strcat(c_rose2,psec[ff].limit);
	strcat(c_rose2," 0");

	vty_out(vty,"%s",c_rose2);*/

/*pipe2 = popen(c_rose2,"r");
	 if (pipe2 == NULL) {  // check for errors
		// perror(argv[0]); // report error message
		 return 1;        // return with exit code indicating error
	 }

	 while (fgets(line2, LINE_BUFSIZE, pipe2) != NULL) {
		 vty_out(vty,"%s", line2);
		 line2[strlen(line2)-1]='\0';
		 ++linenr2;
	 }*/

return 1;
}

DEFUN (switchport_port_security_violation,
        switchport_port_security_violation_cmd,
"switchport port-security violation WORD",
"Set switching mode characteristics\n"
"Enable port security settings for the interface\n"
"Action type\n"
"restrict or shutdown\n")
{
//int LINE_BUFSIZE = 100;
//char line[LINE_BUFSIZE];
//char line2[LINE_BUFSIZE];
//int linenr;
//int linenr2;
//FILE *pipe;
//FILE *pipe2;

//char psec_int[100];
//strcpy(psec_int,"GigabitEthernet13/0/0");

char *psec_int;
VTY_DECLVAR_CONTEXT(interface, ifp);
psec_int= nic_names_get_vpp_from_tap(ifp->name);
if(psec_int==NULL){	
	vty_out(vty,"no such interface");
	return CMD_WARNING;
}
int ff=-1;

for (int i=0; i<psec_i; i++)
{
if (strcmp(psec[i].interfce,psec_int) == 0)
{
ff=i;
break;
}
}

if(ff == -1)
{
strcpy(psec[psec_i].interfce,psec_int);
ff = psec_i;
psec_i++;
}

/*char c_rose[200];
    strcpy(c_rose,"sudo vppctl set interface l2 bridge ");
    strcat(c_rose,psec_int);
    strcat(c_rose," 2");*/

/*pipe = popen(c_rose,"r");
     if (pipe == NULL) {  // check for errors
        // perror(argv[0]); // report error message
         return 1;        // return with exit code indicating error
     }

     while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
         vty_out(vty,"%s", line);
         line[strlen(line)-1]='\0';
         ++linenr;
     }*/

if(strcmp(argv[3]->arg,"shutdown") == 0)
{
strcpy(psec[ff].flag_s,"1");
}
else
{
strcpy(psec[ff].flag_s,"0");
}

/*char c_rose2[200];
	strcpy(c_rose2,"sudo vppctl set interface l2 sticky static ");
	strcat(c_rose2,"00:0c:29:3a:39:e1");
	strcat(c_rose2," ");
	strcat(c_rose2,psec_int);
	strcat(c_rose2," 1 ");
	strcat(c_rose2,psec[ff].limit);
	strcat(c_rose2," ");
	strcat(c_rose2,psec[ff].flag_s);

	vty_out(vty,"%s",c_rose2);*/

/*pipe2 = popen(c_rose2,"r");
	 if (pipe2 == NULL) {  // check for errors
		// perror(argv[0]); // report error message
		 return 1;        // return with exit code indicating error
	 }

	 while (fgets(line2, LINE_BUFSIZE, pipe2) != NULL) {
		 vty_out(vty,"%s", line2);
		 line2[strlen(line2)-1]='\0';
		 ++linenr2;
	 }*/

return 1;
}


DEFUN (switchport_port_security_mac_address_sticky,
        switchport_port_security_mac_address_sticky_cmd,
"switchport port-security mac-address sticky",
"Set switching mode characteristics\n"
"Enable port security settings for the interface\n"
"Enable MAC address security mode\n"
"Enable sticky learning on the interface\n")
{

if(feature_p_sec == 0)
{
vty_out(vty,"port_security feature is disabled on switch!");
return 1;
}

int LINE_BUFSIZE = 100;
char line2[LINE_BUFSIZE];
char line3[LINE_BUFSIZE];
char line4[LINE_BUFSIZE];
int linenr2;
int linenr3;
int linenr4;
FILE *pipe2;
FILE *pipe3;
FILE *pipe4;

//char psec_int[100];
//strcpy(psec_int,"GigabitEthernet13/0/0");

char *psec_int;
VTY_DECLVAR_CONTEXT(interface, ifp);
psec_int= nic_names_get_vpp_from_tap(ifp->name);
if(psec_int==NULL){	
	vty_out(vty,"no such interface");
	return CMD_WARNING;
}
int ff=-1;

for (int i=0; i<psec_i; i++)
{
if (strcmp(psec[i].interfce,psec_int) == 0)
{
ff=i;
break;
}
}

if(ff == -1)
{
strcpy(psec[psec_i].interfce,psec_int);
ff = psec_i;
psec_i++;
}

if (psec[ff].p_sec_set == 0)
{
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;
char c_rose[200];

strcpy(c_rose,"sudo vppctl set interface l2 bridge ");
strcat(c_rose,psec_int);
strcat(c_rose," 2");

pipe = popen(c_rose,"r");
if (pipe == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
}

if(psec[ff].sticky_set == 1)
{
char c_rose3[200];
strcpy(c_rose3,"sudo vppctl no interface l2 sticky learn ");
strcat(c_rose3,psec_int);

pipe3 = popen(c_rose3,"r");
if (pipe3 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line3, LINE_BUFSIZE, pipe3) != NULL) {
vty_out(vty,"%s", line3);
line3[strlen(line3)-1]='\0';
++linenr3;
}
}

char c_rose4[200];
strcpy(c_rose4,"sudo vppctl clear l2fib");

char c_rose2[200];
strcpy(c_rose2,"sudo vppctl set interface l2 sticky learn ");
strcat(c_rose2,psec_int);
strcat(c_rose2," ");
strcat(c_rose2,psec[ff].limit);
strcat(c_rose2," ");
strcat(c_rose2,psec[ff].flag_s);

//vty_out(vty,"%s",c_rose4);

pipe4 = popen(c_rose4,"r");
if (pipe4 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line4, LINE_BUFSIZE, pipe4) != NULL) {
vty_out(vty,"%s", line4);
line4[strlen(line4)-1]='\0';
++linenr4;
}

//vty_out(vty,"%s",c_rose2);

pipe2 = popen(c_rose2,"r");
if (pipe2 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line2, LINE_BUFSIZE, pipe2) != NULL) {
vty_out(vty,"%s", line2);
line2[strlen(line2)-1]='\0';
++linenr2;
}

psec[ff].sticky_set = 1;

return 1;
}

DEFUN (switchport_port_security_mac_address,
        switchport_port_security_mac_address_cmd,
"switchport port-security mac-address WORD",
"Set switching mode characteristics\n"
"Enable port security settings for the interface\n"
"Enable MAC address security mode\n"
"Static MAC address\n")
{
if(feature_p_sec == 0)
{
vty_out(vty,"port_security feature is disabled on switch!");
return 1;
}

int LINE_BUFSIZE = 100;
char line2[LINE_BUFSIZE];
char line3[LINE_BUFSIZE];
char line4[LINE_BUFSIZE];
int linenr2;
int linenr3;
int linenr4;
FILE *pipe2;
FILE *pipe3;
FILE *pipe4;

//char psec_int[100];
//strcpy(psec_int,"GigabitEthernet13/0/0");

char *psec_int;
VTY_DECLVAR_CONTEXT(interface, ifp);
psec_int= nic_names_get_vpp_from_tap(ifp->name);
if(psec_int==NULL){	
	vty_out(vty,"no such interface");
	return CMD_WARNING;
}
int ff=-1;

for (int i=0; i<psec_i; i++)
{
if (strcmp(psec[i].interfce,psec_int) == 0)
{
ff=i;
break;
}
}

if(ff == -1)
{
strcpy(psec[psec_i].interfce,psec_int);
ff = psec_i;
psec_i++;
}

if (psec[ff].p_sec_set == 0)
{
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;
char c_rose[200];

strcpy(c_rose,"sudo vppctl set interface l2 bridge ");
strcat(c_rose,psec_int);
strcat(c_rose," 2");

pipe = popen(c_rose,"r");
if (pipe == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
}

if(psec[ff].mac_set == 1)
{
char c_rose3[200];
strcpy(c_rose3,"sudo vppctl no interface l2 sticky static ");
strcat(c_rose3,psec_int);

//vty_out(vty,"%s",c_rose3);

pipe3 = popen(c_rose3,"r");
if (pipe3 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line3, LINE_BUFSIZE, pipe3) != NULL) {
vty_out(vty,"%s", line3);
line3[strlen(line3)-1]='\0';
++linenr3;
}
}

char c_rose4[200];
strcpy(c_rose4,"sudo vppctl clear l2fib");

char c_rose2[200];
strcpy(c_rose2,"sudo vppctl set interface l2 sticky static ");
strcat(c_rose2,argv[3]->arg);
strcat(c_rose2," ");
strcat(c_rose2,psec_int);
strcat(c_rose2," 1 ");
strcat(c_rose2,psec[ff].limit);
strcat(c_rose2," ");
strcat(c_rose2,psec[ff].flag_s);

//vty_out(vty,"%s",c_rose4);

pipe4 = popen(c_rose4,"r");
if (pipe4 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line4, LINE_BUFSIZE, pipe4) != NULL) {
vty_out(vty,"%s", line4);
line4[strlen(line4)-1]='\0';
++linenr4;
}

//vty_out(vty,"%s",c_rose2);

pipe2 = popen(c_rose2,"r");
if (pipe2 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line2, LINE_BUFSIZE, pipe2) != NULL) {
vty_out(vty,"%s", line2);
line2[strlen(line2)-1]='\0';
++linenr2;
}

psec[ff].mac_set = 1;
strcpy(psec[ff].mac,argv[3]->arg);

return 1;
}

DEFUN (no_switchport_port_security_mac_address_sticky,
        no_switchport_port_security_mac_address_sticky_cmd,
"no switchport port-security mac-address sticky",
NO_STR
"Set switching mode characteristics\n"
"Disable port security settings for the interface\n"
"Disable MAC address security mode\n"
"Disable sticky learning on the interface\n")
{

if(feature_p_sec == 0)
{
vty_out(vty,"port_security feature is disabled on switch!");
return 1;
}

int LINE_BUFSIZE = 100;
char line3[LINE_BUFSIZE];
int linenr3;
FILE *pipe3;
char line4[LINE_BUFSIZE];
int linenr4;
FILE *pipe4;

//char psec_int[100];
//strcpy(psec_int,"GigabitEthernet13/0/0");

char *psec_int;
VTY_DECLVAR_CONTEXT(interface, ifp);
psec_int= nic_names_get_vpp_from_tap(ifp->name);
if(psec_int==NULL){	
	vty_out(vty,"no such interface");
	return CMD_WARNING;
}
int ff=-1;

for (int i=0; i<psec_i; i++)
{
if (strcmp(psec[i].interfce,psec_int) == 0)
{
ff=i;
break;
}
}

if(ff == -1)
{
strcpy(psec[psec_i].interfce,psec_int);
ff = psec_i;
psec_i++;
}

if (psec[ff].p_sec_set == 0)
{
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;
char c_rose[200];

strcpy(c_rose,"sudo vppctl set interface l2 bridge ");
strcat(c_rose,psec_int);
strcat(c_rose," 2");

pipe = popen(c_rose,"r");
if (pipe == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
}

if(psec[ff].sticky_set == 0)
{
vty_out(vty,"port_security sticky feature already is disabled on this interface!");
return 1;
}

char c_rose3[200];
strcpy(c_rose3,"sudo vppctl no interface l2 sticky learn ");
strcat(c_rose3,psec_int);

char c_rose4[200];
strcpy(c_rose4,"sudo vppctl clear l2fib");

//vty_out(vty,"%s",c_rose3);

pipe3 = popen(c_rose3,"r");
if (pipe3 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line3, LINE_BUFSIZE, pipe3) != NULL) {
vty_out(vty,"%s", line3);
line3[strlen(line3)-1]='\0';
++linenr3;
}

//vty_out(vty,"%s",c_rose4);

pipe4 = popen(c_rose4,"r");
if (pipe4 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line4, LINE_BUFSIZE, pipe4) != NULL) {
vty_out(vty,"%s", line4);
line4[strlen(line4)-1]='\0';
++linenr4;
}

psec[ff].sticky_set = 0;

return 1;
}

DEFUN (no_switchport_port_security_mac_address,
        no_switchport_port_security_mac_address_cmd,
"no switchport port-security mac-address",
NO_STR
"Set switching mode characteristics\n"
"Disable port security settings for the interface\n"
"Disable MAC address security mode\n")
{
if(feature_p_sec == 0)
{
vty_out(vty,"port_security feature is disabled on switch!");
return 1;
}

int LINE_BUFSIZE = 100;
char line3[LINE_BUFSIZE];
int linenr3;
FILE *pipe3;
char line4[LINE_BUFSIZE];
int linenr4;
FILE *pipe4;

//char psec_int[100];
//strcpy(psec_int,"GigabitEthernet13/0/0");

char *psec_int;
VTY_DECLVAR_CONTEXT(interface, ifp);
psec_int= nic_names_get_vpp_from_tap(ifp->name);
if(psec_int==NULL){	
vty_out(vty,"no such interface");
	return CMD_WARNING;
}
int ff=-1;

for (int i=0; i<psec_i; i++)
{
if (strcmp(psec[i].interfce,psec_int) == 0)
{
ff=i;
break;
}
}

if(ff == -1)
{
strcpy(psec[psec_i].interfce,psec_int);
ff = psec_i;
psec_i++;
}

if (psec[ff].p_sec_set == 0)
{
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;
char c_rose[200];

strcpy(c_rose,"sudo vppctl set interface l2 bridge ");
strcat(c_rose,psec_int);
strcat(c_rose," 2");

pipe = popen(c_rose,"r");
if (pipe == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
}

if(psec[ff].mac_set == 0)
{
vty_out(vty,"port_security sticky feature already is disabled on this interface!");
return 1;
}

char c_rose3[200];
strcpy(c_rose3,"sudo vppctl no interface l2 sticky static ");
strcat(c_rose3,psec_int);

char c_rose4[200];
strcpy(c_rose4,"sudo vppctl clear l2fib");

//vty_out(vty,"%s",c_rose3);

pipe3 = popen(c_rose3,"r");
if (pipe3 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line3, LINE_BUFSIZE, pipe3) != NULL) {
vty_out(vty,"%s", line3);
line3[strlen(line3)-1]='\0';
++linenr3;
}

//vty_out(vty,"%s",c_rose4);

pipe4 = popen(c_rose4,"r");
if (pipe4 == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line4, LINE_BUFSIZE, pipe4) != NULL) {
vty_out(vty,"%s", line4);
line4[strlen(line4)-1]='\0';
++linenr4;
}

psec[ff].mac_set = 0;

return 1;
}

DEFUN(show_port_security,
        show_port_security_cmd,
"show port-security",
SHOW_STR
"Displays port security settings for the switch\n")
{
if(feature_p_sec == 0)
{
vty_out(vty,"port_security feature is disabled on switch!");
return 1;
}

int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

/*for(int i=0; i<psec_i; i++)
	 {
		char c_rose[200];
		strcpy(c_rose,psec[i].interfce);
		strcat(c_rose,"\t");
		strcat(c_rose,psec[i].limit);
		strcat(c_rose,"\t");
		strcat(c_rose,psec[i].flag_s);

		vty_out(vty,"%s", c_rose);
	 }*/

pipe = popen("sudo vppctl show l2 limit learn","r");
if (pipe == NULL) {  // check for errors
// perror(argv[0]); // report error message
return 1;        // return with exit code indicating error
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}

return 1;
}

//r.mirzaei/ntp:

DEFUN(clock_set,
        clock_set_cmd,
"clock set TIME DATE",
"clock (Time and Date)\n"
"set Time and Date\n"
"TIME hh:mm:ss Hour:Minute:Second(24-hour format)\n"
"Date yyyy-mm-dd Year-Month-Day \n")
{
char s_date[400];
strcpy(s_date,"sudo date --set=\"");

strcat(s_date,argv[3]->arg);
strcat(s_date," ");
strcat(s_date,argv[2]->arg);
strcat(s_date,"\"");
system(s_date);
char s2_date[200];
strcpy(s2_date,"time and date are changed!\t<");
strcat(s2_date,argv[2]->arg);
strcat(s2_date," ");
strcat(s2_date,argv[3]->arg);
strcat(s2_date,">");
//vty_out(vty,"%s\n",s_date);
vty_out(vty,"%s\n",s2_date);
return 1;
}

DEFUN(show_clock,
        show_clock_cmd,
"show clock",
SHOW_STR
"clock (Time and Date)\n")
{
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

pipe = popen("date","r");
if (pipe == NULL) {  /* check for errors */
// perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
return 1;
}

DEFUN(ntp_enable,
        ntp_enable_cmd,
"ntp enable",
"Configure NTP\n"
"Enabling Network Time Protocol\n")
{
system("sudo service ntp restart");
vty_out(vty,"%s","ntp server enabled");
return 1;
}

DEFUN(ntp_disable,
        ntp_disable_cmd,
"ntp disable",
"Configure NTP\n"
"Disabling Network Time Protocol\n")
{
system("sudo service ntp stop");
vty_out(vty,"%s","ntp server disabled");
return 1;
}


DEFUN(show_ntp_associations,
        show_ntp_associations_cmd,
"show ntp associations",
SHOW_STR
"Network Time Protocol\n"
"NTP Associations\n")
{

int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

pipe = popen("sudo ntpq -p","r");
if (pipe == NULL) {  /* check for errors */
// perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
return 1;
}

DEFUN(show_ntp_status,
        show_ntp_status_cmd,
"show ntp status",
SHOW_STR
"Network Time Protocol\n"
"NTP Status\n")
{
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
FILE *pipe;

pipe = popen("sudo ntpq -c kerninfo","r");
if (pipe == NULL) {  /* check for errors */
//   perror(argv[0]); /* report error message */
return 1;        /* return with exit code indicating error */
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
vty_out(vty,"%s", line);
line[strlen(line)-1]='\0';
++linenr;
}
return 1;
}

DEFUN(ntp_server,
        ntp_server_cmd,
"ntp server A.B.C.D",
"Configure NTP\n"
"Configure NTP server\n"
"IP Address of Server\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i '/#Rose71 Server/a server ");
char p_rose[100];
strcpy(p_rose,"' /etc/ntp.conf");

system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[2]->arg);
strcat(c_rose,p_rose);
vty_out(vty,"%s","ntp server added");

char cs2_rose[200];
strcpy(cs2_rose,"sudo sed -i '/#Rose71 Restrict/a restrict ");
char ps2_rose[100];
strcpy(ps2_rose," mask ");
char ns2_rose[100];
strcpy(ns2_rose," nomodify notrap' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs2_rose,argv[2]->arg);
strcat(cs2_rose,ps2_rose);
strcat(cs2_rose,"255.255.255.255");
strcat(cs2_rose,ns2_rose);

system(c_rose);
system(cs2_rose);
system("sudo service ntp restart");
}

DEFUN(no_ntp_server,
        no_ntp_server_cmd,
"no ntp server A.B.C.D",
NO_STR
"Configure NTP\n"
"Configure NTP Server\n"
"IP Address of Server\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i.bak '/server ");
char p_rose[100];
strcpy(p_rose,"/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[3]->arg);
strcat(c_rose,p_rose);

char cs_rose[200];
strcpy(cs_rose,"sudo sed -i.bak '/restrict ");
char ps_rose[100];
strcpy(ps_rose," mask ");
char ns_rose[100];
strcpy(ns_rose," nomodify notrap/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs_rose,argv[3]->arg);
strcat(cs_rose,ps_rose);
strcat(cs_rose,"255.255.255.255");
strcat(cs_rose,ns_rose);


vty_out(vty,"%s","ntp server removed");
system(c_rose);
system(cs_rose);
system("sudo service ntp restart");
}

DEFUN(ntp_server_hostname,
        ntp_server_hostname_cmd,
"ntp server HOSTNAME",
"Configure NTP\n"
"Configure NTP server\n"
"Host Name of Server\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i '/#Rose71 Server/a server ");
char p_rose[100];
strcpy(p_rose,"' /etc/ntp.conf");

system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[2]->arg);
strcat(c_rose,p_rose);
vty_out(vty,"%s","ntp server added");

char cs2_rose[200];
strcpy(cs2_rose,"sudo sed -i '/#Rose71 Restrict/a restrict ");
char ns2_rose[100];
strcpy(ns2_rose," nomodify notrap' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs2_rose,argv[2]->arg);
strcat(cs2_rose,ns2_rose);

system(c_rose);
system(cs2_rose);
system("sudo service ntp restart");
}

DEFUN(no_ntp_server_hostname,
        no_ntp_server_hostname_cmd,
"no ntp server HOSTNAME",
NO_STR
"Configure NTP\n"
"Configure NTP Server\n"
"Host Name of Server\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i.bak '/server ");
char p_rose[100];
strcpy(p_rose,"/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[3]->arg);
strcat(c_rose,p_rose);

char cs_rose[200];
strcpy(cs_rose,"sudo sed -i.bak '/restrict ");
char ns_rose[100];
strcpy(ns_rose," nomodify notrap/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs_rose,argv[3]->arg);
strcat(cs_rose,ns_rose);


vty_out(vty,"%s","ntp server removed");
system(c_rose);
system(cs_rose);
system("sudo service ntp restart");
}

DEFUN(ntp_server_iburst,
        ntp_server_iburst_cmd,
"ntp server A.B.C.D iburst",
"Configure NTP\n"
"Configure NTP server\n"
"IP Address of Server\n"
"NTP Iburst Mode for Faster Clock Synchronization\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i '/#Rose71 Server/a server ");
char p_rose[100];
strcpy(p_rose," iburst' /etc/ntp.conf");

system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[2]->arg);
strcat(c_rose,p_rose);
vty_out(vty,"%s","ntp server added");

char cs2_rose[200];
strcpy(cs2_rose,"sudo sed -i '/#Rose71 Restrict/a restrict ");
char ps2_rose[100];
strcpy(ps2_rose," mask ");
char ns2_rose[100];
strcpy(ns2_rose," nomodify notrap' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs2_rose,argv[2]->arg);
strcat(cs2_rose,ps2_rose);
strcat(cs2_rose,"255.255.255.255");
strcat(cs2_rose,ns2_rose);

system(c_rose);
system(cs2_rose);
system("sudo service ntp restart");
}

DEFUN(no_ntp_server_iburst,
        no_ntp_server_iburst_cmd,
"no ntp server A.B.C.D iburst",
NO_STR
"Configure NTP\n"
"Configure NTP Server\n"
"IP Address of Server\n"
"NTP Iburst Mode for Faster Clock Synchronization\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i.bak '/server ");
char p_rose[100];
strcpy(p_rose," iburst/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[3]->arg);
strcat(c_rose,p_rose);

char cs_rose[200];
strcpy(cs_rose,"sudo sed -i.bak '/restrict ");
char ps_rose[100];
strcpy(ps_rose," mask ");
char ns_rose[100];
strcpy(ns_rose," nomodify notrap/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs_rose,argv[3]->arg);
strcat(cs_rose,ps_rose);
strcat(cs_rose,"255.255.255.255");
strcat(cs_rose,ns_rose);


vty_out(vty,"%s","ntp server removed");
system(c_rose);
system(cs_rose);
system("sudo service ntp restart");
}

DEFUN(ntp_server_hostname_iburst,
        ntp_server_hostname_iburst_cmd,
"ntp server HOSTNAME iburst",
"Configure NTP\n"
"Configure NTP server\n"
"Host Name of Server\n"
"NTP Iburst Mode for Faster Clock Synchronization\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i '/#Rose71 Server/a server ");
char p_rose[100];
strcpy(p_rose," iburst' /etc/ntp.conf");

system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[2]->arg);
strcat(c_rose,p_rose);
vty_out(vty,"%s","ntp server added");

char cs2_rose[200];
strcpy(cs2_rose,"sudo sed -i '/#Rose71 Restrict/a restrict ");
char ns2_rose[100];
strcpy(ns2_rose," nomodify notrap' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs2_rose,argv[2]->arg);
strcat(cs2_rose,ns2_rose);

system(c_rose);
system(cs2_rose);
system("sudo service ntp restart");
}

DEFUN(no_ntp_server_hostname_iburst,
        no_ntp_server_hostname_iburst_cmd,
"no ntp server HOSTNAME iburst",
NO_STR
"Configure NTP\n"
"Configure NTP Server\n"
"Host Name of Server\n"
"NTP Iburst Mode for Faster Clock Synchronization\n")
{
char c_rose[200];
strcpy(c_rose,"sudo sed -i.bak '/server ");
char p_rose[100];
strcpy(p_rose," iburst/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(c_rose,argv[3]->arg);
strcat(c_rose,p_rose);

char cs_rose[200];
strcpy(cs_rose,"sudo sed -i.bak '/restrict ");
char ns_rose[100];
strcpy(ns_rose," nomodify notrap/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(cs_rose,argv[3]->arg);
strcat(cs_rose,ns_rose);


vty_out(vty,"%s","ntp server removed");
system(c_rose);
system(cs_rose);
system("sudo service ntp restart");
}


DEFUN(ntp_master,
        ntp_master_cmd,
"ntp master A.B.C.D A.B.C.D",
"Configure NTP\n"
"Configure NTP Master\n"
"IP Address of Network\n"
"Subnet Mask of Network\n")
{
//char L_rose[200];
//char L2_rose[200];
//strcpy(L_rose,"sudo sed -i '/#Rose71 Local/a fudge 127.127.1.0 stratum 10' /etc/ntp.conf");
//strcpy(L2_rose,"sudo sed -i '/#Rose71 Local/a server 127.127.1.0 prefer' /etc/ntp.conf");
//strcpy(L2_rose,"sudo sed -i '/#Rose71 Local/a server 127.127.1.0' /etc/ntp.conf");

char c2_rose[200];
strcpy(c2_rose,"sudo sed -i '/#Rose71 Restrict/a restrict ");
char p2_rose[100];
strcpy(p2_rose," mask ");
char n2_rose[100];
strcpy(n2_rose," nomodify notrap' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(c2_rose,argv[2]->arg);
strcat(c2_rose,p2_rose);
strcat(c2_rose,argv[3]->arg);
strcat(c2_rose,n2_rose);
vty_out(vty,"%s","master ntp server enabled");

/*FILE *fp;
char ch;
int x=0;
char txt[100];
int len;
fp=fopen("/etc/ntp.conf","r");

ch=fgetc(fp);
while(ch != EOF)
{
  if(ch=='f')
  {
    ch=fgetc(fp);
    if(ch=='u')
    {
      ch=fgetc(fp);
      if(ch=='d')
      {
        ch=fgetc(fp);
        if(ch=='g')
        {
          ch=fgetc(fp);
          if(ch=='e')
	  {
	    x=1;
	    break;
	  }
	}
      }
    }
  }

  else
  {
    ch=fgetc(fp);
  }
}

if(x==0)
{
    system(L_rose);
    system(L2_rose);
}*/
system(c2_rose);
system("sudo service ntp restart");
}

DEFUN(no_ntp_master,
        no_ntp_master_cmd,
"no ntp master A.B.C.D A.B.C.D",
NO_STR
"Configure NTP\n"
"Configure NTP Master\n"
"IP Address of Network\n"
"Subnet Mask of Network\n")
{

char c3_rose[200];
strcpy(c3_rose,"sudo sed -i.bak '/restrict ");
char p3_rose[100];
strcpy(p3_rose," mask ");
char n3_rose[100];
strcpy(n3_rose," nomodify notrap/d' /etc/ntp.conf");
system("sudo chmod 777 /etc/ntp.conf");
strcat(c3_rose,argv[3]->arg);
strcat(c3_rose,p3_rose);
strcat(c3_rose,argv[4]->arg);
strcat(c3_rose,n3_rose);

vty_out(vty,"%s","master ntp server disabled");
system(c3_rose);
system("sudo service ntp restart");
}

DEFUN (switchport,
        switchport_cmd,
"switchport",
"Configure the interface as a switch port\n")
{
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
struct access_list *access;
struct access_master *master;
struct filter *mfilter;
struct filter_cisco *filter;
int write = 0;
char str[400];
char interface_name[100];
FILE *pipe;

VTY_DECLVAR_CONTEXT(interface, ifp);
sprintf(str,"vppctl \"show tap-inject\" | grep \"%s\" | awk \'{print $1}\'",ifp->name);
vty_out(vty,"%s\n",str);
pipe = popen(str, "r");
if (pipe == NULL) {
return 1;
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
line[strlen(line)-1]='\0';
vty_out(vty,"%s", line);
strcpy(interface_name,line);
++linenr;
}

if(strcmp(interface_name,"")==0){
vty_out(vty,"no such interface\n");
return CMD_WARNING;
}


sprintf(str,"ifconfig %s 0",ifp->name);
pipe_cmd(vty,str);
sprintf(str,"sudo vppctl set interface l2 bridge %s 1", interface_name);
pipe_cmd(vty,str);

return CMD_SUCCESS;
}

DEFUN (no_switchport,
        no_switchport_cmd,
"no switchport",
NO_STR
"Configure the interface as a routing port\n")
{
int LINE_BUFSIZE = 100;
char line[LINE_BUFSIZE];
int linenr;
struct access_list *access;
struct access_master *master;
struct filter *mfilter;
struct filter_cisco *filter;
int write = 0;
char str[400];
char interface_name[100];
FILE *pipe;

VTY_DECLVAR_CONTEXT(interface, ifp);
sprintf(str,"vppctl \"show tap-inject\" | grep \"%s\" | awk \'{print $1}\'",ifp->name);
vty_out(vty,"%s\n",str);
pipe = popen(str, "r");
if (pipe == NULL) {
return 1;
}

while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
line[strlen(line)-1]='\0';
vty_out(vty,"%s", line);
strcpy(interface_name,line);
++linenr;
}

if(strcmp(interface_name,"")==0){
vty_out(vty,"no such interface\n");
return CMD_WARNING;
}

sprintf(str,"sudo vppctl set interface l3 %s", interface_name);
pipe_cmd(vty,str);
return CMD_SUCCESS;
}
/*
DEFUN (ip_dhcp_snooping_trust,
        ip_dhcp_snooping_trust_cmd,
        "ip dhcp snooping trust",
        "Configure the interface as a routing port\n")
{
	struct access_list *access;
//	access = access_list_lookup(AFI_L2VPN,argv[2]->arg);
	struct filter_cisco ccc;
	struct filter *filter;
	int LINE_BUFSIZE = 100;
    char line[LINE_BUFSIZE];
    int linenr;
    FILE *pipe;
	char interface_name[100];
	char str[400];


	VTY_DECLVAR_CONTEXT(interface, ifp);
	sprintf(str,"vppctl \"show tap-inject\" | grep \"%s\" | awk \'{print $1}\'",ifp->name);

	pipe = popen(str, "r");
	if (pipe == NULL) {
	 return 1;
	}

	while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
		 line[strlen(line)-1]='\0';
		 //vty_out(vty,"%s", line);
		strcpy(interface_name,line);
		 ++linenr;
	}
	sprintf(str,"sudo -E LD_LIBRARY_PATH=/vpp/build-root/install-vpp_debug-native/vpp/lib64/libvppapiclient.so python /root/L2/ACL_MACIP_ADD.py ");
	sprintf(str,"%s 0.0.0.0/0 67 67 0.0.0.0/0 0 0 0 deny snooping",str);
	system(str);
	vty_out(vty, "%s\n",str);

	sprintf(str,"python /root/L3/ACL_Show_Table.py | tail -n +2 | grep snooping | awk '{print $1}' | cut -d '_' -f2");
	pipe = popen(str, "r");
	if (pipe == NULL) {
	 return 1;
	}

	while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
	 line[strlen(line)-1]='\0';
	 //vty_out(vty,"%s", line);
	 sprintf(str,"python /root/L2/ACL_MACIP_Apply_Interface.py %s %s",interface_name,line);
	 vty_out(vty,str);
	 pipe_cmd(vty,str);
	 ++linenr;
	}
}

DEFUN (no_ip_dhcp_snooping_trust,
       no_ip_dhcp_snooping_trust_cmd,
        "no ip dhcp snooping trust",
        NO_STR
        "Configure the interface as a routing port\n")
{
	struct access_list *access;
//	access = access_list_lookup(AFI_L2VPN,argv[2]->arg);
	struct filter_cisco ccc;
	struct filter *filter;
	int LINE_BUFSIZE = 100;
    char line[LINE_BUFSIZE];
    int linenr;
    FILE *pipe;
	char interface_name[100];
	char str[400];

	VTY_DECLVAR_CONTEXT(interface, ifp);
	sprintf(str,"vppctl \"show tap-inject\" | grep \"%s\" | awk \'{print $1}\'",ifp->name);

	pipe = popen(str, "r");
	if (pipe == NULL) {
	 return 1;
	}

	while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
		 line[strlen(line)-1]='\0';
		 //vty_out(vty,"%s", line);
		strcpy(interface_name,line);
		 ++linenr;
	}

	sprintf(str,"python /root/L3/ACL_Show_Table.py | tail -n +2 | grep snooping | awk '{print $1}' | cut -d '_' -f2");
	pipe = popen(str, "r");
	if (pipe == NULL) {
	 return 1;
	}

	while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
	 line[strlen(line)-1]='\0';
	 //vty_out(vty,"%s", line);
	 sprintf(str,"python /root/L2/ACL_MACIP_Unapply_Interface.py %s %s",interface_name,line);
	 //vty_out(vty,str);
	 pipe_cmd(vty,str);
	 ++linenr;
	}
}

*/

// o.zeynalpour  vpls(Q in Q)
DEFUN (switchport_mode_dot1qtunnel,
        switchport_mode_dot1qtunnel_cmd,
"switchport mode dot1qtunnel",
"Set switching mode characteristics\n"
"Set trunking mode of the interface\n"
"Set trunking mode to tunnel unconditionally\n")
{
char cmd[300];
char str[400];
char *interface_name;
VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name= nic_names_get_vpp_from_tap(ifp->name);
if (strcmp(interface_name, "") == 0) {
vty_out(vty, "no such interface\n");
return CMD_WARNING;
}
sprintf(cmd, "sudo vppctl set interface l2 efp-filter %s", interface_name);
system(cmd);
enable_qinq++;
return CMD_SUCCESS;
}
DEFUN (no_switchport_mode_dot1qtunnel,
        no_switchport_mode_dot1qtunnel_cmd,
"no switchport mode dot1qtunnel",
NO_STR
"Set switching mode characteristics\n"
"Set trunking mode of the interface\n"
"Set trunking mode to tunnel unconditionally\n")
{
char cmd[300];
char str[400];
char *interface_name;
VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name= nic_names_get_vpp_from_tap(ifp->name);
if (strcmp(interface_name, "") == 0) {
vty_out(vty, "no such interface\n");
return CMD_WARNING;
}

if (strcmp(interface_name, "") == 0) {
vty_out(vty, "no such interface\n");
return CMD_WARNING;
}
sprintf(cmd, "sudo vppctl set interface l2 efp-filter %s disable", interface_name);
system(cmd);
enable_qinq++;
return CMD_SUCCESS;
}
DEFUN (switchport_access_vlan,
        switchport_access_vlan_cmd,
"switchport access vlan (1-4094)",
"Set switching mode characteristics\n"
"Set access mode characteristics of the Interface\n"
"Set vlan when interface is in access mode\n"
"vlan ID of the vlan when this port is in access mode\n")
{
char cmd[300];
char str[400];
char *interface_name;
VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name= nic_names_get_vpp_from_tap(ifp->name);
if (strcmp(interface_name, "") == 0) {
vty_out(vty, "no such interface\n");
return CMD_WARNING;
}

if (strcmp(interface_name, "") == 0) {
vty_out(vty, "no such interface\n");
return CMD_WARNING;
}

sprintf(cmd, "sudo vppctl set interface l2 tag-rewrite %s push dot1ad %s", interface_name, argv[3]->arg);
system(cmd);
return CMD_SUCCESS;
}


DEFUN (no_switchport_access_vlan,
        no_switchport_access_vlan_cmd,
"no switchport access vlan (1-4094)",
NO_STR
"Set switching mode characteristics\n"
"Set access mode characteristics of the Interface\n"
"Set vlan when interface is in access mode\n"
"vlan ID of the vlan when this port is in access mode\n")
{
char cmd[300];
char str[400];
char *interface_name;
VTY_DECLVAR_CONTEXT(interface, ifp);
interface_name= nic_names_get_vpp_from_tap(ifp->name);
if (strcmp(interface_name, "") == 0) {
vty_out(vty, "no such interface\n");
return CMD_WARNING;
}

if (strcmp(interface_name, "") == 0) {
vty_out(vty, "no such interface\n");
return CMD_WARNING;
}
sprintf(cmd, "sudo vppctl set interface l2 tag-rewrite %s disable", interface_name);
system(cmd);
return CMD_SUCCESS;
}


// routerlab end

static int if_config_write(struct vty *vty)
{
    struct vrf *vrf;
    struct interface *ifp;

    zebra_ptm_write(vty);

    RB_FOREACH (vrf, vrf_name_head, &vrfs_by_name)
    FOR_ALL_INTERFACES (vrf, ifp) {
        struct zebra_if *if_data;
        struct listnode *addrnode;
        struct connected *ifc;
        struct prefix *p;
        struct vrf *vrf;

        if_data = ifp->info;
        vrf = vrf_lookup_by_id(ifp->vrf_id);

        if (ifp->vrf_id == VRF_DEFAULT)
            vty_frame(vty, "interface %s\n", ifp->name);
        else
            vty_frame(vty, "interface %s vrf %s\n",
                      ifp->name, vrf->name);

        if (if_data) {
            if (if_data->shutdown == IF_ZEBRA_SHUTDOWN_ON)
                vty_out(vty, " shutdown\n");

            zebra_ptm_if_write(vty, if_data);
        }

        if (ifp->desc)
            vty_out(vty, " description %s\n", ifp->desc);

        /* Assign bandwidth here to avoid unnecessary interface
			   flap
			   while processing config script */
        if (ifp->bandwidth != 0)
            vty_out(vty, " bandwidth %u\n", ifp->bandwidth);

        if (!CHECK_FLAG(ifp->status,
                        ZEBRA_INTERFACE_LINKDETECTION))
            vty_out(vty, " no link-detect\n");

        for (ALL_LIST_ELEMENTS_RO(ifp->connected, addrnode,
                                  ifc)) {
            if (CHECK_FLAG(ifc->conf,
                           ZEBRA_IFC_CONFIGURED)) {
                char buf[INET6_ADDRSTRLEN];
                p = ifc->address;
                vty_out(vty, " ip%s address %s",
                        p->family == AF_INET ? ""
                                             : "v6",
                        inet_ntop(p->family,
                                  &p->u.prefix, buf,
                                  sizeof(buf)));
                if (CONNECTED_PEER(ifc)) {
                    p = ifc->destination;
                    vty_out(vty, " peer %s",
                            inet_ntop(p->family,
                                      &p->u.prefix,
                                      buf,
                                      sizeof(buf)));
                }
                vty_out(vty, "/%d", p->prefixlen);

                if (ifc->label)
                    vty_out(vty, " label %s",
                            ifc->label);

                vty_out(vty, "\n");
            }
        }

        if (if_data) {
            if (if_data->multicast
                != IF_ZEBRA_MULTICAST_UNSPEC)
                vty_out(vty, " %smulticast\n",
                        if_data->multicast
                        == IF_ZEBRA_MULTICAST_ON
                        ? ""
                        : "no ");
        }

        hook_call(zebra_if_config_wr, vty, ifp);

        link_params_config_write(vty, ifp);

        vty_endframe(vty, "!\n");
    }
	
	//mirzaei/port_security/running-config
	if(feature_p_sec == 0)
	{
		vty_out(vty,"\n!\nno feature port-security\n");
		return 1;
	}
	
	vty_out(vty,"\n!\nfeature port-security \n");

	int LINE_BUFSIZE = 100;
	char line[LINE_BUFSIZE];
	int linenr;
	FILE *pipe;
	//char *features[10];
	char *intfps;
	char *limit;
	char *sticky;
	char *shutdown;

	pipe = popen("sudo vppctl show l2 limit learn","r");
	if (pipe == NULL) {  // check for errors
	// perror(argv[0]); // report error message
		return 1;        // return with exit code indicating error
	}
	
	while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
		//vty_out(vty,"%s", line);
		
		//char arr[10][100];
		char target[20][100];
		int n=0,i,j=0;
	
		for(i=0;TRUE;i++)
		{
			if(line[i]!=' '){
				target[n][j++]=line[i];
			}
			else{
				target[n][j++]='\0';//insert NULL
				n++;
				j=0;
			}
			if(line[i]=='\0')
				break;
		}
		
		/*for(i=0;i<=n;i++)
			vty_out(vty,"%d = %s \n", i,target[i]);*/

		//vty_out(vty,"int_idx_ps = %s \n", target[2]);
		char int_idx_ps[100];
		memcpy(int_idx_ps, target[2]+1 , strlen(target[2])-1);
		int_idx_ps[strlen(target[2])-3] = '\0';
		//vty_out(vty,"int_idx = %s \n", int_idx_ps);
		
		int LINE_BUFSIZE2 = 100;
		char line2[LINE_BUFSIZE2];
		int linenr2;
		FILE *pipe2;
		char fname2[100];
		strcpy(fname2,"sudo vppctl show int | awk '$2 == \"");
		strcat(fname2,int_idx_ps);
		strcat(fname2,"\" {print $1}'");
		//vty_out(vty,"%s \n", fname2);
		pipe2 = popen(fname2,"r");
		if (pipe2 == NULL) {  // check for errors 
		// perror(argv[0]); // report error message 
		return 1;        // return with exit code indicating error 
		}

		while (fgets(line2, LINE_BUFSIZE2, pipe2) != NULL) {
		//vty_out(vty,"%s", line2);
		line2[strlen(line2)-1]='\0';
		++linenr2;
		}
		
		int LINE_BUFSIZE3 = 100;
		char line3[LINE_BUFSIZE3];
		int linenr3;
		FILE *pipe3;
		char fname3[100];
		strcpy(fname3,"sudo vppctl show tap-inject | awk '$1 == \"");
		strcat(fname3,line2);
		strcat(fname3,"\" {print $3}'");
		//vty_out(vty,"%s \n", fname2);
		pipe3 = popen(fname3,"r");
		if (pipe3 == NULL) {  // check for errors 
		// perror(argv[0]); // report error message 
		return 1;        // return with exit code indicating error 
		}

		while (fgets(line3, LINE_BUFSIZE3, pipe3) != NULL) {
		//vty_out(vty,"%s", line2);
		line3[strlen(line3)-1]='\0';
		++linenr3;
		}
		
		int ff=-1;

		for (int i=0; i<psec_i; i++)
		{
			if (strcmp(psec[i].interfce,line2) == 0)
			{
				ff=i;
				break;
			}
		}
		
		//vty_out(vty,"%s", line2);
		
		vty_out(vty," interface %s \n",line3);
		
		vty_out(vty,"switchport port-security \n");
				
		//vty_out(vty,"limit = %s \n", target[5]);
		//vty_out(vty,"sticky = %s \n", target[13]);
		//vty_out(vty,"shutdown = %s \n", target[17]);

		vty_out(vty,"switchport port-security maximum %s \n",target[5]);	
			
		if(atoi(target[17]) == 0)
			vty_out(vty,"switchport port-security violation restrict \n");
		else
			vty_out(vty,"switchport port-security violation shutdown \n");
			
		//vty_out(vty,"intface = %s",psec[ff].interfce);
		//vty_out(vty,"mac-set = %d",psec[ff].mac_set);
		if(psec[ff].mac_set == 0)
			vty_out(vty,"switchport port-security mac-address sticky \n");
		else
		{
			/*char mac_command[100];
			strcpy(mac_command,"switchport port-security mac-address ");
			strcat(mac_command,psec[ff].mac);
			strcat(mac_command," \n");*/
			vty_out(vty,"switchport port-security mac-address %s \n",psec[ff].mac);
			
		}
		
		vty_out(vty,"q \n");
		
		line[strlen(line)-1]='\0';
		++linenr;
		}	
    return 0;
}

/* Allocate and initialize interface vector. */
void zebra_if_init(void)
{
    /* Initialize interface and new hook. */
    hook_register_prio(if_add, 0, if_zebra_new_hook);
    hook_register_prio(if_del, 0, if_zebra_delete_hook);

    /* Install configuration write function. */
    install_node(&interface_node, if_config_write);
    install_node(&link_params_node, NULL);
    if_cmd_init();

    install_element(VIEW_NODE, &show_interface_cmd);
    install_element(VIEW_NODE, &show_interface_vrf_all_cmd);
    install_element(VIEW_NODE, &show_interface_name_vrf_cmd);
    install_element(VIEW_NODE, &show_interface_name_vrf_all_cmd);



    install_element(ENABLE_NODE, &show_interface_desc_cmd);
    install_element(ENABLE_NODE, &show_interface_desc_vrf_all_cmd);
    install_element(INTERFACE_NODE, &multicast_cmd);
    install_element(INTERFACE_NODE, &no_multicast_cmd);
    install_element(INTERFACE_NODE, &linkdetect_cmd);
    install_element(INTERFACE_NODE, &no_linkdetect_cmd);
    install_element(INTERFACE_NODE, &shutdown_if_cmd);
    install_element(INTERFACE_NODE, &no_shutdown_if_cmd);
    install_element(INTERFACE_NODE, &bandwidth_if_cmd);
    install_element(INTERFACE_NODE, &no_bandwidth_if_cmd);
    install_element(INTERFACE_NODE, &ip_address_cmd);
    install_element(INTERFACE_NODE, &no_ip_address_cmd);
    install_element(INTERFACE_NODE, &ip_address_peer_cmd);
    install_element(INTERFACE_NODE, &no_ip_address_peer_cmd);
    install_element(INTERFACE_NODE, &ipv6_address_cmd);
    install_element(INTERFACE_NODE, &no_ipv6_address_cmd);
    // o.zeynalpour vpls(Q in Q)
    install_element(INTERFACE_NODE, &switchport_mode_dot1qtunnel_cmd);
    install_element(INTERFACE_NODE, &switchport_access_vlan_cmd);
    install_element(INTERFACE_NODE, &no_switchport_mode_dot1qtunnel_cmd);
    install_element(INTERFACE_NODE, &no_switchport_access_vlan_cmd);
#ifdef HAVE_NETLINK
    install_element(INTERFACE_NODE, &ip_address_label_cmd);
	install_element(INTERFACE_NODE, &no_ip_address_label_cmd);
#endif /* HAVE_NETLINK */
    install_element(INTERFACE_NODE, &link_params_cmd);
    install_default(LINK_PARAMS_NODE);
    install_element(LINK_PARAMS_NODE, &link_params_enable_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_enable_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_metric_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_metric_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_maxbw_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_max_rsv_bw_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_unrsv_bw_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_admin_grp_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_admin_grp_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_inter_as_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_inter_as_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_delay_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_delay_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_delay_var_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_delay_var_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_pkt_loss_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_pkt_loss_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_ava_bw_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_ava_bw_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_res_bw_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_res_bw_cmd);
    install_element(LINK_PARAMS_NODE, &link_params_use_bw_cmd);
    install_element(LINK_PARAMS_NODE, &no_link_params_use_bw_cmd);
    install_element(LINK_PARAMS_NODE, &exit_link_params_cmd);

    //routerlab
    install_element (CONFIG_NODE,&xconnect_cmd);
    install_element (ENABLE_NODE,&show_xconnect_cmd);
    install_element (CONFIG_NODE,&no_xconnect_cmd);
//Reza Mirzaei: port_security
    install_element (ENABLE_NODE,&show_port_security_cmd);
    install_element (ENABLE_NODE,&show_l2fib_cmd);
    install_element (INTERFACE_NODE, &switchport_port_security_cmd);
    install_element (INTERFACE_NODE, &switchport_port_security_limit_cmd);
    install_element (INTERFACE_NODE, &switchport_port_security_violation_cmd);
    install_element (INTERFACE_NODE, &switchport_port_security_mac_address_cmd);
    install_element (INTERFACE_NODE, &switchport_port_security_mac_address_sticky_cmd);
    install_element (INTERFACE_NODE, &no_switchport_port_security_mac_address_cmd);
    install_element (INTERFACE_NODE, &no_switchport_port_security_mac_address_sticky_cmd);
    install_element (CONFIG_NODE,&feature_port_security_cmd);

//interface tunnel
    install_element (CONFIG_NODE,&interface_tunnel_cmd);
    install_element (CONFIG_NODE,&no_interface_tunnel_cmd);
    install_element (ENABLE_NODE,&show_interface_tunnel_cmd);
    install_element (INTERFACE_NODE, &ip_dhcp_client_cmd);

    num_aaa_radius=0;
    num_auth_radius=0;
    num_author_radius=0;
    num_acct_radius=0;
    install_element(CONFIG_NODE,&vtysh_aaa_new_model_cmd);
    install_element(CONFIG_NODE,&vtysh_radius_server_host_cmd);
    install_element(CONFIG_NODE,&vtysh_no_radius_server_host_cmd);
    install_element(CONFIG_NODE,&vtysh_aaa_authentication_cmd);
    install_element(CONFIG_NODE,&vtysh_aaa_authorization_cmd);
    install_element(CONFIG_NODE,&vtysh_aaa_accounting_cmd);
    install_element(CONFIG_NODE,&vtysh_no_aaa_new_model_cmd);
    install_element(ENABLE_NODE,&vtysh_show_radius_server_cmd);

    install_element (INTERFACE_NODE, &switchport_cmd);
    install_element (INTERFACE_NODE, &no_switchport_cmd);
//	install_element (INTERFACE_NODE, &no_ip_dhcp_snooping_trust_cmd);
//	install_element (INTERFACE_NODE, &ip_dhcp_snooping_trust_cmd);
    install_element (INTERFACE_NODE, &access_group_cmd);
    install_element (INTERFACE_NODE, &no_access_group_cmd);
    install_element (ENABLE_NODE, &show_access_group_cmd);
    install_element (ENABLE_NODE, &show_mac_access_group_cmd);
    install_element (INTERFACE_NODE, &mac_access_group_cmd);
    install_element (INTERFACE_NODE, &no_mac_access_group_cmd);
    //routerlab end
}


