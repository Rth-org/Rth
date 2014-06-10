rthhist <- function(xname,nbins=15,plot=TRUE, 
   nthreads=automatic(),nchunks=nthreads) {

  if (mode(xname) != "character") 
     stop("the data vector must be specified by quoted name")
  x <- get(xname)
  if (!is.double(x))
    storage.mode <- "double"
  
  tmp <- .Call("rthhist", x, 
    as.integer(nbins), as.integer(nchunks), as.integer(nthreads), PACKAGE="Rth")
  
  if (!plot)
    return(list(counts=tmp$counts, left=tmp$left, binwidth=tmp$binwidth))
  
  plt <- list()
  class(plt) <- "histogram"
  plt$breaks <- seq(from=tmp$left,by=tmp$binwidth,length.out=nbins+1)
  plt$counts <- tmp$counts
  plt$xname <- xname
  plot(plt)
}


