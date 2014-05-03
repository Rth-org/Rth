# author: N. Matloff

rthorder <- function(x,rnk=FALSE) {
   tmp <- .Call("rthorder",x)
   # convert from C's 0-based indexing to R's 1-based
   return(tmp$out + 1)
}


