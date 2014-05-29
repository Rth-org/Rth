rthrnorm <- function(n, mean=0, sd=1, seed=rth.getseed())
{
  .Call("rth_rnorm", as.integer(n), as.double(mean), as.double(sd), seed=as.integer(seed), PACKAGE="Rth")
}
