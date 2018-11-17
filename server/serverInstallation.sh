#!/bin/bash 
runDir=$PWD
apt-get update -y;
apt-get upgrade -y;
apt-get install -y ufw git default-jdk software-properties-common python-software-properties debconf-utils;
add-apt-repository "deb http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main";
apt-get update -y;
echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | debconf-set-selections
printf 'y\ny\n'| apt-get install oracle-java8-installer;
tee -a /etc/environment <<< 'JAVA_HOME="/usr/lib/jvm/java-8-oracle"';
source /etc/environment;
cd /usr/local;
wget http://www-eu.apache.org/dist/maven/maven-3/3.5.4/binaries/apache-maven-3.5.4-bin.tar.gz;
tar xzf apache-maven-3.5.4-bin.tar.gz;
ln -s apache-maven-3.5.4 apache-maven;
cd /;
tee -a /etc/profile.d/apache-maven.sh <<< 'export JAVA_HOME=/usr/lib/jvm/java-8-oracle';
tee -a /etc/profile.d/apache-maven.sh <<<  'export M2_HOME=/usr/local/apache-maven';
tee -a /etc/profile.d/apache-maven.sh <<< 'export MAVEN_HOME=/usr/local/apache-maven';
tee -a /etc/profile.d/apache-maven.sh <<< 'export PATH=${M2_HOME}/bin:${PATH}';
source /etc/profile.d/apache-maven.sh;
rm -f apache-maven-3.5.4-bin.tar.gz;
git clone https://git.eclipse.org/r/om2m/org.eclipse.om2m ./$runDir/om2m;
cd $runDir/om2m;
echo y | mvn clean install;
chmod +x org.eclipse.om2m.site.in-cse/target/products/in-cse/linux/gtk/x86_64/start.sh;
./org.eclipse.om2m.site.in-cse/target/products/in-cse/linux/gtk/x86_64/start.sh;
