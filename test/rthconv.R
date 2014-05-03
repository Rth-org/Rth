rthconv <- function(x,y) {
  
  if (!is.double(x))
    storage.mode(x) <- "double"
  if (!is.double(y))
    storage.mode(y) <- "double"
  
  ret <- .Call("rthconv", x, y)
  
  return(ret)
}

