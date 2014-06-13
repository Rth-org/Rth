
rthcolsums <- function(m, nthreads=automatic()) {
   if (is.data.frame(m)) m <- as.matrix(m)
   .Call("rthcolsums",m, as.integer(nthreads), PACKAGE="Rth")
}


