#' Parallel Computation of Histograms
#' 
#' Similar to R's \code{hist()}
#' 
#' Similar to \code{hist()}, though with fewer options.
#' 
#' @param xname 
#' Name of data vector, quoted.
#' @param nbins 
#' The number of bins.
#' @param plot 
#' If TRUE, plot will be displayed.
#' @param nthreads 
#' An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @param nchunks 
#' Number of chunks for partitioning the data.
#' 
#' @return Object of R class \code{"histogram"}.
#' 
#' @examples
#' 
#' \dontrun{
#' # R built-in geyser data
#' fw <- faithful$waiting
#' fwh <- rthhist("fw")
#' fwh  # display plot, famously bimodal
#' fwh <- rthhist("fw",plot=FALSE)
#' fwh
#' # bin counts, left endpt, binwidth printed out
#' }
#' 
#' @export
rthhist <- function(xname,nbins=15,plot=TRUE, nthreads=rth.nthreads(),nchunks=nthreads) {

  # in TBB cases, automatic() returns -1, the code for giving TBB carte
  # blanche
  if (nthreads == -1) nchunks <- 2

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


