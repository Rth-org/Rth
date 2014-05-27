### slow, don't use
rthmean <- function(x)
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rthmean", x)
  
  return( ret )
}



rthgini <- function(x, unbiased=TRUE, na.rm=FALSE)
{
  if (na.rm)
  {
    x <- na.omit(x)
    attributes(x) <- NULL
  }
  
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  mu <- mean(x)
  
  ret <- .Call("rthgini", x, mu, as.integer(unbiased))
  
  return( ret )
}
