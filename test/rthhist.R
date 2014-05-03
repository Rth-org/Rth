rthhist <- function(xname,nbins,nchunks,plot=T,gpu=F) {
   if (gpu) {
      dyn.load("rthhistgpu.so") 
      asfunction <- as.single
      vecfunction <- single
      dupval <- T
   } else {
      dyn.load("rthhist.so") 
      asfunction <- as.double
      vecfunction <- double
      dupval <- F
   }
   x <- get(xname)
   n <- length(x)
   tmp <- .C("rthhist",asfunction(x),
      as.integer(n),as.integer(nbins),as.integer(nchunks),
      counts=integer(nbins),left=vecfunction(1),binwidth=vecfunction(1),
      DUP=dupval)
   if (!plot)
      return(list(counts=tmp$counts,left=tmp$left,binwidth=tmp$binwidth))
   plt <- list()
   class(plt) <- "histogram"
   plt$breaks <- seq(from=tmp$left,by=tmp$binwidth,length.out=nbins+1)
   plt$counts <- tmp$counts
   plt$xname <- xname
   plot(plt)
}


