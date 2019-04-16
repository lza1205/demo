

#include "all.h"


int sockfd;

#define SERVER_IP	"106.13.62.194"
#define SERVER_PORT	6666

int __cli_handle(int sock_fd)
{
	ssize_t ret;
	char recvbuf[512];

	while(1)
	{
		if((ret = recv(sock_fd, &recvbuf, sizeof(recvbuf), 0)) == -1){
			return -1;
		}

		printf_hex(recvbuf, ret);
	}

}


void *__tcp_client(void *pdata)
{

	/*  �����ߵ�������Ϣ */
	struct sockaddr_in their_addr;	

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		/*  ���socket()���ó��ִ�������ʾ������Ϣ���˳� */
		perror("socket");
//		exit(1);
	}

	/*  �����ֽ�˳�� */
	their_addr.sin_family = AF_INET;
	/*  �����ֽ�˳�򣬶����� */
	their_addr.sin_port = htons(SERVER_PORT);
	their_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	/*  ���ṹʣ�µĲ�������*/
	bzero(&(their_addr.sin_zero), 8);
	if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		/*  ���connect()�������Ӵ�������ʾ��������Ϣ���˳� */
		perror("connect");
		exit(1);
	}


	if(__cli_handle(sockfd) == 0)
	{
		close(sockfd);

		exit(0xa);
	}
	

	close(sockfd);

	return NULL;
}

void create_tcp_client(void)
{
	pthread_t client_thread;
    pthread_attr_t client_thread_attr;

	/* �����ӽ��� */
	pthread_attr_init(&client_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&client_thread_attr, PTHREAD_CREATE_DETACHED);
//	  if (pthread_create(&recv_thread, &recv_thread_attr, recv_pthread, NULL) < 0)
	if (pthread_create(&client_thread, &client_thread_attr, __tcp_client, NULL) < 0)
	{
		perror("pthread_create");
	}
}


int main(void)
{
	char command[1024];
	char *str;

	create_tcp_client();

	/* ��ʼ�������� */
	client_command_init();

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


