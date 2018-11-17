#
# Version:  1.0
# Description: This is a bash script to simplify the installation process for
# the web app. It will install the required dependencies if needed and run the
# web app using nodejs.
# THIS CURRENTLY ONLY SUPPORTS UBUNUTU and REDHAT/CENTOS DISTRIBUTIONS

# GLOBAL VARIABLES
declare -a dependencies
dependencies = ("nodejs" "npm")


# FUNCTIONS
##
# A function that checks if a specificed parameter is installed or not
# $1 package The package to check if it is installed
# returns: 0 (success) if the package is installed, 1 if not installed
# OR 2 if unable to determine.
#
isInstalled() {
    package=$1
    set retVal

    if [[ $(getPackageManger) == "yum" ]]; then
        if rpm -qa |grep -q ${package}; then
            retVal=0
        else
            retVal=1
        fi  
    elif [[ $(getPackageManger) == "apt-get" ]]; then
        if dpkg -l |grep -q ${pacakge}; then
            retVal=0
        else
            retVal=1
        fi
    else
        retVal=2
    fi

    return ${retVal}
}

##
# Gets the type of package manager we use on this system
#
getPackageManger() {
    # First lets find out which package manager we're using based on distribution
    if [[ $(getLinuxDist) == "ubuntu" ]]; then
        packageManger = "apt-get"
    elif [[ $(getLinuxDist) == "redhat" || $(getLinuxDist) == "centos" ]]; then
        packageManger = "yum"
    else
        packageManger = "unknown"
    fi

    echo ${packageManger}
}

##
# This checks which linux distribution is installed and reports it as a string value
# 
getLinuxDist() {
    for dist in ubuntu centos redhat; do
        if grep -q -i ${dist} /etc/*-release; then
            echo ${dist}
        fi
    done
}

##
# Installs the specified package
#
installPackage() {
    package=$1

    sudo $(getPackageManger) install ${package} -y
    return $?
}





# EXECUTION
main() {
    # Trap interupt signals
    trap "echo interupted; exit 255" INT SIGINT

    # Make sure dependencies are installed
    for dep in ${dependencies[@]}; do
        if ! isInstalled ${dep}; then
            echo "Package: ${dep} is not installed. Installing it now."
            if ! installPackage ${dep}; then
                echo "Failed to install dependency: ${package}"
                echo "Exiting now"
                return 1
        fi
    done



    return 0
}



# RUN
main
exit $?