#' Parallel Random Normal Generation
#' 
#' Generate from a normal distribution in parallel.
#' 
#' @aliases rthrnorm
#' @param n 
#' The number of values to generate
#' @param mean,sd 
#' Parameters for normal random variables.
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
rthrnorm <- function(n, mean=0, sd=1, seed=rth.getseed(), nthreads=rth.nthreads())
{
  .Call("rth_rnorm", as.integer(n), as.double(mean), as.double(sd), seed=as.integer(seed), as.integer(nthreads), PACKAGE="Rth")
}
