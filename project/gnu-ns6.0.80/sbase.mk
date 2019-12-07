SBASE_DIR   := /mnt/hgfs/Software_Platform/sbase

CC_INCLUDES := -I./ -I/usr/include -I/usr/local/include -I/usr/include/mysql -I/usr/src/uk/oci/include -I$(SBASE_DIR)/include  -I$(SBASE_DIR)/include/comm -I$(SBASE_DIR)/include/db -I$(SBASE_DIR)/include/db/mysql -I$(SBASE_DIR)/include/db/oracle -I$(SBASE_DIR)/include/db/mdb/  -I$(SBASE_DIR)/include/gui  -I$(SBASE_DIR)/include/gui/qt  -I$(SBASE_DIR)/include/gui/svg -I$(SBASE_DIR)/include/print -I$(SBASE_DIR)/include/print/usb_linux

CC_FLAGS    := -pipe -w -O3 -pipe 
CC_FLAGS_D  := -pipe -Wall -O2 -g -pipe
CC_COMPLIE  := $(CC_FLAGS) $<
CC_COMPLIE_D:= $(CC_FLAGS_D) $<
CC_LINK     := -L/usr/lib/ -L/usr/local/lib/ -L$(SBASE_DIR)/lib/linux -lsbase -lpthread -lm -lrt
CC_LINK_D   := -L/usr/lib/ -L/usr/local/lib/ -L$(SBASE_DIR)/lib/linux -lsbased -lpthread -lm -lrt

CC := g++
AR = ar cqs
RM := rm -rf
LN := ln
CP := cp -f

