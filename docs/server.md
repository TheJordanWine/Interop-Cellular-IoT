# Server

## Introduction

The OM2M server adheads to the oneM2M Standards and Specfication from the offical oneM2m Site: [here](http://www.onem2m.org/developers-corner/documentation/getting-started)

Server can run in a Linux or Windows environment.


## Basic Archeciture Design

The server has a list of RESTFul API Endpoints provided [here](https://wiki.eclipse.org/OM2M/one/REST_API). 


## How to install
_These instructions are based on an Ubuntu system. Adapt as needed for the target platform_
```bash
# Update repositories
sudo apt-get update
sudo apt-get install wget -y  && apt-get clean
# Prepare Java8 install
mkdir -p /var/cache/apt/archives/
sudo apt-get install java-common default-jre software-properties-common python-software-properties debconf-utils -y  && sudo apt-get clean
sudo add-apt-repository "deb http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main";
sudo apt-get update -y
sudo apt-get install unzip -y && sudo apt-get clean
# Install Java8
echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | debconf-set-selections
printf 'y\ny\n'| apt-get install oracle-java8-installer;
# Set JAVA_HOME environment variable
tee -a /etc/environment <<< 'JAVA_HOME="/usr/lib/jvm/java-8-oracle"';
source /etc/environment;
# Download OM2M release
wget http://ftp.osuosl.org/pub/eclipse/om2m/releases/1.0.0/OM2M-1.0.0.zip
# Unzip OM2M zip file
unzip OM2M-1.0.0.zip
# Make start.sh executable
chmod a+x IN-CSE/start.sh 
```

## How to configure HTTPS
Generate a java keystore with the following commands
```bash
cd server
 # Generate key/cert pair. Edit SAN as applicable to your domain. Alternatively, provide a CA-signed cert in a java keystore
keytool --keystore server-keystore.jks --storepass SomePassword -genkeypair -dname "CN=OM2M Server, OU=SER 401 Capstone Team6, O=Arizona State University, L=Tempe, ST=Arizona, C=US" -keyalg RSA -alias om2m -ext SAN=dns:om2mServer,ip:127.0.0.1
# Export certificate to client and webapp
keytool --keystore server-keystore.jks --storepass onem2m -alias om2m -export -file ../client/certstore/serverCertificate.der
openssl x509 -inform DER -outform PEM -in ../client/certstore/serverCertificate.der -out ../client/certstore/serverCertificate.pem
cp -pr ../client/certstore ../webapp/certstore
```
Add the following lines to the IN-CSE `config.ini` file.
```ini
org.eclipse.equinox.http.jetty.https.enabled=true
org.eclipse.equinox.http.jetty.https.port=8443
org.eclipse.equinox.http.jetty.ssl.password=SomePassword
org.eclipse.equinox.http.jetty.ssl.keypassword=SomePassword
org.eclipse.equinox.http.jetty.ssl.keystore=../server-keystore.jks
org.eclipse.equinox.http.jetty.ssl.protocol=TLS
```

## How to run

```bash
cd server/IN-CSE
bash start.sh
```