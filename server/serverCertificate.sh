#!/bin/bash 
apt-get install -y openssl;
openssl req -nodes -newkey rsa:2048 -keyout server.key -out server.csr -subj "/C=US/ST=Arizona/L=Tempe/O=Arizona State University/OU=Capstone Team 6/CN=osiot.com";
openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt;
