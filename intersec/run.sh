#!/bin/bash

cd quarks
quark_sets=`echo */`
cd ..

cd hadrons
hadron_sets=`echo */`
cd ..

for quark_set in $quark_sets; do
	for hadron_set in $hadron_sets; do
	
		quark_set=${quark_set%*/}
		hadron_set=${hadron_set%*/}
		
		cp quarks/$quark_set/Rep_Tsue/data/pressure_chem_pot.dat quark_set.dat
		cp hadrons/$hadron_set/Rep_Tsue/data/pressure_chem_pot.dat hadron_set.dat
		
		echo $quark_set-$hadron_set
		./intersec -f hadron_set.dat -s quark_set.dat
		echo "---"

		# output suitable for inclusion in TeX tables
		# val=`./intersec -f hadron_set.dat -s quark_set.dat -x`
		# echo "$quark_set & $hadron_set & \\np{$val}" >> log.tex 
		
		gnuplot gnuplot.gpi
		
		mv output.png graph/$quark_set-$hadron_set.png
		
		rm quark_set.dat
		rm hadron_set.dat
	done
done
