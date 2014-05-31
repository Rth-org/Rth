rthsort <- function(x, nthreads=automatic())
{
  nthreads <- as.integer(nthreads)
  
  if (is.integer(x))
    .Call("rthsort_int", x, nthreads, PACKAGE="Rth")
  else
  {
    if (!is.double(x))
      storage.mode(x) <- "double"
      
      .Call("rthsort_double", x, nthreads, PACKAGE="Rth")
  }
}

