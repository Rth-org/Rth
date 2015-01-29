## autoconf configure.ac && autoreconf -if

################################################################################
# This file is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or distribute 
# this file, for any purpose, and by any means.
#
# In jurisdictions that recognize copyright laws, the author or authors of this 
# file dedicate any and all copyright interest in the file to the public
# domain. We make this dedication for the benefit of the public at large and to 
# the detriment of our heirs and successors. We intend this dedication to be an 
# overt act of relinquishment in perpetuity of all present and future rights to 
# this file under copyright law.
#
# THE FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
# WITH THIS FILE OR THE USE OR OTHER DEALINGS IN THIS FILE.
#
# For more information, please refer to http://unlicense.org/
################################################################################


AC_INIT(DESCRIPTION)

### CUDA paths
AC_ARG_WITH([cuda-home],
  AS_HELP_STRING(
    [--with-cuda-home@<:@=DIR@:>@],
    [Set CUDA install dir @<:@]m4_default(/usr/local/cuda/)[@:>@]
  ),
  [cuda_home=$withval], 
  [cuda_home=/usr/local/cuda/]
)


### TBB paths
AC_ARG_WITH([tbb-incdir],
  AC_HELP_STRING(
    [--with-tbb-incdir@<:@=DIR@:>@], 
    [Set TBB include directory @<:@]m4_default(/usr/include)[@:>@]
  ),
  [tbb_incdir=$withval], 
  [tbb_incdir=/usr/include/]
)

AC_ARG_WITH([tbb-libdir],
  AC_HELP_STRING(
    [--with-tbb-libdir@<:@=DIR@:>@], 
    [Set TBB library directory @<:@]m4_default(/usr/lib/tbb)[@:>@]
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
  AC_PROG_CXX
  AC_LANG([C++])
  
  AC_OPENMP
  if test "X$ac_cv_prog_cxx_openmp" = Xunsupported; then
    AC_MSG_WARN([OpenMP is not supported by your compiler; very strange things may happen during compile time!])
  fi
  
  RTH_CPPFLAGS="-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_OMP \$(SHLIB_OPENMP_CXXFLAGS)"
  RTH_LDFLAGS=""
elif test "X${BACKEND}" = "XTBB"; then
  AC_MSG_RESULT([Using Thread Building Blocks backend])
  AC_PROG_CXX
  AC_LANG([C++])
  
  AC_CHECK_HEADERS([${tbb_incdir}/tbb/tbb.h],,AC_MSG_ERROR([tbb.h not found]))
  
  TMP_LDFLAGS=${LDFLAGS}
  LDFLAGS="${LDFLAGS} -L${tbb_libdir}/tbb"
  AC_CHECK_LIB([tbb], [TBB_runtime_interface_version], [], [AC_MSG_ERROR(["Unable to link with Intel TBB])])
  LDFLAGS=${TMP_LDFLAGS}
  
  RTH_CPPFLAGS="-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_BACKEND_TBB -I${tbb_incdir}"
  RTH_LDFLAGS="-L${tbb_libdir}/tbb -ltbb"
elif test "X${BACKEND}" = "XCUDA"; then
  AC_MSG_RESULT([Using CUDA backend])
  AC_PROG_CXX([${cuda_home}/bin/nvcc])
  AC_LANG([C++])
  
  NVCC="${cuda_home}/bin/nvcc -gencode arch=compute_13,code=sm_13"
  
  RTH_CPPFLAGS="-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_BACKEND_CUDA"
  RTH_LDFLAGS="-L${cuda_home}/lib/ -lcuda -lcudart"
  
  CUDA_INCS="-I\"${cuda_home}/include\" -I\"${R_INCLUDE_DIR}\" -I\"../inst/include/\""
else
  AC_MSG_ERROR([Unknown backend; use one of OPENMP, TBB, or CUDA.])
fi


dnl Start to substitute templates
AC_SUBST(BACKEND)
AC_SUBST(RTH_CPPFLAGS)
AC_SUBST(RTH_LDFLAGS)
AC_SUBST(NVCC)
AC_SUBST(CUDA_INCS)
AC_OUTPUT(src/Makevars)


