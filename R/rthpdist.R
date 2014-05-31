
rthpdist <- function(m1,m2, nthreads=automatic()) {
   .Call("rthpdist", m1, m2, as.integer(nthreads), PACKAGE="Rth")
}


