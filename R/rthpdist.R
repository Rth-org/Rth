
rthpdist <- function(m1,m2, nthreads=automatic()) {
   if(rth.get.backend() == "tbb")
      stop("this function does not yet work under TBB")

   .Call("rthpdist", m1, m2, as.integer(nthreads), PACKAGE="Rth")
}


