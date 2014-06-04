\name{rthsort}
\alias{rthsort}

\title{Parallel Sort}
\description{
  Sorts a numeric vector into ascending or descending order.
}
\usage{
  rthsort(x, descending=FALSE, nthreads=automatic())
}
\arguments{
  \item{x}{a numeric vector}
  \item{descending}{logical; determines if the sort is ascending
  (\code{ascending=TRUE}) or descending (\code{decreasing=TRUE}).}
  \item{nthreads}{An optional argument for specifying the number of
  threads (in the case of OpenMP or TBB backends) that the routine
  should (at most) use.  Does nothing with a CUDA backend.}
}
\details{
  The implementation uses \code{thrust::sort}, which is not guaranteed
  to be stable.
}
\value{
  If \code{decreasing=FALSE}, then the input vector is returned in
  ascending order.  Otherwise, the input vector is returned in 
  descending order.
}
\references{
  \url{https://docs.thrust.googlecode.com/hg/group__sorting.html}
}
\examples{
\dontrun{
library(Rth)

x <- sample(1:10)
x
rthsort(x)
rthsort(x, decreasing=TRUE)
}
}
