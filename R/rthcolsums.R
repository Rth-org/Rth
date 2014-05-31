
rthcolsums <- function(m, nthreads=automatic()) {
   .Call("rthcolsums",m, as.integer(nthreads), PACKAGE="Rth")
}


