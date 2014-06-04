rth.get.backend <- function()
{
  .Call("Rth_get_backend", PACKAGE="Rth")
}



omp.get.num.threads <- function()
{
  nthreads <- .Call("Rth_omp_get_num_threads", PACKAGE="Rth")
  
  return( nthreads )
}



tbb.get.num.threads <- function()
{
  nthreads <- .Call("Rth_tbb_auto_threads", PACKAGE="Rth")
  
  return( nthreads )
}



automatic <- function()
{
  backend <- rth.get.backend()
  if (backend == "omp")
  {
    nthreads <- omp.get.num.threads()
    return( nthreads )
  }
  else if (backend == "tbb")
  {
    nthreads <- tbb.get.num.threads()
    return( nthreads )
  }
  else
    return( as.integer(NA) )
}



