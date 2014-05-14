rthhist <- function(xname,nbins,nchunks,plot=TRUE) {
  x <- get(xname)
  
  if (!is.double(x))
    storage.mode <- "double"
  
  tmp <- .Call("rthhist", x, as.integer(nbins), as.integer(nchunks),
    PACKAGE="rth")
  
  if (!plot)
    return(list(counts=tmp$counts, left=tmp$left, binwidth=tmp$binwidth))
  
  plt <- list()
  class(plt) <- "histogram"
  plt$breaks <- seq(from=tmp$left,by=tmp$binwidth,length.out=nbins+1)
  plt$counts <- tmp$counts
  plt$xname <- xname
  plot(plt)
}


