# author: N. Matloff

rthorder <- function(x, rnk=FALSE, nthreads=rth.nthreads()) {
   if (!is.double(x))
      storage.mode(x) <- "double"
   
   ret <- .Call("rthorder", x, rnk, as.integer(nthreads), PACKAGE="Rth")
   
   # convert from C's 0-based indexing to R's 1-based
   return(ret + 1L)
}


