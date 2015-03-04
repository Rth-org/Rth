#' Parallel Computation of Distances
#' 
#' Similar to R's \code{dist()} and \code{pdist()}.
#' 
#' Calling \code{rthdist()} on the matrix \code{m} returns the euclidean
#' distances between all pairs of rows in \code{m}.
#' 
#' The call \code{rthpdist(m1,m2)} returns the distance from all rows of
#' \code{m1} to all in \code{m2}.
#' 
#' @aliases rthdist rthpdist
#' @param m Matrix/data frame for between-rows distance computation.
#' @param m1 First matrix/data frame for between-matrices distance computation.
#' @param m2 Second matrix/data frame for between-matrices distance
#' computation.
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return Object of R class \code{"matrix"}.  Duplicate computation is avoided
#' in \code{rthdist()} but the entire matrix is constructed.
#' @examples
#' 
#' \dontrun{
#' library(Rth)
#' m1 <- rbind(1:2,3:4)
#' m2 <- matrix(c(1,0,1,6),ncol=2)
#' m1
#' m2
#' rthdist(m1)
#' rthpdist(m1,m2)
#' }
#' 
#' @export
rthdist <- function(m, nthreads=rth.nthreads()) {
   if (is.data.frame(m)) m <- as.matrix(m)
   .Call("rthdist",m, as.integer(nthreads), PACKAGE="Rth")
}


