In order to use the simpleSender.sh testing script, the hostname "om2mServer" must be present
in /etc/hosts in order to validate the subject alternative name of the server certificate.
Whichever host the script attempts to connect to for the OM2M server (default of 127.0.0.1) must be
tagged as "om2mServer"