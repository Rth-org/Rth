rthnorm <- function(x, p=2, nthreads=automatic())
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rth_norm", x, as.double(p), as.integer(nthreads), PACKAGE="Rth")
  
  return( ret )
}

