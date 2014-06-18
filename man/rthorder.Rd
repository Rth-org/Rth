\name{rthorder}
\alias{rthorder}

\title{Parallel Order and Rank}
\description{
  Similar to R's \code{order()} and \code{rank()}.
}
\usage{
  rthorder(x, rnk=FALSE, nthreads=rth.nthreads())
}
\arguments{
  \item{x}{A numeric vector.}
  \item{rnk}{Logical; if TRUE, the result is like a call to R's
  \code{rank()}; if FALSE, it is like R's \code{order()}.}
  \item{nthreads}{See \link{rsort}.}
}
\value{
  Vector of indices for either order or rank operation..
}
}
\examples{
\dontrun{
x <- c(1.2,0.3,0.8,2.8)
order(x)  # prints 2,3,1,4
rthorder(x)  # same
rank(x)  # prints 3,1,2,4
rthorder(x,rnk=TRUE)  # same
}
}
