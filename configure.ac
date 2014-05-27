## autoconf configure.ac && autoreconf -if

AC_INIT(DESCRIPTION)
AC_PROG_CXX
AC_LANG([C++])


### Setup custom CUDA paths
CUDA_INSTALL_PATH="/usr/local/cuda"
CUDA_SDK_INSTALL_PATH="/usr/local/NVIDIA_GPU_Computing_SDK"

AC_ARG_WITH([cuda],
  [AS_HELP_STRING(
    [--with-cuda=PATH],
    [prefix where CUDA is installed @<:@default=auto@:>@])],
  [CUDA_INSTALL_PATH=$withval],
  [with_cuda=auto])
AC_ARG_WITH([cuda-sdk],
  [AS_HELP_STRING(
    [--with-cuda-sdk=PATH],
    [prefix where CUDA SDK is installed @<:@default=auto@:>@])],
  [CUDA_SDK_INSTALL_PATH=$withval],
  [with_cuda_sdk=auto])



### TBB paths
AC_ARG_WITH([tbb-incdir],
  AC_HELP_STRING(
    [--with-tbb-incdir@<:@=DIR@:>@], 
    [Set TBB include directory @<:@]m4_default([$1], /usr/include)[@:>@]
  ),
  [tbb_incdir=$withval], 
  [tbb_incdir=/usr/include/]
)

AC_ARG_WITH([tbb-libdir],
  AC_HELP_STRING(
    [--with-tbb-libdir@<:@=DIR@:>@], 
    [Set TBB library directory @<:@]m4_default([$1], /usr/lib/tbb)[@:>@]
  ),
  [tbb_libdir=$withval], 
  [tbb_libdir=/usr/lib/]
)



### Set backend flags
AC_MSG_CHECKING([thrust backend])
AC_ARG_WITH(backend,
  AC_HELP_STRING(
    [--with-backend], 
    [Determines the parallel backend for thrust. Options are OPENMP, TBB, and CUDA.]
  ),
  [BACKEND=$withval], 
  [BACKEND="OPENMP"]
)


if test "X${BACKEND}" = "XOPENMP"; then
  AC_MSG_RESULT([Using OpenMP backend])
  AC_OPENMP
  NVCC=""
  NVCCFLAGS=""
  RTH_CPPFLAGS="-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP \$(SHLIB_OPENMP_CXXFLAGS)"
  RTH_LDFLAGS=""
elif test "X${BACKEND}" = "XTBB"; then
  AC_MSG_RESULT([Using Thread Building Blocks backend])
  
  AC_CHECK_HEADERS([${tbb_incdir}/tbb/tbb.h],,AC_MSG_ERROR([tbb.h not found]))
  
  TMP_LDFLAGS=${LDFLAGS}
  LDFLAGS="${LDFLAGS} -L${tbb_libdir}/tbb"
  AC_CHECK_LIB([tbb], [TBB_runtime_interface_version], [], [AC_MSG_ERROR(["Unable to link with Intel TBB])])
  LDFLAGS=${TMP_LDFLAGS}
  
  NVCC=""
  NVCCFLAGS=""
  RTH_CPPFLAGS="-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_BACKEND_TBB -I${tbb_incdir}"
  RTH_LDFLAGS="-L${tbb_libdir}/tbb -ltbb"
elif test "X${BACKEND}" = "XCUDA"; then
  AC_MSG_RESULT([Using CUDA backend])
  NVCCFLAGS="$NVCCFLAGS"
  CUDA_CFLAGS="$CUDA_CFLAGS"
  CUDA_CFLAGS="$CUDA_CFLAGS -I$CUDA_SDK_INSTALL_PATH/C/common/inc/"
  CUDA_CFLAGS="$CUDA_CFLAGS -I$CUDA_INSTALL_PATH/include"
  CUDA_LDFLAGS="-L$CUDA_INSTALL_PATH/lib64"
  CUDA_LIBS="-lcuda -lcudart -lcufft"
  NVCCFLAGS="$NVCCFLAGS $CUDA_CFLAGS $CUDA_LDFLAGS $CUDA_LIBS"
  NVCC="nvcc"
  AC_SUBST(NVCC)
  AC_SUBST(NVCCFLAGS)
  AC_DEFINE([_CUDA],[1],[Defined if CUDA should be used])
  eval "ARRAY${N}='-D_CUDA'"
  N=`expr $N + 1`
  
  RTH_LDFLAGS=""
else
  AC_MSG_ERROR([Unknown backend; use one of OPENMP, TBB, or CUDA.])
fi




dnl Start to substitute templates
AC_SUBST(BACKEND)
AC_SUBST(NVCC)
AC_SUBST(NVCCFLAGS)
AC_SUBST(RTH_CPPFLAGS)
AC_SUBST(RTH_LDFLAGS)
AC_OUTPUT(src/Makevars)


