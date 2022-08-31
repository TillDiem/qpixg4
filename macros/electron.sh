#!/bin/sh

for e in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do

echo $e
echo "
# set verbosity
/control/verbose 1
/run/verbose 1
/tracking/verbose 0

# output path
/Inputs/root_output ./output/CoverageStudy/single_electron_$e.root

# initialize run
/run/initialize
/random/setSeeds 0 31

# limit radioactive decays
#/grdm/nucleusLimits 1 35 1 17  # aMin aMax zMin zMax

# particle type
# e-, e+, mu-, mu+, neutron, proton, anti_proton, pi-, pi+, pi0, kaon-, kaon+,
# gamma, opticalphoton, ...
/gps/particle e-

# point source
/gps/pos/type Volume
/gps/pos/shape Para
/gps/pos/centre 0.8 1.3 0.5 m
/gps/pos/halfx 0.8 m
/gps/pos/halfy 1.3 m
/gps/pos/halfz 0.5 m

# angular distribution
# initial momentum in the -y direction
/gps/ang/type iso

# energy distribution
/gps/ene/type Mono
/gps/ene/mono $e MeV

# run
/run/beamOn 10000
" > single_electron_$e.mac
done
