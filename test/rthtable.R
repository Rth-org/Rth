library(Rth)

### test 1
m <- rbind(
    c(1,2),
    c(1,1),
    c(2,3),
    c(1,2))


x1 <- rthtable(m,c(2,3),2)
x2 <- matrix(c(1, 0, 2, 0, 0, 1), 2, 3)
class(x2) <- "table"
#      [,1] [,2] [,3]
# [1,]    1    2    0
# [2,]    0    0    1

all.equal(x1, x2, check.attributes=FALSE)


### test 2
#m <- rbind( 
#    c(1,2,2), 
#    c(1,1,1),
#    c(2,3,1),
#    c(2,3,2),
#    c(1,2,2))

#x1 <- rthtable(m,c(2,3,2),3)

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


