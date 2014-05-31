rthrunif <- function(n, min=0, max=1, seed=rth.getseed(), nthreads=automatic())
{
  .Call("rth_runif", as.double(n), as.double(min), as.double(max), seed=as.integer(seed), as.integer(nthreads), PACKAGE="Rth")
}

