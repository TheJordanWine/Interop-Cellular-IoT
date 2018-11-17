#!/bin/bash
##
# Version:  1.0
# Descirption: This is a shell based implementation of an OneM2M Application Entity which
# uses the OS-IoT library via it's command line interface to simulate sending data
# 

# GLOBAL VARIABLES
AE_NAME="MY_SENSOR"
IN_CSE_HOST="127.0.0.1"
IN_CSE_PORT="8080"


# FUNCTIONS
usage(){
    echo "Usage: $(basename $0) [-h CseHost] [-p CsePort]"
    exit 254
}



# EXECUTION
main() {
    trap "echo Interupted; exit 255" INT SIGINT

    # Parse command line arguments
    while getopts "h:p:" opt; do
        case "$opt" in
        h)  IN_CSE_HOST=$opt
            ;;
        p)  IN_CSE_PORT=$opt
            ;;
        *)  usage
            ;;
        esac
    done

    if [[ -f osiotcmd ]]; then
        chmod 755 osiotcmd
    else
        echo "osiotcmd is not in ${PWD}. First compile osiotcmd"
        return 2
    fi

    echo "Connecting to ${IN_CSE_HOST}:${IN_CSE_PORT}"

    # Create AE
    ./osiotcmd -h ${IN_CSE_HOST}:${IN_CSE_PORT} -f admin:admin --ae --rn ${AE_NAME}

    # Create DATA Container
    ./osiotcmd -h ${IN_CSE_HOST}:${IN_CSE_PORT} -f admin:admin --container -A "~/in-cse/in-name/${AE_NAME}" --rn DATA

    # Send data
    while true; do
        number=$RANDOM
        let "number %= 100"
        ./osiotcmd -h ${IN_CSE_HOST}:${IN_CSE_PORT} -f admin:admin --ci -A "~/in-cse/in-name/${AE_NAME}/DATA" -j --content '{"temp" : '${number}'}'
        sleep 5
    done

    return 1
}

# RUN
main
exit $?
