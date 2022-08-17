PWD=`pwd | awk '{print $1}'`
TMP=$PWD
echo "Util  Home = $TMP"
EUTIL_HOME=$TMP

if [ -f "eutil.mak" ] ;then
	echo "Already Exist File"
else
	echo "EUTIL_HOME=$TMP">>eutil.mak
	echo "EUTIL_INC= -I${EUTIL_HOME}">>eutil.mak
	echo "EUTIL_LIB= -L${EUTIL_HOME} -leSipUtil">>eutil.mak
	echo "EUTIL_DEF= -DEUTIL_DYNAMIC">>eutil.mak

	echo "TBB_USE=false">>eutil.mak
	echo "TBB_INCDIR=">>eutil.mak
	echo "TBB_LIBDIR=">>eutil.mak
	echo "TBB_LIBNAME=">>eutil.mak
	echo "TBB_LIBDEPEND=">>eutil.mak
	echo "TBB_DEF=">>eutil.mak

	echo "ifeq"" ($""(TBB_USE),true)">>eutil.mak
	echo "TBB_INCDIR= ${EUTIL_HOME}/tbb/include/tbb">>eutil.mak
	echo "TBB_LIBDIR= ${EUTIL_HOME}/tbb/lib/intel64/gcc4.4">>eutil.mak
	echo "TBB_LIBNAME=tbb">>eutil.mak
	echo "TBB_LIBDEPEND=${EUTIL_HOME}/tbb/lib/intel64/gcc4.4/libtbb.so.2">>eutil.mak
	echo "TBB_DEF=TBB_USE">>eutil.mak
	echo "EUTIL_INC+= -I${EUTIL_HOME}/tbb/include/tbb">>eutil.mak
	echo "EUTIL_LIB+= -L${EUTIL_HOME}/tbb/lib/intel64/gcc4.4 -ltbb">>eutil.mak
	echo "EUTIL_DEF+= -D${TBB_DEF}">>eutil.mak
	echo "endif">>eutil.mak
	chmod 777 eutil.mak
fi

make clean
make 
make install
rm -rf *.o

