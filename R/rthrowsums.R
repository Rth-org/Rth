
rthrowsums <- function(m, nthreads=automatic()) {
   .Call("rthrowsums",m, as.integer(nthreads), PACKAGE="Rth")
}


