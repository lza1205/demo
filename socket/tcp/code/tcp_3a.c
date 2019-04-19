
#define __TCP_3A_C__

#include "all.h"
#include "tcp_3a.h"

extern int g_iCliFd;

u8 check_crc(u8 *buf, int len)
{
	u8 i;
	u8 crc;

	crc = buf[0];
	
	for(i = 1; i < len; i++)
	{
		crc ^= buf[i];
	}

	return crc;
}


void send_3a_data(int socket_fd, u8 cmd, u8 *buf, u16 size)
{
	u8 data[100];
	u32 i = 0;
	u16 len = size;

	data[i ++] = PRO_3A_HEAD_1;
	data[i ++] = PRO_3A_HEAD_2;

	data[i ++] = cmd;

	data[i ++] = (u8)(len >> 8);
	data[i ++] = (u8)(len & 0x00ff);

	if(buf != NULL)
	{
		memcpy(data + i, buf, len);
		i += len;
	}
	else
	{
		i --;
		len = 0;
	}
	

	data[i ++] = check_crc(&data[3], 2 + len);

	data[i ++] = PRO_3A_END_1;
	data[i ++] = PRO_3A_END_2;

	//printf("socket_fd : %d  i is %d \r\n", socket_fd, i);

	write(socket_fd, data, i);
}


void recv_3a(int socket_fd, u8 *buf, int len)
{
	if((buf[INDEX_HEAD_1] == PRO_3A_HEAD_1) && (buf[INDEX_HEAD_2] == PRO_3A_HEAD_2))
	{
		switch(buf[INDEX_CMD])
		{
			case CLIENT_LOGIN:
				printf("CLIENT_LOGIN \r\n");
				send_3a_data(socket_fd, SERVER_LOGIN_ACK, NULL, 0);

				break;

			case CLIENT_HEART:
				printf("CLIENT_HEART \r\n");
				break;
				
			case CLIENT_SET_SWITCH_ACK:
				printf("CLIENT_SET_SWITCH_ACK \r\n");
				break;

			case CLIENT_GET_OUT_POINT_ACK:
				printf("CLIENT_GET_OUT_POINT_ACK \r\n");
				break;

			case CLIENT_GET_CHECK_POINT_ACK:
				printf("CLIENT_GET_CHECK_POINT_ACK \r\n");
				break;

			case CLIENT_UPLOAD_SWITCH:
				printf("CLIENT_UPLOAD_SWITCH \r\n");
				break;

			case CLIENT_GET_ICCID_ACK:
				printf("CLIENT_GET_ICCID_ACK \r\n");
				break;
				
		}
	}
}



void set_3a_switch(char flag, char of)
{
	u8 buf[2];

	buf[0] = flag;
	buf[1] = of;
	
	send_3a_data(g_iCliFd, SERVER_SET_SWITCH, buf, 2);
}

void get_3a_switch(void)
{
	send_3a_data(g_iCliFd, SERVER_GET_SWITCH, NULL, 0);
}


void get_3a_iccid(void)
{
	send_3a_data(g_iCliFd, SERVER_GET_ICCID, NULL, 0);
}


