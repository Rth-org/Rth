\name{nthreads}
\alias{automatic}

\title{Determining the Number of Threads a Routine Uses}
\description{
  Note, this only applies for OpenMP and Intel Thread Building Blocks
  backends.
}
\usage{
  automatic()
}
\details{
  PRIORITY (highest to lowest):
    nthreads argument in the R-level Rth function
    RTH_NUM_THREADS environment variable
    OMP_NUM_THREADS environment variable (for OpenMP backends)
    Number of cores
}
\value{
  .
}
\references{
  \url{https://docs.thrust.googlecode.com/hg/group__sorting.html}
}
