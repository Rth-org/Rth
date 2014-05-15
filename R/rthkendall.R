# author: N. Matloff

rthkendall <- function(x,y)
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  if (!is.double(y))
    storage.mode(y) <- "double"
  
  ret <- .Call("rthkendall", x, y, PACKAGE="Rth")
  
  return(ret)
}

