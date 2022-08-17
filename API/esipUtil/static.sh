PWD=`pwd | awk '{print $1}'`
TMP=$PWD
echo "Util  Home = $TMP"
EUTIL_HOME=$TMP

if [ -f "eutil_s.mak" ] ;then
	echo "Already Exist File"
else
	echo "EUTIL_HOME=$TMP">>eutil_s.mak
	echo "EUTIL_INC= -I${EUTIL_HOME}">>eutil_s.mak
	echo "EUTIL_LIB= -L${EUTIL_HOME} -leSipUtil_s">>eutil_s.mak
	echo "EUTIL_DEF= -DEUTIL_STATIC">>eutil_s.mak

	echo "TBB_USE=false">>eutil_s.mak
	echo "TBB_INCDIR=">>eutil_s.mak
	echo "TBB_LIBDIR=">>eutil_s.mak
	echo "TBB_LIBNAME=">>eutil_s.mak
	echo "TBB_LIBDEPEND=">>eutil_s.mak
	echo "TBB_DEF=">>eutil_s.mak

	echo "ifeq"" ($""(TBB_USE),true)">>eutil_s.mak
	echo "TBB_INCDIR= ${EUTIL_HOME}/tbb/include/tbb">>eutil_s.mak
	echo "TBB_LIBDIR= ${EUTIL_HOME}/tbb/lib/intel64/gcc4.4">>eutil_s.mak
	echo "TBB_LIBNAME=tbb">>eutil_s.mak
	echo "TBB_LIBDEPEND=${EUTIL_HOME}/tbb/lib/intel64/gcc4.4/libtbb.so.2">>eutil_s.mak
	echo "TBB_DEF=TBB_USE">>eutil_s.mak
	echo "EUTIL_INC+= -I${EUTIL_HOME}/tbb/include/tbb">>eutil_s.mak
	echo "EUTIL_LIB+= -L${EUTIL_HOME}/tbb/lib/intel64/gcc4.4 -ltbb">>eutil_s.mak
	echo "EUTIL_DEF+= -D${TBB_DEF}">>eutil_s.mak
	echo "endif">>eutil_s.mak
	chmod 777 eutil_s.mak
fi

make -f Makefile.static clean
make -f Makefile.static
make -f Makefile.static install

rm -rf *.o

