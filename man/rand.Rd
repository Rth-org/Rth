\name{rand}

\alias{rthrunif}
\alias{rthrnorm}

\title{Parallel Random Number Generation}
\description{
  Sorts a numeric vector into ascending or descending order.
}
\usage{
  rthrunif(n, min=0, max=1, seed=rth.getseed(), nthreads=rth.nthreads())
  rthrnorm(n, mean=0, sd=1, seed=rth.getseed(), nthreads=rth.nthreads())
}
\arguments{
  \item{n}{The number of values to generate}
  \item{min,max}{Parameters for uniform random variables.}
  \item{mean,sd}{Parameters for normal random variables.}
  \item{seed}{Seed for the random number generation.}
  \item{nthreads}{An optional argument for specifying the number of
  threads (in the case of OpenMP or TBB backends) that the routine
  should (at most) use.  Does nothing with a CUDA backend.}
}
\details{
  \code{rthrunif()} uses thrust's implementation of the minimal standard 
  random number generation algorithm.  The way seeds for threads are
  chosen at the moment does not guarantee independence of the RNG
  streams.
}
\references{
  \url{https://thrust.github.io/doc/group__predefined__random.html}
}
\examples{
\dontrun{
x <- runif(10)
x
rthsort(x)  # outputs sorted x
x  # but x itself is unchanged
rthsort(x,inplace=TRUE)  # outputs nothing
x  # but x is now a sorted version of itself
}
}
