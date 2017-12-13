#!/bin/tcsh 

set files = `ls /volatile/clas12/dmriser/analysis/ef1_analysis/root_files/*`

foreach file ($files)
    set run = `echo $file | cut -d'.' -f1 | cut -d'0' -f2-4`
    set stub = `echo $file | cut -d'A' -f2 | cut -d'.' -f1`

#    echo "Searching for run $run, stub $stub"


    set possibleFiles = `ls /volatile/clas12/dmriser/analysis/e1f_analysis/pass2/* | grep $run`

    foreach pf ($possibleFiles)
	set pstub = `echo $pf | cut -d'.' -f2 | cut -d'a' -f2`
	
	if ($pstub == $stub) then 
	    ./print_hel $file $pf /volatile/clas12/dmriser/analysis/e1f_analysis/corr_hel/corr_hel.$run.a$stub.root
	endif
    end
end
