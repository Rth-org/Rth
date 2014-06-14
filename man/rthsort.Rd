\name{rthsort}
\alias{rthsort}

\title{Parallel Sort}
\description{
  Sorts a numeric vector into ascending or descending order.
}
\usage{
  rthsort(x, descending=FALSE, inplace=FALSE, nthreads=automatic())
}
\arguments{
  \item{x}{a numeric vector}
  \item{descending}{logical; determines if the sort is ascending
  (\code{ascending=TRUE}) or descending (\code{decreasing=TRUE}).}
  \item{inplace}{If TRUE, \code{x} is sorted in-place, no new R vectory
  created.}
  \item{nthreads}{An optional argument for specifying the number of
  threads (in the case of OpenMP or TBB backends) that the routine
  should (at most) use.  Does nothing with a CUDA backend.}
}
\details{
  The implementation uses \code{thrust::sort}, which does not
  necessarily preserve the order of tied values.
}
\value{
  If \code{inplace} is FALSE:
  If \code{decreasing=FALSE}, then the input vector is returned in
  ascending order.  Otherwise, the input vector is returned in 
  descending order.

  If \code{inplace} is TRUE, there is no return value.
}
\references{
  \url{https://docs.thrust.googlecode.com/hg/group__sorting.html}
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
