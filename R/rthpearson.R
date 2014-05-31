# author: N. Matloff

rthpearson <- function(x,y, nthreads=automatic()) {
   if (!is.double(x))
      storage.mode(x) <- "double"
   if (!is.double(y))
      storage.mode(y) <- "double"
   
   .Call("rthpearson", x, y, as.integer(nthreads), PACKAGE="Rth")
}

