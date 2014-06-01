
rthxpos <- function(m) {
   tmp <- .Call("rthxpos",m,PACKAGE="Rth")
   matrix(tmp,nrow=ncol(m))
}


