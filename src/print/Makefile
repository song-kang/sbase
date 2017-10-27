CC = g++
AR            = ar cqs

CC_INCLUDES = -I/usr/include -I/usr/local/include -I../ysbase

CC_FLAGS    = $(CC_INCLUDES) -pipe -w -O3 -pipe 
CC_FLAGS_D  = $(CC_INCLUDES) -pipe -Wall -O2 -g -pipe
CC_COMPLIE  = $(CC_INCLUDES) $(CC_FLAGS) $<
CC_COMPLIE_D= $(CC_INCLUDES) $(CC_FLAGS_D) $<
CC_LINK     = -L/usr/lib/ -L/usr/local/lib/ -lpq -lpthread -lysbase -lm -lrt

CC_TARGET   = libsbase_esc.a
INSTALL_PATH= /usr/ysr/

RM = rm -rf
LN = ln
CP = cp -f

SRCS =  *.cpp 
BK = usb_linux/*.c
OBJS =  *.o 

all:
		$(CC) -c $(SRCS) $(CC_COMPLIE)
		$(AR) $(CC_TARGET) $(OBJS)

debug:
		$(CC) -c $(SRCS) $(CC_COMPLIE_D)
		$(AR) $(CC_TARGET) $(OBJS)

clean:
		$(RM) *.o
		$(RM) *.d
		$(RM) *.a

install:
		$(CP) $(CC_TARGET) $(INSTALL_PATH)/$(CC_TARGET)
