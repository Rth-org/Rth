
rthcolsums <- function(m, nthreads=rth.nthreads()) {
   if (is.data.frame(m)) m <- as.matrix(m)
   .Call("rthcolsums",m, as.integer(nthreads), PACKAGE="Rth")
}


