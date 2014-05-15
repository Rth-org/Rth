rthnorm <- function(x, p=2)
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rth_norm", x, as.double(p))
  
  return( ret )
}


rthdist <- function(x, y, p=2)
{
  if (length(x) != length(y))
    stop("the lengths of 'x' and 'y' must agree")
  
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  if (!is.double(y))
    storage.mode(y) <- "double"
  
  ret <- .Call("rth_dist", x, y, as.double(p))
  
  return( ret )
}



