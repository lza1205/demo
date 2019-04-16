
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <Windows.h>
#include <pthread.h>
#include <malloc.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <netdb.h>


#include <sys/time.h>

#include <signal.h>



#define SERVER_PORT_TCP			6666
#define TCP_BACKLOG 10


/* 在sock_fd 进行监听，在 new_fd 接收新的链接 */
int sock_fd, new_fd;



int client_handle(int sock_fd, struct sockaddr_in *cli_addr)
{
	ssize_t ret;
	char recvbuf[512];

	while(1)
	{
		if((ret = recv(sock_fd, recvbuf, sizeof(recvbuf), 0)) == -1){
			printf("recv error \r\n");
			return -1;
		}
		printf("recv :\r\n");
		printf_hex(recvbuf, ret);
		printf("\r\n");
	}
}


void sig_chld(int signo)
{
	pid_t pid;
	int stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\r\n", pid);
	return;
}


void *__tcp_server(void *pdata)
{
	
	/* 自己的地址信息 */
	struct sockaddr_in my_addr;
	/*	连接者的地址信息*/
	struct sockaddr_in their_addr;
	int sin_size;

	struct sockaddr_in *cli_addr;

	/* 错误检查 */
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket is error\r\n");
		exit(1);
	}

	/* 主机字节顺序 */
	/* 协议 */
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVER_PORT_TCP);
	/* 当前IP 地址写入 */
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* 将结构体其余的都清零 */
	bzero(&(my_addr.sin_zero), 8);

	/* bind 绑定*/
	if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind is error\r\n");
		exit(1);
	}

	/* 开始监听 */
	if(listen(sock_fd, TCP_BACKLOG) == -1)
	{
		perror("listen is error\r\n");
		exit(1);
	}

	printf("start accept\n");

	/* 因为我们后面会创建出许多子进程来服务新的链接
		一旦子进程异常终止了，需要父进程来进行资源回收
	*/
	signal(SIGCHLD, sig_chld);			//在这里处理僵死进程

	/* accept() 循环 */
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		if((new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size)) == -1)
		{
			perror("accept");
			continue;
		}

		cli_addr = malloc(sizeof(struct sockaddr));

		printf("accept addr\r\n");

		if(cli_addr != NULL)
		{
			memcpy(cli_addr, &their_addr, sizeof(struct sockaddr));
		}
		
		//处理目标
		client_handle(new_fd, cli_addr);
		
		close(new_fd);
	}
}


void create_tcp_server(void)
{
	pthread_t server_thread;
    pthread_attr_t server_thread_attr;

	/* 创建子进程 */
	pthread_attr_init(&server_thread_attr);		//初始化进程属性
	pthread_attr_setdetachstate(&server_thread_attr, PTHREAD_CREATE_DETACHED);
//	  if (pthread_create(&recv_thread, &recv_thread_attr, recv_pthread, NULL) < 0)
	if (pthread_create(&server_thread, &server_thread_attr, __tcp_server, NULL) < 0)
	{
		perror("pthread_create");
	}
}

int main(void)
{
	char command[1024];
	char *str;

	create_tcp_server();

	/* 初始化命令行 */
	server_command_init();

	while(1)
	{
        str = fgets(command, 1024, stdin);
		if(str != NULL)
		{
	        command_format(command);
			printf("server @: $ ");
		}

	}
	
	return 0;
}


