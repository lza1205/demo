/***********************************************************
文件名	:	command_server.c
作者		:	Faker
版本号	:	1.0
日期		:	2015.05.15

说明:	
	服务器支持的命令行

***********************************************************/


#include "all.h"
#include "command.h"


extern char *gp_comman_tag[10];		//命令行参数



/********************************************
函数名: __help
功能:		help
*********************************************/

static void __help(void)
{
	int i;
	printf("/------------------- Welcome to Faks -------------------/\r\n");
	printf("You can enter the following command : \r\n");

	for(i = 0; gt_comman[i].name != NULL; i++)	//一个个比对
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


