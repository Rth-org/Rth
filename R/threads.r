rth.get.backend <- function()
{
  .Call("Rth_get_backend", PACKAGE="Rth")
}




get.num.threads <- function()
{
  nthreads <- .Call("Rth_get_num_threads", PACKAGE="Rth")
  
  return( nthreads )
}



automatic <- function()
{
  backend <- rth.get.backend()
  nthreads <- get.num.threads()
  
  return( nthreads )
}



