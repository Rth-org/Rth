x <- c(3,5.01,12,6,5,15,7,8,4.01,10,4,16)
rthhist("x",5,4,plot=F)  # comp hist, 5 bins, 4 threads, no plot
#  $counts
#  [1] 5 3 1 1 2

#  $left
#  [1] 3
#  attr(,"Csingle")
#  [1] TRUE

#  $binwidth
#  [1] 2.6
#  attr(,"Csingle")
#  [1] TRUE

