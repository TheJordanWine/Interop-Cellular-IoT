# This is a Dockerfile for OM2M version 1.0.0
# Author: Jordan Wine
# Version: 1

# Build command
# sudo docker build -t ser401group6/om2m:1.0.0 -t ser401group6/om2m:latest .

# Source
# Need version before 16.04 so that we can use java 7
FROM ubuntu:14.04

# Maintainer
MAINTAINER TheJordanWine jordandwine@gmail.com

# Install
RUN apt-get update
RUN apt-get install wget -y  && apt-get clean

# Download image
RUN wget http://ftp.osuosl.org/pub/eclipse/om2m/releases/1.0.0/OM2M-1.0.0.zip

# Install java
RUN mkdir -p /var/cache/apt/archives/
RUN apt-get install java-common default-jre -y  && apt-get clean

# Unzip OM2M zip file
RUN apt-get install unzip -y && apt-get clean
RUN unzip OM2M-1.0.0.zip

# Make start.sh executable
RUN chmod a+x IN-CSE/start.sh 

WORKDIR /IN-CSE
ENTRYPOINT ["sh"]
CMD ["start.sh"]
