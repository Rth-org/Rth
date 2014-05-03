# author: N. Matloff

rthkendall <- function(x,y,gpu=F) {
   if (gpu) {
      asfunction <- as.single
      vecfunction <- single
      dupval <- T
   } else {
      asfunction <- as.double
      vecfunction <- double
      dupval <- F
   }
   n <- length(x)
   tmp <-
   .C("rthkendall",asfunction(x),asfunction(y),as.integer(n),
      tmpres=vecfunction(1),DUP=dupval)
   return(tmp$tmpres)
}



