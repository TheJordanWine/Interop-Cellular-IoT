The server certificate currently onl has subject Alternative names of:
    om2mServer
    winehome.publicvm.com

It also includes IP addresses of:
    127.0.0.1
    192.168.0.90

If the client needs to connect to any other IP or hostname other than those above, the simplest
thing to do is add the following line to /etc/hosts on the client
    <IP Of Server> om2mServer

Alternatively, the server certificate can be re-generated with additional names or IPs.