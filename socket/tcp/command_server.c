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

void set_3a_switch(char flag, char of);


void get_3a_switch(void);



void get_3a_iccid(void);


void __set_switch(void)
{
	char *tag_1 = gp_comman_tag[1];
	char *tag_2 = gp_comman_tag[2];
	
	set_3a_switch((*tag_1) - '0', (*tag_2) - '0');
}

void __get_switch(void)
{
	get_3a_switch();
}


void __get_iccid(void)
{
	get_3a_iccid();
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
		.name		= "set_switch",
		.com_fun	= __set_switch,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "you can enter : set_switch 1/2 0/1/2",
	},

	{
		.name		= "get_switch",
		.com_fun	= __get_switch,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "you can enter : get_switch",
	},

	{
		.name		= "get_iccid",
		.com_fun	= __get_iccid,
		.tag_num	= 0,
		.tag_p		= NULL,
		.help 		= "you can enter : get_iccid",
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


