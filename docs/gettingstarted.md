# Getting Started

This project is part of the Capstone Senior Project at ASU under the Ira. School Student of Engineering. The goal is to make devleoping with IoT applications alot easier than what it is today. We are using a the atis's os iot library which is avaible in C/C++. The project works as follows

We have a om2m server which resides in the `server/` folder. This server acts as the middle man between the client applications and the web application. This is what is consdired oneM2M standards as communicating directly to the client is a nono and should only be done via subscriptions done form teh webapp

The `client/` and `client_c/` folders are what we call the client applications. The client folder was devleoped on a Raspbery Pi using WiFi with a gcc compiler. What the client application does is it sends data to the OM2M server in which the web application is able to read and in some cases (under development with C) send data on command. The client_c is the client application that runs on the BG96, see [here](/client_c.html) for more details

The `webapp/` folder is used to run a webapplication that is able to create subscriptions to the om2m server. Upon a successfuly subscription, a user is able to 

- See data come in live
- Download data as an excel sheet
- Ping the resource to send current reading
- Delete the resource



## Prerequisites

You will need the following tools installed on your computer, although not everything is needed


- Git
- NodeJS and NPM
- MongoDB
- OM2M Server 1.0.0
- Python 2.7
- C++ (if doing work on the C++ Client)
- C Compiler (comes with the Quectel SDK)
- BG96 Module (if doing dev work on the c client)