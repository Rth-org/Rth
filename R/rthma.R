#' Parallel Moving Average
#' 
#' Computes a moving-average vector
#' 
#' Computes a vector like that of \code{runmean()} in \pkg{caTools}, except
#' that only full windows are computed, causing a discrepancy at the endpoints
#' with \code{runmean()}.
#' 
#' @param x a numeric vector
#' @param w window width for the moving average
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return The moving-average vector.  Element i is the average of
#' \code{x[i-w+1]},..., \code{x[i]}.
#' @examples
#' 
#' \dontrun{
#' x <- c(5,2,12,13,8,0,50)
#' rthma(x,3)
#' # should output c(6.333333,9.000000,11.000000,7.000000,19.333333)
#' }
#' 
#' @export
rthma <- function(x, w, nthreads=rth.nthreads())
{
  nthreads <- as.integer(nthreads)
  ret <- .Call("rthma",x,as.integer(w),as.integer(nthreads))
  
  if (is.null(ret))
    return(0)
  else
    return(ret)
}

