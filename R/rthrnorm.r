rthrnorm <- function(n, mean=0, sd=1, seed=rth.getseed(), nthreads=automatic())
{
  .Call("rth_rnorm", as.double(n), as.double(mean), as.double(sd), seed=as.integer(seed), as.integer(nthreads), PACKAGE="Rth")
}
