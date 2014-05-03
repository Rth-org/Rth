# author: N. Matloff

rthpearson <- function(x,y) {
   if (!is.double(x))
      storage.mode(x) <- "double"
   if (!is.double(y))
      storage.mode(y) <- "double"
   
   .Call("rthpearson", x, y)
}

