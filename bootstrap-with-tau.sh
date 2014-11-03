#!/bin/bash -e

#configure parameters
export TAU_ROOT=/usr/local/tau/git
export BOOST_ROOT=/usr
# this one is only meaningful for HPX-3 from LSU
# export HPX_HAVE_ITTNOTIFY=1 

# runtime parameters for HPX-3 (LSU)
export APEX_POLICY=1
export APEX_CONCURRENCY=0
export APEX_TAU=1
# this one is only meaningful for HPX-3 from LSU
# export HPX_HAVE_ITTNOTIFY=1

# NO NEED TO MODIFY ANYTHING BELOW THIS LINE
# ------------------------------------------------------------------------

# Get time as a UNIX timestamp (seconds elapsed since Jan 1, 1970 0:00 UTC)
T="$(date +%s)"

if [ $# -eq 1 ] ; then
	if [ $1 == "--clean" ] || [ $1 == "-c" ] ; then
		rm -rf build_*
	fi
fi

datestamp=`date +%Y.%m.%d-%H.%M.%S`
dir="build_$datestamp"
mkdir $dir
cd $dir

cmake \
-G "CodeBlocks - Unix Makefiles" \
-DBOOST_ROOT=$BOOST_ROOT \
-DCMAKE_BUILD_TYPE=RelWithDebInfo \
-DCMAKE_INSTALL_PREFIX=../install \
-DTAU_ROOT=$TAU_ROOT \
..

procs=1
if [ -f '/proc/cpuinfo' ] ; then
  procs=`grep -c ^processor /proc/cpuinfo`
fi
make -j `expr $procs + 1`

make test
make install

printf "\nSUCCESS!\n"
T="$(($(date +%s)-T))"
printf "Time to configure and build APEX: %02d days %02d hours %02d minutes %02d seconds.\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"