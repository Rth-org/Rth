
rthdist <- function(m, nthreads=automatic()) {
   .Call("rthdist",m, as.integer(nthreads), PACKAGE="Rth")
}


