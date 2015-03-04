rth.get.backend <- function()
{
  .Call("Rth_get_backend", PACKAGE="Rth")
}




get.num.threads <- function()
{
  nthreads <- .Call("Rth_get_num_threads", PACKAGE="Rth")
  
  return( nthreads )
}


#' Determining the Number of Threads a Routine Uses
#' 
#' Note, this only applies for OpenMP and Intel Thread Building Blocks
#' backends.
#' 
#' \code{rth.nthreads()} will determine the maximum number of threads to use by
#' checking against the environment variable \code{RTH_NUM_THREADS} (and in the
#' case of OpenMP, \code{OMP_NUM_THREADS}), and failing that, the default for
#' the given backend.  For OpenMP this means that the number of cores
#' (physical+logical) will be returned.  For Intel Thread Building Blocks, the
#' default is the value \code{-1}.
#' 
#' For any function taking \code{nthreads=} as an argument, the priority is
#' (highest to lowest): \tabular{ll}{ nthreads argument in the R-level Rth
#' function \cr RTH_NUM_THREADS environment variable \cr OMP_NUM_THREADS
#' environment variable (for OpenMP backends) \cr Number of cores }
#' 
#' @aliases nthreads rth.nthreads
#' @export
rth.nthreads <- function()
{
  backend <- rth.get.backend()
  nthreads <- as.integer(get.num.threads())
  
  return( nthreads )
}

