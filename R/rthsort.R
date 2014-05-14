rthsort <- function(x)
{
  if (is.integer(x))
    .Call("rthsort_int", x, PACKAGE="rth")
  else
  {
    if (!is.double(x))
      storage.mode(x) <- "double"
      
      .Call("rthsort_double", x, PACKAGE="rth")
  }
}

