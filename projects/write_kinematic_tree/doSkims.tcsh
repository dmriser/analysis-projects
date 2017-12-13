#!/bin/tcsh 

set pathToFiles = "/volatile/clas12/dmriser/data/kaon/root"
set outputPath  = "/volatile/clas12/dmriser/data/miniTree"
set files = `ls $pathToFiles/*`

foreach file ($files)
	set run = `echo $file | grep -o "3[0-9][0-9][0-9][0-9]"`
    
	echo "doing $run.root"
	./MakeTree -N=1e9 -OUT=$outputPath/mini_$run.root $file
end
