#!/bin/bash 
#
# serverInstallation.sh
#
# This file is used to install OM2M server version 1.0.0 and all dependencies.
#
# Instructions:
#   1) Ensure dependencies are met.
#   2) Execute with "./serverInstallation.sh". This creates a "test" binary
#      executable in the current directory.
#	3) The server will be installed in the current working directory at execution.
#   3) To start the server:
#      ./org.eclipse.om2m.site.in-cse/target/products/in-cse/linux/gtk/x86_64/start.sh
#
# @author Team 6
#
# @version 1.0
#
# Get current working directory
runDir=$PWD;
# Install needed packages and apply updates
apt-get update -y;
apt-get upgrade -y;
apt-get install -y ufw git default-jdk software-properties-common python-software-properties debconf-utils;
add-apt-repository "deb http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main";
apt-get update -y;
# Install Java8
echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | debconf-set-selections
printf 'y\ny\n'| apt-get install oracle-java8-installer;
# Set JAVA_HOME environment variable
tee -a /etc/environment <<< 'JAVA_HOME="/usr/lib/jvm/java-8-oracle"';
source /etc/environment;
# Install Apache Maven
cd /usr/local;
wget http://www-eu.apache.org/dist/maven/maven-3/3.5.4/binaries/apache-maven-3.5.4-bin.tar.gz;
tar xzf apache-maven-3.5.4-bin.tar.gz;
ln -s apache-maven-3.5.4 apache-maven;
# Set Maven HOME environment variables
tee -a /etc/profile.d/apache-maven.sh <<< 'export JAVA_HOME=/usr/lib/jvm/java-8-oracle';
tee -a /etc/profile.d/apache-maven.sh <<<  'export M2_HOME=/usr/local/apache-maven';
tee -a /etc/profile.d/apache-maven.sh <<< 'export MAVEN_HOME=/usr/local/apache-maven';
tee -a /etc/profile.d/apache-maven.sh <<< 'export PATH=${M2_HOME}/bin:${PATH}';
source /etc/profile.d/apache-maven.sh;
# Remove Apache Maven tar
rm -f apache-maven-3.5.4-bin.tar.gz;
# Get OM2M V1.0.0
wget http://git.eclipse.org/c/om2m/org.eclipse.om2m.git/snapshot/org.eclipse.om2m-1.0.0.tar.gz;
tar xzf org.eclipse.om2m-1.0.0.tar.gz -C $runDir;
rm -f org.eclipse.om2m-1.0.0.tar.gz
# Install OM2M V1.0.0
cd $runDir/org.eclipse.om2m-1.0.0;
echo y | mvn clean install;
# Make server executable
chmod +x org.eclipse.om2m.site.in-cse/target/products/in-cse/linux/gtk/x86_64/start.sh;
