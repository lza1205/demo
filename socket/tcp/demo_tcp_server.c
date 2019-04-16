
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


/* ��sock_fd ���м������� new_fd �����µ����� */
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
	
	/* �Լ��ĵ�ַ��Ϣ */
	struct sockaddr_in my_addr;
	/*	�����ߵĵ�ַ��Ϣ*/
	struct sockaddr_in their_addr;
	int sin_size;

	struct sockaddr_in *cli_addr;

	/* ������ */
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket is error\r\n");
		exit(1);
	}

	/* �����ֽ�˳�� */
	/* Э�� */
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVER_PORT_TCP);
	/* ��ǰIP ��ַд�� */
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* ���ṹ������Ķ����� */
	bzero(&(my_addr.sin_zero), 8);

	/* bind ��*/
	if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind is error\r\n");
		exit(1);
	}

	/* ��ʼ���� */
	if(listen(sock_fd, TCP_BACKLOG) == -1)
	{
		perror("listen is error\r\n");
		exit(1);
	}

	printf("start accept\n");

	/* ��Ϊ���Ǻ���ᴴ��������ӽ����������µ�����
		һ���ӽ����쳣��ֹ�ˣ���Ҫ��������������Դ����
	*/
	signal(SIGCHLD, sig_chld);			//�����ﴦ��������

	/* accept() ѭ�� */
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
		
		//����Ŀ��
		client_handle(new_fd, cli_addr);
		
		close(new_fd);
	}
}


void create_tcp_server(void)
{
	pthread_t server_thread;
    pthread_attr_t server_thread_attr;

	/* �����ӽ��� */
	pthread_attr_init(&server_thread_attr);		//��ʼ����������
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

	/* ��ʼ�������� */
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


