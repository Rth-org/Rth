rth.get.backend <- function()
{
  .Call("Rth_get_backend", PACKAGE="Rth")
}


### FIXME doesn't work with Rth, probably works with BLAS ???
omp.set.num.threads <- function(n)
{
  .Call("Rth_omp_set_num_threads", as.integer(n), PACKAGE="Rth")
  
  invisible()
}

### this is stupid
omp.get.num.threads <- function()
{
  nthreads <- .Call("Rth_omp_get_num_threads", PACKAGE="Rth")
  
  return( nthreads )
}


automatic <- function()
{
  backend <- rth.get.backend()
  if (backend == "omp")
  {
    nthreads <- parallel::detectCores()
#    omp.set.num.threads(nthreads)
    return( nthreads )
  }
  else if (backend == "tbb")
    return( -1L )
  else
    return( as.integer(NA) )
}



