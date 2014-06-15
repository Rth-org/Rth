rthgini <- function(x, unbiased=TRUE, na.rm=FALSE, nthreads=rth.nthreads())
{
  if (na.rm)
  {
    x <- na.omit(x)
    attributes(x) <- NULL
  }
  
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  mu <- mean(x)
  
  ret <- .Call("rthgini", x, mu, as.integer(unbiased), as.integer(nthreads), PACKAGE="Rth")
  
  return( ret )
}
