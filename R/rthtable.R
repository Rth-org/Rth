# m:  data matrix, 1 observation per row
# lb:  vector of lower bds on the variables
# ub:  vector of upper bds on the variables
# varnames:  names of each dimension
# dnn:  list of names of elements of dimension
# nch:  number of chunks of computation
# nthreads:  number of threads (OMP, TBB cases)

#' Parallel Computation of Contingency Tables
#' 
#' Similar to R's \code{table}, but with important differences.
#' 
#' The function \code{rthtable()} is similar to R's \code{table()}.  It allows
#' more cells than \code{table()}, and is much faster.  However, unlike
#' \code{table()}, here users must specify the ranges of the variables in
#' advance.
#' 
#' The function \code{arylin2mult()} is handy for tables of high dimension.
#' For example, one may be interested in searching for outliers, and thus
#' consider cells of small sizes, say less than 5.  We can apply \code{which()}
#' to the table, then use \code{arylin2mult()} to convert the resulting linear
#' indices to multidimensional ones.
#' 
#' @aliases rthtable arylin2mult
#' @param m Data matrix, one row per observation.
#' @param lb Vector of lower bounds on the variables.
#' @param ub Vector of upper bounds on the variables.
#' @param varnames Character vector of names of the variables.
#' @param dnn List of names of the levels of the variables.
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @param nch Number of chunks for partitioning the data.
#' @param lins Linear indices to convert to multidimensional ones.
#' @return The function \code{rthtable()} returns an object of R class
#' \code{table}.
#' 
#' The function \code{arylin2mult()} returns a matrix of multidimensional
#' indices, one row for each element of \code{lins}.
#' @examples
#' 
#' \dontrun{
#' library(MASS)
#' pm <- Pima.te  
#' # cut diabetic pedigree, age into 3 ranges
#' pm$ped1 <- cut(pm$ped,3,1:3)
#' pm$age1 <- cut(pm$age,3,1:3)
#' # for diabetes, recode Yes/No at 1/0
#' pm$type1 <- as.integer(pm$type == 'Yes')
#' # names of the levels
#' dnn <- list(c("low risk","med risk","high risk"),c("young","middle age",
#'    "senior"),c("no","yes"))
#' tbl <- rthtable(pm[,9:11],c(1,1,0),c(3,3,1),dnn=dnn)
#' tbl  # display the table
#' 
#' # which cells are rate (clear visually here, less so with many vars)
#' tbli <- as.integer(tbl)  # pure cell counts
#' arylin2mult(which(tbli < 5),c(1,1,0),c(3,3,1))
#' # e.g. output shows one small cell is (3,1,0), i.e. high risk/young/no
#' 
#' }
#' 
#' @export
rthtable <- function(m,lb,ub,varnames=NULL,dnn=NULL,
   nthreads=rth.nthreads(),nch=nthreads)
{
   if(rth.get.backend() == "tbb")
      stop("this function does not yet work under TBB")

   if (is.data.frame(m)) m <- as.matrix(m)
   if (is.vector(m)) m <- matrix(m,ncol=1)
   nv <- as.integer(ncol(m))

   if (length(lb) == 1) lb <- rep(lb,nv)
   if (length(ub) == 1) ub <- rep(ub,nv)

   dim <- ub - lb + 1
   
   if (!is.integer(m))
     storage.mode(m) <- "integer"
   
   freq <- .Call("rthtable",m,lb,ub,as.integer(nch),as.integer(nthreads))
   
   # dimension labels
   if (is.null(dnn)) {
      dnn <- list()
      for (i in 1:nv) {
         dnn[[i]] <- as.character(lb[i]:ub[i])
      }
   }
   # dimension names
   if (!is.null(varnames)) names(dnn) <- varnames

   tbl <- array(freq,dim,dimnames=dnn)
   class(tbl) <- "table"
   
   tbl
}

# converts linear array indices to one for the table output of rthtable;
# useful e.g. in identifying which cells are extremes
#
#   lins:  vector of linear indices of interest 
#   lb, ub:  as in rthtable(), but must be fully specified, no reliance
#            on recycling
#
# output value is matrix, row i giving the multidimensional index for
# lins[i]

arylin2mult <- function(lins,lb,ub) {
   d <- length(lb)  # dimension of table
   # products of lengths of the dimensions, e.g. number of rows times
   # number of columns times number of layers
   prods <- vector(length=d)
   prd <- 1
   for (i in 1:d) {
      prods[i] <- prd
      prd <- prd * (ub[i] - lb[i] + 1)
   }
   nlins <- length(lins)
   multixs <- matrix(nrow=nlins,ncol=d)
   for (i in 1:nlins) {
      s <- lins[i] - 1
      for (j in d:1) {
         multixs[i,j] <- s %/% prods[j]
         s <- s %% prods[j]
      }
   }
   # translate 0-based indices to values of the table variables
   multixs + rep(lb,each=nlins)
}

