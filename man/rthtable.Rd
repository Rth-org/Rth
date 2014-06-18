\name{rthtable, arylin2mult}
\alias{rthtable}
\alias{arylin2mult}

\title{Parallel Computation of Contingency Tables}
\description{
  Similar to R's \code{table}, but with important differences.
}
\usage{
  rthtable(m, lb, ub, varnames = NULL, dnn = NULL, 
     nthreads = rth.nthreads(), nch = nthreads) 
  arylin2mult(lins, lb, ub) 
}
\arguments{
  \item{m}{Data matrix, one row per observation.}
  \item{lb}{Vector of lower bounds on the variables.}
  \item{ub}{Vector of upper bounds on the variables.}
  \item{varnames}{Character vector of names of the variables.}
  \item{dnn}{List of names of the levels of the variables.}
  \item{nthreads}{An optional argument for specifying the number of
  threads (in the case of OpenMP or TBB backends) that the routine
  should (at most) use.  Does nothing with a CUDA backend.  See
  \link{nthreads}.}
  \item{nch}{Number of chunks for partitioning the data.}
  \item{lins}{Linear indices to convert to multidimensional ones.}
}
\details{
  The function \code{rthtable()} is similar to R's \code{table()}.  
  It allows more cells than \code{table()}, and is much faster.  
  However, unlike \code{table()}, here users must specify the ranges
  of the variables in advance.  

  The function \code{arylin2mult()} is handy for tables of high
  dimension.  For example, one may be interested in searching for
  outliers, and thus consider cells of small sizes, say less than 5.
  We can apply \code{which()} to the table, then use
  \code{arylin2mult()} to convert the resulting linear indices to
  multidimensional ones.
}
\value{
   The function \code{rthtable()} returns an object of R class \code{table}.

   The function \code{arylin2mult()} returns a matrix of
   multidimensional indices, one row for each element of \code{lins}.
}
\examples{
\dontrun{
library(MASS)
pm <- Pima.te  
# cut diabetic pedigree, age into 3 ranges
pm$ped1 <- cut(pm$ped,3,1:3)
pm$age1 <- cut(pm$age,3,1:3)
# for diabetes, recode Yes/No at 1/0
pm$type1 <- as.integer(pm$type == 'Yes')
# names of the levels
dnn <- list(c("low risk","med risk","high risk"),c("young","middle age",
   "senior"),c("no","yes"))
tbl <- rthtable(pm[,9:11],c(1,1,0),c(3,3,1),dnn=dnn)
tbl  # display the table

# which cells are rate (clear visually here, less so with many vars)
tbli <- as.integer(tbl)  # pure cell counts
arylin2mult(which(tbli < 5),c(1,1,0),c(3,3,1))
# e.g. output shows one small cell is (3,1,0), i.e. high risk/young/no

}
}
