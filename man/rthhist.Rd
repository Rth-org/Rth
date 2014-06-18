\name{rthhist}
\alias{rthhist}

\title{Parallel Computation of Histograms}
\description{
  Similar to R's \code{hist()}
}
\usage{
  rthhist(xname,nbins=15,plot=TRUE,nthreads=rth.nthreads(),nchunks=nthreads) 
}
\arguments{
  \item{xname}{Name of data vector, quoted.}
  \item{nbins}{The number of bins.}
  \item{plot}{If TRUE, plot will be displayed.}
  \item{nthreads}{An optional argument for specifying the number of
  threads (in the case of OpenMP or TBB backends) that the routine
  should (at most) use.  Does nothing with a CUDA backend.  See
  \link{nthreads}.}
  \item{nchunks}{Number of chunks for partitioning the data.}
}
\details{
  Similar to \code{hist()}, though with fewer options.
}
\value{
   Object of R class \code{"histogram"}.
}
\examples{
\dontrun{
# R built-in geyser data
fw <- faithful$waiting
fwh <- rthhist("fw")
fwh  # display plot, famously bimodal
fwh <- rthhist("fw",plot=FALSE)
fwh
# bin counts, left endpt, binwidth printed out
}
}
