#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#define	NETLINK_USER 24
#define MAX_PAYLOAD 1024

struct sockaddr_nl src_addr,dest_addr;
struct nlmsghdr *nlh=NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int main(int argc,char **argv)
{
	//while(1){
		char usr_msg[100] = {'0'};
		printf("something you want say to kernel\n");
		scanf("%s", usr_msg);
		printf("the mesg you have input is :%s\n", usr_msg);
	
		sock_fd = socket(PF_NETLINK,SOCK_RAW,NETLINK_USER);

		memset(&src_addr,0,sizeof(src_addr));
		src_addr.nl_family = AF_NETLINK;
		src_addr.nl_pid	= getpid();

		bind(sock_fd,(struct sockaddr*)&src_addr,sizeof(src_addr));

		memset(&dest_addr,0,sizeof(dest_addr));
		dest_addr.nl_family = AF_NETLINK;
		dest_addr.nl_pid = 0;/*send to the kernel*/
		dest_addr.nl_groups = 0;/*unicast*/
		
		nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PAYLOAD));
		memset(nlh,0,NLMSG_SPACE(MAX_PAYLOAD));
		
		nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
		nlh->nlmsg_pid = getpid();
		nlh->nlmsg_flags = 0;
	
		
		strcpy(NLMSG_DATA(nlh), usr_msg);
	
		iov.iov_base = (void *)nlh;
		iov.iov_len = nlh->nlmsg_len;
		msg.msg_name = (void *)&dest_addr;
		msg.msg_namelen = sizeof(dest_addr);
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;

		printf("sending message to kernel\n");
		sendmsg(sock_fd,&msg,0);
		printf("waiting for message from kernel\n");
		recvmsg(sock_fd,&msg,0);
		printf("receive message:%s\n",(char *)NLMSG_DATA(nlh));
		printf("you can see detail mesg in /var/log/mesg \n");
						//return 0;
	//}
}
