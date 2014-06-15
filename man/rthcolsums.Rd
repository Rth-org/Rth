\name{rthcolsums}
\alias{rthcolsums}

\title{
Matrix column sums.
}

\description{

For the given input matrix, returns the vector of column sums.
}

\usage{
rthcolsums(m, nthreads=rth.nthreads()) 
}

\arguments{
   \item{m}{The data, in data frame or matrix form.}  
   \item{nthreads}{Number of threads, in the case of OpenMP or TBB
      backends.  The authors of TBB recommend that normally the user
      should allow TBB itself to determine this, and even if used, it
      will be interpreted as the maximum number of threads.}
}

\value{
Vector of column sums.}
}

\author{
Norm Matloff <matloff@cs.ucdavis.edu> and Drew Schmidt
<schmidt@math.utk.edu>
}

% }

% \seealso{
% }

\examples{
\dontrun{

# "attitude" data set, built-in to R
rthcolsums(attitude)
# should print 1939 1998...
}

}


