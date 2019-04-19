



#include <stdio.h>
#include <string.h>
#include <sys/socket.h>     
#include <netinet/in.h>     
#include <arpa/inet.h>     
#include <netdb.h> 
#include <stdlib.h>
#include <unistd.h>


#include <event2/event_struct.h>
#include <event2/event_compat.h>
#include <event2/event.h>


// �¼�base 
struct event_base* base; 

int g_iCliFd;


// ���¼��ص����� 
void onRead(int iCliFd, short iEvent, void *arg) 
{ 
    int iLen; 
    char buf[1500]; 

    iLen = recv(iCliFd, buf, 1500, 0); 

    if (iLen <= 0) { 
        printf("Client Close \r\n"); 

        // ���ӽ���(=0)�����Ӵ���(<0)�����¼�ɾ�����ͷ��ڴ�ռ� 
        struct event *pEvRead = (struct event*)arg; 
        event_del(pEvRead); 

        free(pEvRead); 

        close(iCliFd); 
        return; 
    } 

	printf("iCliFd : %d  \r\n", iCliFd);
	//write(iCliFd, buf, iLen + 1);
	

    //buf[iLen] = 0; 
    printf("Client Info: \r\n");
	printf_hex(buf, iLen);

	/*
	����ִ�����ǵĺ���
	*/
	recv_3a(iCliFd, buf, iLen);
	g_iCliFd = iCliFd;

    //write(iCliFd, buf, iLen + 1);
} 


//���������¼��ص�����
void onAccept(int sockfd, short iEvent, void *arg)
{
    int iclifd;
    struct sockaddr_in scliAddr;

    socklen_t iSinSize = sizeof(scliAddr);
    iclifd = accept(sockfd, (struct sockaddr *)&scliAddr, &iSinSize);

    //ע���µ��¼�
    struct event *pEvRead = malloc(sizeof(struct event));
    if(pEvRead == NULL)
    {
        close(iclifd);
        return ;
    }
    event_set(pEvRead, iclifd, EV_READ|EV_PERSIST | EV_WRITE, onRead, pEvRead); 
    event_base_set(base, pEvRead); 
    event_add(pEvRead, NULL); 
}

int socket_event_init(int port)
{
    int sockfd;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sockfd, 10);

    //��ʼ��base
    base = event_base_new();

    struct event evListen;
    //�����¼�
    event_set(&evListen, sockfd, EV_READ|EV_PERSIST, onAccept, NULL);
    //����Ϊbase �¼�
    event_base_set(base, &evListen);
    //����¼�
    event_add(&evListen, NULL);

    //�¼�ѭ��
    event_base_dispatch(base);

    return 0;
}

#if 0
static int
foreach_call_fd(const struct event_base *base, const struct event *ev, void *arg)
{
	struct inotify_fd *intf_fp = (struct inotify_fd *)arg;
	
	int fd;
	
	if (event_get_callback(ev) == onRead)
	{
		fd = event_get_fd(ev);

		/* ������Ϣ*/
		printf("foreach_call_fd %s \r\n", intf_fp->name);
	}

	return 0;
}

void foreach_socket(void *pdata)
{
	
	event_base_foreach_event(base, foreach_call_fd, pdata);
}
#endif

