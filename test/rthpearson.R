rthpearson <- function(x,y) {
   library(Rcpp)
   dyn.load("rthpearson.so")
   .Call("rthpearson",x,y)
}

