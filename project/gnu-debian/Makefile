include /usr/src/uk/sbase.mk

CC_TARGET   = libsbase.so
CC_TARGET_D = libsbased.so
INSTALL_PATH= ../../lib/linux-debian

SRCS =  ../../src/*.cpp ../../src/db/*.cpp ../../src/comm/*.cpp
OBJS =  *.o 

all:
		$(CC) -c $(SRCS) $(CC_INCLUDES) $(CC_COMPLIE)
		$(RM) $(CC_TARGET)
		$(CC) -shared $(OBJS) -o $(CC_TARGET)
		$(RM) *.o
		$(CP) $(CC_TARGET) $(INSTALL_PATH)/

debug:
		$(CC) -c $(SRCS) $(CC_INCLUDES) $(CC_COMPLIE_D)
		$(RM) $(CC_TARGET)
		$(CC) -shared $(OBJS) -o $(CC_TARGET_D)
		$(RM) *.o
		$(CP) $(CC_TARGET_D) $(INSTALL_PATH)/

clean:
		$(RM) *.o
		$(RM) $(CC_TARGET)

install:
		$(CP) $(CC_TARGET) $(INSTALL_PATH)/$(CC_TARGET)
