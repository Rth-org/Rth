
rthcolsums <- function(m) {
#   require(Rcpp)
#   dyn.load("rthcolsums.so")
   .Call("rthcolsums",m, nrow(m),ncol(m), PACKAGE="Rth")
}


