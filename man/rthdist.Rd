\name{rthdist,rthpdist}
\alias{rthdist}
\alias{rthpdist}

\title{Parallel Computation of Distances}
\description{
  Similar to R's \code{dist()} and \code{pdist()}.
}
\usage{
  rthdist(m, nthreads=rth.nthreads()) 
  rthpdist(m1,m2, nthreads=rth.nthreads())
}
\arguments{
  \item{m}{Matrix/data frame for between-rows distance computation.}
  \item{m1}{First matrix/data frame for between-matrices distance computation.}
  \item{m2}{Second matrix/data frame for between-matrices distance computation.}
  \item{nthreads}{See \link{rthsort}.}
}
\details{
  Calling \code{rthdist()} on the matrix \code{m} returns the euclidean
  distances between all pairs of rows in \code{m}.

  The call \code{rthpdist(m1,m2)} returns the distance from all rows of
  \code{m1} to all in \code{m2}.
}
\value{
   Object of R class \code{"matrix"}.  Duplicate computation is avoided
   in \code{rthdist()} but the entire matrix is constructed.
}
\examples{
\dontrun{
library(Rth)
m1 <- rbind(1:2,3:4)
m2 <- matrix(c(1,0,1,6),ncol=2)
m1
m2
rthdist(m1)
rthpdist(m1,m2)
}
}
