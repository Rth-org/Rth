rthrunif <- function(n, min=0, max=1, seed=rth.getseed(), nthreads=rth.nthreads())
{
  .Call("rth_runif", as.integer(n), as.double(min), as.double(max), seed=as.integer(seed), as.integer(nthreads), PACKAGE="Rth")
}

