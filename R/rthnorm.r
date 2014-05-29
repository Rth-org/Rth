rthnorm <- function(x, p=2)
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rth_norm", x, as.double(p), PACKAGE="Rth")
  
  return( ret )
}

