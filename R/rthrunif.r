#' Parallel Random Uniform Generation
#' 
#' Generate from a uniform distribution in parallel.
#' 
#' \code{rthrunif()} uses thrust's implementation of the minimal standard
#' random number generation algorithm.  The way seeds for threads are chosen at
#' the moment does not guarantee independence of the RNG streams.
#' 
#' @aliases rthrunif
#' @param n 
#' The number of values to generate
#' @param min,max 
#' Parameters for uniform random variables.
#' @param seed 
#' Seed for the random number generation.
#' @param nthreads 
#' An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' 
#' @references
#' \url{https://thrust.github.io/doc/group__predefined__random.html}
#' 
#' @export
rthrunif <- function(n, min=0, max=1, seed=rth.getseed(), nthreads=rth.nthreads())
{
  .Call("rth_runif", as.integer(n), as.double(min), as.double(max), seed=as.integer(seed), as.integer(nthreads), PACKAGE="Rth")
}

