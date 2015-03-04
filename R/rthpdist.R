#' @export
rthpdist <- function(m1,m2, nthreads=rth.nthreads()) {
   if(rth.get.backend() == "tbb")
      stop("this function does not yet work under a TBB backend")

   if (is.data.frame(m1)) m1 <- as.matrix(m1)
   if (is.data.frame(m2)) m2 <- as.matrix(m2)
   .Call("rthpdist", m1, m2, as.integer(nthreads), PACKAGE="Rth")
}


