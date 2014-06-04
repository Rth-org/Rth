\name{nthreads}
\alias{automatic}

\title{Determining the Number of Threads a Routine Uses}
\description{
  Note, this only applies for OpenMP and Intel Thread Building Blocks
  backends.
  
  Priority (highest to lowest):
\tabular{ll}{
  nthreads argument in the R-level Rth function \cr
  RTH_NUM_THREADS environment variable \cr
  OMP_NUM_THREADS environment variable (for OpenMP backends) \cr
  Number of cores
}
  .
}
\usage{
  automatic()
}
\details{
  .
}
\value{
  .
}
\references{
  \url{https://docs.thrust.googlecode.com/hg/group__sorting.html}
}
