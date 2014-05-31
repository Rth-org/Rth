### slow, don't use
rthmean <- function(x, nthreads=automatic())
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rthmean", x, as.integer(nthreads), PACKAGE="Rth")
  
  return( ret )
}
