library(Rth)


n <- 1e2
x <- matrix(rnorm(n), ncol=1)


### Norms
x1 <- norm(x, type="F")
x2 <- rthnorm(x, p=2)
all.equal(x1, x2)

x1 <- norm(x, type="O")
x2 <- rthnorm(x, p=1)
all.equal(x1, x2)


### Distances
y <- matrix(rnorm(n), ncol=1)
X <- t(matrix(c(x,y), ncol=2))

all.equal(dist(X)[1], rthdist(x,y))
