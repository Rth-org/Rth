# author: N. Matloff

rthkendall <- function(x,y, nthreads=automatic())
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  if (!is.double(y))
    storage.mode(y) <- "double"
  
  ret <- .Call("rthkendall", x, y, as.integer(nthreads), PACKAGE="Rth")
  
  return(ret)
}

