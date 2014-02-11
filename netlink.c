#include <net/sock.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>

#define NETLINK_USER 24

struct sock *nl_sk;

static void nl_recv_msg(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	int pid;
	struct sk_buff *skb_out;
	int msg_size;
	// char cbmsg[200];
	char msg[50] = " [0,0] this is a message from kernel.";
	int res;

	printk("function:%s\n",__FUNCTION__);
	
	nlh = nlmsg_hdr(skb);

	printk(KERN_INFO "receive from user:%s\n",(char *)nlmsg_data(nlh));

	// printf("kernel received this message: %s\n", (char *)nlmsg_data(nlh));
	// printf("and this has been written into /var/log/messages!\n");
	// strcat(cbmsg, (char *)nlmsg_data(nlh));
	// strcat(cbmsg, msg);

	msg_size = strlen(msg);

	pid = nlh->nlmsg_pid;

	skb_out = nlmsg_new(msg_size,0);
	if(!skb_out){
		printk(KERN_WARNING "failed to allocate new skb\n");
		return;
	}

	nlh = nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);
	NETLINK_CB(skb_out).dst_group = 0;
	strncpy(nlmsg_data(nlh),msg,msg_size);
	res = nlmsg_unicast(nl_sk,skb_out,pid);

}


static int netlink_init(void)
{
	printk("function:%s\n",__FUNCTION__);
	nl_sk = netlink_kernel_create(&init_net,NETLINK_USER,0,nl_recv_msg,NULL,THIS_MODULE);
	if(!nl_sk){
		printk(KERN_EMERG "fail to create socket\n");
		return -1;
	}
	return 0;
}

static void netlink_exit(void)
{
	printk(KERN_WARNING "exit netlink module\n");
	netlink_kernel_release(nl_sk);
}

module_init(netlink_init);
module_exit(netlink_exit);
