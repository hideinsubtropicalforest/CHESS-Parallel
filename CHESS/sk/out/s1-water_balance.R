# libraries


# lcoal setting
# setwd("C:/Users/xuzhw/source/repos/xuzhenwu/CHESS-Parallel/CHESS/lxh_sk/out/")
# setwd("E:/")
setwd("C:/Users/xuzhw/source/repos/xuzhenwu/TestFiles/results/t3/")

#1.
out <- water_balance()
#2.
simu <- read.table("Basin_daily_hydro.dat", header = T)
obse <- read.table("lxh_sf_00_16.txt", header = T)
obse <- subset(obse, Year >= 2000 & Year <= 2016)$StreamF
simu <- subset(simu, Year >= 2000 & Year <= 2016)$StreamF

ns <- compute_ns(obse,simu)