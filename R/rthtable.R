
# m:  data matrix
# dim:  vector of upper bds on the variables
# nch:  number of chunks of computation

rthtable <- function(m,dim,nch=0,gpu=F) {
   if (gpu) {
      asfunction <- as.single
      vecfunction <- single
      dupval <- T
   } else {
      asfunction <- as.double
      vecfunction <- double
      dupval <- F
   }
   n <- nrow(m)
   nv <- ncol(m)
   ndim <- prod(dim)
   tmp <- .C("rthtable",as.integer(m),
      as.integer(n),as.integer(nv),as.integer(dim),as.integer(ndim),
      as.integer(nch),freq=integer(ndim),DUP=dupval)
   tbl <- array(tmp$freq,dim)
   class(tbl) <- "table"
   tbl
}

# tests
#
# m <- rbind(
#    c(1,2),
#    c(1,1),
#    c(2,3),
#    c(1,2))
#
# should output
# > rthtable(m,c(2,3),2)
#      [,1] [,2] [,3]
# [1,]    1    2    0
# [2,]    0    0    1

# m <- rbind( 
#    c(1,2,2), 
#    c(1,1,1),
#    c(2,3,1),
#    c(2,3,2),
#    c(1,2,2))
# rthtable(m,c(2,3,2),3)
#
# should output
# , , 1
# 
#      [,1] [,2] [,3]
# [1,]    1    0    0
# [2,]    0    0    1
# 
# , , 2
# 
#      [,1] [,2] [,3]
# [1,]    0    2    0
# [2,]    0    0    1


