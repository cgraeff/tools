#!/bin/bash

# get quarks sets list
cd quarks
quarks_set_list=`echo */`
cd ..

# get hadrons sets list
cd hadrons
hadrons_set_list=`echo */`
cd ..

for quarks_set in $quarks_set_list; do

	qset=${quarks_set%?}
	cp quarks/$qset/EOS/data/pressure.dat pressure_quarks.dat
	cp quarks/$qset/Rep_Tsue/data/pressure_chem_pot.dat pressure_chem_pot_quarks.dat
	
	for hadrons_set in $hadrons_set_list; do

		hset=${hadrons_set%?}
		cp hadrons/$hset/EOS/data/pressure.dat pressure_hadrons.dat
		cp hadrons/$hset/Rep_Tsue/data/pressure_chem_pot.dat pressure_chem_pot_hadrons.dat

		gnuplot gnuplot.gpi

		mv quark-hadron_phase_transition.png graph/$qset-$hset-quark-hadron_phase_transition.png
		mv quark-hadron_phase_transition.tex graph/$qset-$hset-quark-hadron_phase_transition.tex
		mv quark-hadron_phase_transition2.png graph/$qset-$hset-quark-hadron_phase_transition2.png
		mv quark-hadron_phase_transition2.tex graph/$qset-$hset-quark-hadron_phase_transition2.tex
	done
done

rm pressure_quarks.dat pressure_hadrons.dat pressure_chem_pot_quarks.dat pressure_chem_pot_hadrons.dat
