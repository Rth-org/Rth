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
  \item{plot}{If TRUE, plot will be displayed.}
  \item{nthreads}{See \link{rthsort}.}
  \item{nch}{Number of chunks for partitioning the data.}
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
