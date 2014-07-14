rthma <- function(x, w, nthreads=rth.nthreads())
{
  nthreads <- as.integer(nthreads)
  .Call("rthma",x,as.integer(w),as.integer(nthreads))
}

