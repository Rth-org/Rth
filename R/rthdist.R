
rthdist <- function(m, nthreads=rth.nthreads()) {
   if (is.data.frame(m)) m <- as.matrix(m)
   .Call("rthdist",m, as.integer(nthreads), PACKAGE="Rth")
}


