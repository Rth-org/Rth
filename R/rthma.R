rthma <- function(x, w, nthreads=rth.nthreads())
{
  nthreads <- as.integer(nthreads)
  ret <- .Call("rthma",x,as.integer(w),as.integer(nthreads))
  
  if (is.null(ret))
    return(0)
  else
    return(ret)
}

