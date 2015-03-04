#' Parallel Order and Rank
#' 
#' Similar to R's \code{order()} and \code{rank()}.
#' 
#' 
#' @param x A numeric vector.
#' @param rnk Logical; if TRUE, the result is like a call to R's \code{rank()};
#' if FALSE, it is like R's \code{order()}.
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return Vector of indices for either order or rank operation..
#' @examples
#' 
#' \dontrun{
#' x <- c(1.2,0.3,0.8,2.8)
#' order(x)  # prints 2,3,1,4
#' rthorder(x)  # same
#' rank(x)  # prints 3,1,2,4
#' rthorder(x,rnk=TRUE)  # same
#' }
#' 
#' @export
rthorder <- function(x, rnk=FALSE, nthreads=rth.nthreads()) {
   if (!is.double(x))
      storage.mode(x) <- "double"
   
   ret <- .Call("rthorder", x, rnk, as.integer(nthreads), PACKAGE="Rth")
   
   # convert from C's 0-based indexing to R's 1-based
   return(ret + 1L)
}


