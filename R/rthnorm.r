rthnorm <- function(x, p=2, nthreads=rth.nthreads())
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rth_norm", x, as.double(p), as.integer(nthreads), PACKAGE="Rth")
  
  return( ret )
}

