rthsort <- function(x, nthreads=automatic())
{
  nthreads <- as.integer(nthreads)
  
  if (is.integer(x))
    .Call("rthsort_int", x, as.integer(nthreads), PACKAGE="Rth")
  else
  {
    if (!is.double(x))
      storage.mode(x) <- "double"
      
      .Call("rthsort_double", x, as.integer(nthreads), PACKAGE="Rth")
  }
}

