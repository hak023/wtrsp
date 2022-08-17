#alox tcp-ip server client commnication if

CC=g++

DEBUG = -g -Wall -DDEBUG  -DUNIX -D_REENTRANT -DUNIX_SVR4 -DLINUX -D_LINUX_ 
DEBUG += -DPI_USING_POLL
DEFS    = $(DEBUG)

#PDK
PDKDIR=../pdk-3.2.2
PDKINCS=-I${PDKDIR}/include/
PDKLIBS=-L${PDKDIR}/lib -lbase



INCS = -I./if $(PDKINCS) 
LIBS =$(PDKLIBS) 

CFLAGS=$(DEFS) $(INCS)

TARGET=alcomm

LIBDIR=./lib

OBJS =aloxConf.o \
		aloxUtil.o \
		cdrlog.o \
		tcpif.o \
		ifcfg.o \
		nwm_api.o \
		nwm_mgr.o \

#		mgpt_util.o \
#		aloxProcInfo.o \


.SUFFIXES: .c .cpp .o
.c.o:
	$(CC) $(CFLAGS) $(INCS)-c -o $*.o $<

.cpp.o:
	$(CXX) $(CFLAGS) $(INCS) -c -o $*.o $<


all :  $(TARGET)

$(TARGET): $(OBJS)
	rm -f  $(LIBDIR)/lib$(TARGET).a
	ar rcu $(LIBDIR)/lib$(TARGET).a $(OBJS)

clean:  
	rm -f  *.a *.o $(LIBDIR)/lib$(TARGET).a


r:	clean all
