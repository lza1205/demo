

CROSS_COMPILE = 
#CROSS_COMPILE = i486-openwrt-linux-

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP


CCFLAGS+=-g -O -Wall
LDFLAGS+=-g -O -Wall

CFLAGS+=-g -O -Wall
LDFLAGS+=-g -O -Wall


INC = -I./include
LDLIB = -lpthread

obj-server += command.o 
obj-server += command_server.o 
obj-server += demo_tcp_server.o
obj-server += util.o


obj-client += command.o 
obj-client += command_client.o 
obj-client += demo_tcp_client.o
obj-client += util.o

all:tcp_server tcp_client

tcp_server:command.o command_server.o demo_tcp_server.o util.o
	$(CC) $(LDFLAGS) $(INC) -o tcp_server $(obj-server) $(LDLIB)

tcp_client:command.o command_client.o demo_tcp_client.o util.o
	$(CC) $(LDFLAGS) $(INC) -o tcp_client $(obj-client) $(LDLIB)

%.o:%.c
	$(CC) -c $(CPPFLAGS) $(CCFLAGS) $< -o $@ $(INC)
	

clean:
	rm *.o tcp_server tcp_client

