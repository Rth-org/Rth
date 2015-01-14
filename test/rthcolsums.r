library(Rth)

m <- 100
n <- 10
x <- matrix(rnorm(m*n), m, n)

t2 <- colSums(x)
t1 <- rthcolsums(x)

all.equal(t1, t2)
