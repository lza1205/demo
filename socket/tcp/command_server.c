/***********************************************************
�ļ���	:	command_server.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.15

˵��:	
	������֧�ֵ�������

***********************************************************/


#include "all.h"
#include "command.h"


extern char *gp_comman_tag[10];		//�����в���



/********************************************
������: __help
����:		help
*********************************************/

static void __help(void)
{
	int i;
	printf("/------------------- Welcome to Faks -------------------/\r\n");
	printf("You can enter the following command : \r\n");

	for(i = 0; gt_comman[i].name != NULL; i++)	//һ�����ȶ�
	{
		printf("[%d]%s       :   %s \r\n", i, gt_comman[i].name, gt_comman[i].help);
	}

	printf("/------------------- --------------- -------------------/\r\n");
}


struct command_t gt_comman_server[] = {

	{
		.name		= "help",
		.com_fun	= __help,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "show help",
	},

	{
		.name		= "?",
		.com_fun	= __help,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "show help",
	},

	{
		.name		= NULL,
		.com_fun 	= NULL,
		.tag_num	= 0,
		.tag_p		= NULL,
	},
};


void server_command_init(void)
{
	gt_comman = gt_comman_server;
}


