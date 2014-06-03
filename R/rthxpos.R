
rthxpos <- function(m) {
   tmp <- .Call("rthxpos",m,PACKAGE="Rth")
#   matrix(tmp,nrow=ncol(m))
   dim(tmp) <- c(ncol(m), nrow(m))
   
   return(tmp)
}


