rthsort <- function(x, decreasing=FALSE, inplace=FALSE, nthreads=rth.nthreads())
{
  nthreads <- as.integer(nthreads)
  
  if (is.integer(x)) {
     out <- .Call("rthsort_int", x, as.integer(decreasing), 
               as.integer(inplace), as.integer(nthreads), PACKAGE="Rth")
  }
  else
  {
    if (!is.double(x))
      storage.mode(x) <- "double"
    out <- .Call("rthsort_double", x, as.integer(decreasing), 
              as.integer(inplace), as.integer(nthreads), PACKAGE="Rth")
  }

  if (inplace) return(invisible(0)) else return(out)
}

