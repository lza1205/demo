



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


// 事件base 
struct event_base* base; 

int g_iCliFd;


// 读事件回调函数 
void onRead(int iCliFd, short iEvent, void *arg) 
{ 
    int iLen; 
    char buf[1500]; 

    iLen = recv(iCliFd, buf, 1500, 0); 

    if (iLen <= 0) { 
        printf("Client Close \r\n"); 

        // 连接结束(=0)或连接错误(<0)，将事件删除并释放内存空间 
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
	这里执行我们的函数
	*/
	recv_3a(iCliFd, buf, iLen);
	g_iCliFd = iCliFd;

    //write(iCliFd, buf, iLen + 1);
} 


//连接请求事件回调函数
void onAccept(int sockfd, short iEvent, void *arg)
{
    int iclifd;
    struct sockaddr_in scliAddr;

    socklen_t iSinSize = sizeof(scliAddr);
    iclifd = accept(sockfd, (struct sockaddr *)&scliAddr, &iSinSize);

    //注册新的事件
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

    //初始化base
    base = event_base_new();

    struct event evListen;
    //设置事件
    event_set(&evListen, sockfd, EV_READ|EV_PERSIST, onAccept, NULL);
    //设置为base 事件
    event_base_set(base, &evListen);
    //添加事件
    event_add(&evListen, NULL);

    //事件循环
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

		/* 发送信息*/
		printf("foreach_call_fd %s \r\n", intf_fp->name);
	}

	return 0;
}

void foreach_socket(void *pdata)
{
	
	event_base_foreach_event(base, foreach_call_fd, pdata);
}
#endif

