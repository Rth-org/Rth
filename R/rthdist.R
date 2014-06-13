
rthdist <- function(m, nthreads=automatic()) {
   if (is.data.frame(m)) m <- as.matrix(m)
   .Call("rthdist",m, as.integer(nthreads), PACKAGE="Rth")
}


