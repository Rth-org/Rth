# m:  data matrix, 1 observation per row
# lb:  vector of lower bds on the variables
# ub:  vector of upper bds on the variables
# varnames:  names of each dimension
# nch:  number of chunks of computation
# nthreads:  number of threads (OMP, TBB cases)

rthtable <- function(m,lb,ub,varnames=NULL,nthreads=automatic(),nch=nthreads)
{
   if (is.vector(m)) m <- matrix(m,ncol=1)
   nv <- as.integer(ncol(m))

   if (length(lb) == 1) lb <- rep(lb,nv)
   if (length(ub) == 1) ub <- rep(ub,nv)

   dim <- ub - lb + 1
   
   if (!is.integer(m))
     storage.mode(m) <- "integer"
   
   freq <- .Call("rthtable",m,lb,ub,as.integer(nch),as.integer(nthreads))
   if (freq == 1) stop("this function does not yet work under TBB")
   
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

