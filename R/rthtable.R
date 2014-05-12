# m:  data matrix
# dim:  vector of upper bds on the variables
# nch:  number of chunks of computation

rthtable <- function(m,dim,nch=0)
{
  ndim <- prod(dim)
  n <- as.integer(nrow(m))
  nv <- as.integer(ncol(m))
  
  if (!is.integer(m))
    storage.mode(m) <- "integer"
  
  freq <- .Call("rthtable", m, n, nv, as.integer(dim), as.integer(ndim), as.integer(nch), 
    PACKAGE="rth")
  
  tbl <- array(freq, dim)
  class(tbl) <- "table"
  
  return(tbl)
}

