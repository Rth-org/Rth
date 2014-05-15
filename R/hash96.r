hash96 <- function(a, b, c)
{
  .Call("R_mix_96", as.integer(a), as.integer(b), as.integer(c), PACKAGE="Rth")
}
