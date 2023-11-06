#! /usr/bin/env bash

if [ -z "$1" ]; then
    echo " usage: $0 [thickness (mm)] "
    exit 1
fi

WHAT="RW3-$1mm"
#NRUNS=100
#NJOBS=4
NEVENTS=10000
NRUNS=10
NJOBS=4

generate_files() {

    thick=$2
    pos=$(echo "520. - 30. - $2 * 0.5" | bc -l)

    ### geo file
    cat <<EOF >> $1/irradSiPM.geo.mac
### RW3 energy degrader 

/absorber0/material RP_RW3
/absorber0/pos 0. 0. ${pos} mm
/absorber0/size 500. 500. ${thick} mm

### here is the sensor

/sensor/size 500. 500. 0.5 mm
/sensor/pos 0. 0. 520.25 mm

### ok, we can initialise the run

/run/initialize
EOF

    ### run file
    cat <<EOF >> $1/irradSiPM.run.mac
/control/execute irradSiPM.geo.mac
/control/execute irradSiPM.gps.tifpa.mac
/run/printProgress 10000
/run/beamOn ${NEVENTS}
EOF

    ### copy other files
    cp irradSiPM $1/.
    cp niel.*.dat $1/.
    cp irradSiPM.gps.tifpa.mac $1/.
}

### create current directory
CURRENTRUNDIR="$(date +%Y%m%d-%H%M%S)-RUN-$WHAT"
mkdir $CURRENTRUNDIR || exit 1
generate_files $CURRENTRUNDIR $1
cd $CURRENTRUNDIR

### loop over runs
for RUN in $(seq -f '%03g' $NRUNS); do

    ### check how many running jobs
    NRUNNING=$(ps -ef | grep "./irradSiPM" | grep -v "grep" | wc -l)
    while [ $NRUNNING -ge $NJOBS ]; do
	sleep 1
	NRUNNING=$(ps -ef | grep "./irradSiPM" | grep -v "grep" | wc -l)
    done

    ### create current run directory
    DIRNAME="$RUN"
    mkdir $DIRNAME
    generate_files $DIRNAME $1
    cd $DIRNAME
		    
    ### get and set random seeds
    RNDSEED1=$(echo "${RANDOM}${RUN}")
    RNDSEED2=$(echo "${RANDOM}${RUN}")
    echo "/random/setSeeds $RNDSEED1 $RNDSEED2" | cat - irradSiPM.run.mac > temp
    mv temp irradSiPM.run.mac
    
    ### start the run
    echo "--- starting $DIRNAME"    
    (time -p ./irradSiPM irradSiPM.run.mac) &> irradSiPM.log &
    cd ..

done

wait

hadd irradSiPM.0.root */irradSiPM.0.root

