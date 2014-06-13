# m:  data matrix, 1 observation per row
# lb:  vector of lower bds on the variables
# ub:  vector of upper bds on the variables
# varnames:  names of each dimension
# nch:  number of chunks of computation
# nthreads:  number of threads (OMP, TBB cases)

rthtable <- function(m,lb,ub,varnames=NULL,nthreads=automatic(),nch=nthreads)
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
   dnn <- list()
   for (i in 1:nv) {
      dnn[[i]] <- as.character(lb[i]:ub[i])
   }
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

