##############################################################################
# SAMPLE Makefile #
##############################################################################
# source & object files
include ../API/esipUtil/eutil_s.mak

##############################################################################
# General
##############################################################################
INCLSFX=.h
CPPSFX=.cpp
OBJSFX=.o
EXESFX=
AR=ar
MYCC = g++
MYLNK = g++
TARGET = MTM
BINDIR=../../bin/
LIBDIR=./

##############################################################################
# Define 
##############################################################################
DEF = -g -Wall -Wno-deprecated -DDEBUG  -DUNIX -D_REENTRANT
DEF += ${EUTIL_DEF}

##############################################################################
# OBJ LIST
##############################################################################
MAIN_OBJ  = main.o MainConfig.o NasSystemTable.o
MAIN_OBJ += TrsgTransport.o TrsgTransaction.o TrsgTransactionMgr.o TrsgTransactionTimer.o
MAIN_OBJ += Worker.o GarbageTimer.o #TargetContent.o
MAIN_OBJ += Session.o SessionMgr.o SessionTimer.o DailyTime.o
MAIN_OBJ += TrsgNodeMgr.o TrsgNode.o easyzlib.o 
MAIN_OBJ += CZip.o CBase64.o
MAIN_OBJ += tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o
MAIN_OBJ += AppXmlParser.o


OBJSRC_ALL =  ${MAIN_OBJ}

##############################################################################
# Cpp Flags
##############################################################################
CPPINCLS += ${EUTIL_INC}
#CPPINCLS += -I../API/pdk-3.2.2/include
#CPPINCLS += -I../include/ext/bcf_if/omc
#CPPINCLS += -I../API/alcomm/if/
#CPPINCLS += -I../include/eRte/
CPPINCLS += -I../include/resip/
CPPINCLS += -I../include/rutil/
CPPINCLS += -I../API/resiprocate-1.9.1/
CPPINCLS += -I./tinyxml/

CPPFLAGS += ${DEF} ${CPPINCLS}

##############################################################################
# Append links
##############################################################################
LINKLIBS = -L./ -lpthread -L/usr/local/lib -lstdc++ -lcurses -lz #-L../API/pdk-3.2.2/lib -lbase 
LINKLIBS += ${EUTIL_LIB}
LINKLIBS += -L/usr/local/lib -lrutil -lresip

##############################################################################
# OS Defined
##############################################################################
HOST := $(shell uname)
ifeq ($(HOST),hpux)
        LINKLIBS  += -lrt -lcl
endif
ifeq ($(HOST),Linux)
        LINKLIBS  += -lm -ldl -lnsl -lrt 
        CPPFLAGS  += -DUNIX_SVR4 -DLINUX -D_LINUX_ -D__LINUX__
endif
ifeq ($(HOST),SunOS)
        LINKLIBS  += -lm -ldl -lresolv -lsocket -lnsl -lrt -lposix4 -lgen
        CPPFLAGS  += -DUNIX_SVR4 
endif

##############################################################################	
# build part
# Rules
##############################################################################

COMPILE_cpp=${MYCC} ${CPPFLAGS} -c $< -o $@
COMPILE_cpp_OUT=$$($(COMPILE_cpp) 2>&1 | sed -e 's/: error:/\: \\e[1;31;5merror\\\e[0m:/g' -e 's/: warning:/\: \\e[1;33;5mwarning\\\e[0m:/g')

.SUFFIXES: .cpp .o
SRC = ${OBJS:.o=.cpp}

.cpp.o: ${SRC}
	@echo -e "===============> C++ Compile \e[1;40;35m[$<]\e[0m "
	@echo -e "$(COMPILE_cpp)\n$(COMPILE_cpp_OUT)"

all: ${TARGET}
	@echo -e "\e[1;40;32m****************************\e[0m"
	@echo -e "\e[1;40;32;5m$(TARGET) made successfully.\e[0m"    
	@echo -e "\e[1;40;32m****************************\e[0m"

install:
	#cp -p ${TARGET} ${BINDIR}

clean:
	/bin/rm -f core ${OBJSRC_ALL} ${TARGET} 

update:
	mv -f ${TARGET} /home/ibc/ibc/bin/${TARGET}

${TARGET}: ${OBJSRC_ALL} 
	${MYCC} -o ${TARGET} ${OBJSRC_ALL} ${LINKLIBS}
##############################################################################
# EOF
##############################################################################



