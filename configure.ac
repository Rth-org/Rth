## autoconf configure.ac && autoreconf -if

AC_INIT(DESCRIPTION)

CUDA_INSTALL_PATH="/usr/local/cuda"
CUDA_SDK_INSTALL_PATH="/usr/local/NVIDIA_GPU_Computing_SDK"

AC_MSG_CHECKING([whether to use GPU])
AC_ARG_ENABLE(gpu,
  AC_HELP_STRING([--enable-gpu], [Build with nvcc for gpu; default is no, in which case OpenMP is used.]),
  [USE_GPU="yes"], [USE_GPU="no"])

if test "$USE_GPU" == "no"; then
  AC_MSG_RESULT([Using OpenMP])
else
  AC_MSG_RESULT([Using CUDA])
fi

# Setup custom CUDA paths
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

# Setup nvcc flags
if test  "$USE_GPU" = "yes"; then
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
fi



dnl Start to substitute templates
AC_SUBST(USE_GPU)
AC_SUBST(NVCC)
AC_SUBST(NVCCFLAGS)
AC_OUTPUT(src/Makevars)

