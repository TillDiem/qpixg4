for f in ../macros/EnergyStudy/*.mac
do

        echo "Processing file $f ..."
	macro=$(echo $f | sed 's/\.\.\//\.\//g')
	run="run_"$(echo $f | sed 's/\.mac//g' | sed 's/\.\.\/macros\/BGS\///g' )
        echo "cd /pc2014-data3/tdieminger && source setup_lAr.sh && cd /pc2014-data3/tdieminger/SoLAr_Env/qpixg4 && ./build/app/G4_QPIX $macro" > $run.sh
        chmod +x $run.sh
        screen -S $run -m -d bash $run.sh
        sleep 1
done
