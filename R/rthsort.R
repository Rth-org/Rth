#' Parallel Sort
#' 
#' Sorts a numeric vector into ascending or descending order.
#' 
#' The implementation uses \code{thrust::sort}, which does not necessarily
#' preserve the order of tied values.
#' 
#' @param x a numeric vector
#' @param decreasing logical; determines if the sort is ascending
#' (\code{decreasing=FALSE}) or descending (\code{decreasing=TRUE}).
#' @param inplace If TRUE, \code{x} is sorted in-place, no new R vectory
#' created.
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return If \code{decreasing=FALSE}, then the input vector is returned in
#' ascending order.  Otherwise, the input vector is returned in descending
#' order.
#' 
#' If \code{inplace} is TRUE, there is no return value.
#' @references \url{https://docs.thrust.googlecode.com/hg/group__sorting.html}
#' @examples
#' 
#' \dontrun{
#' x <- runif(10)
#' x
#' rthsort(x)  # outputs sorted x
#' x  # but x itself is unchanged
#' rthsort(x,inplace=TRUE)  # outputs nothing
#' x  # but x is now a sorted version of itself
#' }
#' 
#' @export
rthsort <- function(x, decreasing=FALSE, inplace=FALSE, nthreads=rth.nthreads())
{
  nthreads <- as.integer(nthreads)
  
  if (is.integer(x)) {
     out <- .Call("rthsort_int", x, as.integer(decreasing), 
               as.integer(inplace), as.integer(nthreads), PACKAGE="Rth")
  }
  else
  {
    if (!is.double(x))
      storage.mode(x) <- "double"
    out <- .Call("rthsort_double", x, as.integer(decreasing), 
              as.integer(inplace), as.integer(nthreads), PACKAGE="Rth")
  }

  if (inplace) return(invisible(0)) else return(out)
}

