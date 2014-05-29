### slow, don't use
rthmean <- function(x)
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rthmean", x, PACKAGE="Rth")
  
  return( ret )
}
