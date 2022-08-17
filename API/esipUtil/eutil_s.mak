EUTIL_HOME=/home/wtrsp/API/esipUtil
EUTIL_INC= -I/home/wtrsp/API/esipUtil
EUTIL_LIB= -L/home/wtrsp/API/esipUtil/lib -leSipUtil_s
EUTIL_DEF= -DEUTIL_STATIC
TBB_USE=false
TBB_INCDIR=
TBB_LIBDIR=
TBB_LIBNAME=
TBB_LIBDEPEND=
TBB_DEF=
ifeq ($(TBB_USE),true)
TBB_INCDIR= /home/wtrsp/API/esipUtil/tbb/include/tbb
TBB_LIBDIR= /home/wtrsp/API/esipUtil/tbb/lib/intel64/gcc4.4
TBB_LIBNAME=tbb
TBB_LIBDEPEND=/home/wtrsp/API/esipUtil/tbb/lib/intel64/gcc4.4/libtbb.so.2
TBB_DEF=TBB_USE
EUTIL_INC+= -I/home/wtrsp/API/esipUtil/tbb/include/tbb
EUTIL_LIB+= -L/home/wtrsp/API/esipUtil/tbb/lib/intel64/gcc4.4 -ltbb
EUTIL_DEF+= -D
endif
