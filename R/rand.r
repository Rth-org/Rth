rth.getseed <- function()
{
  date <- as.integer(Sys.Date())
  time <- as.integer(Sys.time())
  pid <- as.integer(Sys.getpid())
  
  ret <- hash96(time, pid, date)
  ret <- as.integer(ret)
  
  return( ret )
}



rthrunif <- function(n, min=0, max=1, seed=rth.getseed())
{
  .Call("rth_runif", as.integer(n), as.double(min), as.double(max), seed=as.integer(seed), PACKAGE="Rth")
}



rthrnorm <- function(n, mean=0, sd=1, seed=rth.getseed())
{
  .Call("rth_rnorm", as.integer(n), as.double(mean), as.double(sd), seed=as.integer(seed), PACKAGE="Rth")
}
