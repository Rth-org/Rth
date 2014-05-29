rthrunif <- function(n, min=0, max=1, seed=rth.getseed())
{
  .Call("rth_runif", as.integer(n), as.double(min), as.double(max), seed=as.integer(seed), PACKAGE="Rth")
}

