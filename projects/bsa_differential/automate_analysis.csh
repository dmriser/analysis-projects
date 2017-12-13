#!/bin/tcsh 

if ($1 == "") then 
    echo "Please enter a folder full of root files from the farm"
    exit(0) 
endif

set outputName = $1
set localFile  = "/volatile/clas12/dmriser/rootFiles/asymmetry/$outputName.root"

# getting hadd 
source ~/.login 

if (`ls /volatile/clas12/dmriser/farm_out/$outputName/*.root | wc -l | awk '{print $1}'` > 0) then
    # creating combined file 
    hadd $localFile /volatile/clas12/dmriser/farm_out/$outputName/*.root 

    # running local codes 
    if (-f $localFile) then 
	./src/fit $localFile 

	# running plots 
#	cd macro/ 
#	root -b -q 'plot_tofmass.cxx("'$localFile'")'
#	root -b -q 'plot_integrated_asymmetries.cxx("'$localFile'")'
#	root -b -q 'plot_signal_background.cxx("'$localFile'")'
#	cd - 
    endif 

endif


