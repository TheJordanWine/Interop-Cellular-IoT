CERTIFICATE:

The server certificate currently only has subject Alternative names of:
    om2mServer
    winehome.publicvm.com

It also includes IP addresses of:
    127.0.0.1
    192.168.0.90

If the client needs to connect to any other IP or hostname other than those above, the simplest
thing to do is add the following line to /etc/hosts on the client
    <IP Of Server> om2mServer

Alternatively, the server certificate can be re-generated with additional names or IPs.

INSTALL:

To install the client, you may executed the following command as root user:
          sh ./ClientBuild.sh

This script will install any needed dependencies, and compile the C++ client.

RUN:

When running the client, it will automatically load the values stored in the settings.config file.
Argument flags can be used to modify any of the client's settings.
These configuration changes can be saved to the settings.config File.
Below is a list of all argument flags and their corresponding setting:

-help               :        Prints this list
-a <name>           :        AE App ID
-c <name>           :        Container Name
-d <seconds>        :        Delay between communications in seconds
-h <IPaddress:port> :        Host Name
-l <location>       :        Location
-n <name>           :        AE Resource Name
-p                  :        password    -   Will prompt for entry
-r <filepath>       :        CSE Root address
-t <minutes>        :        Runtime in Minutes
-s                  :        Save new configurations to the configuration file

example:
./main -a appID -c contName -d 5 -h 123.123.123.123:1234 -l Tempe -n aeName -p -r /cseRootAddr -t 1 -s
