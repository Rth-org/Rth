
rthpdist <- function(m1,m2, nthreads=automatic()) {
   res <- .Call("rthpdist", m1, m2, as.integer(nthreads), PACKAGE="Rth")
   if (res == -1) stop("this function does not yet work under TBB")
   res
}


