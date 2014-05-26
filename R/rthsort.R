rthsort <- function(x)
{
  if (is.integer(x))
    .Call("rthsort_int", x, PACKAGE="Rth")
  else
  {
    if (!is.double(x))
      storage.mode(x) <- "double"
      
      .Call("rthsort_double", x, PACKAGE="Rth")
  }
}

