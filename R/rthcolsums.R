#' Matrix column sums.
#' 
#' 
#' For the given input matrix, returns the vector of column sums.
#' 
#' 
#' @param m The data, in data frame or matrix form.
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return Vector of column sums.
#' @author Norm Matloff <matloff@@cs.ucdavis.edu> and Drew Schmidt
#' <schmidt@@math.utk.edu>
#' @examples
#' 
#' \dontrun{
#' 
#' # "attitude" data set, built-in to R
#' rthcolsums(attitude)
#' # should print 1939 1998...
#' }
#' 
#' @export
rthcolsums <- function(m, nthreads=rth.nthreads()) {
   if (is.data.frame(m)) m <- as.matrix(m)
   .Call("rthcolsums",m, as.integer(nthreads), PACKAGE="Rth")
}


