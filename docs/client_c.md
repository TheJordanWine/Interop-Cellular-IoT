# C Client

## Introduction

The `client_c` folder will represent the device application. This is the version of the application that is written in C and can be compiled using a Quectel SDK package.  The compiled executable is inteded to be executed on a Quectel board with BG96 chip.  The BG96 module can provide internet access through LTE-M.  This example uses the AT&T network.  


## Archeciture Design

The C client application will create an application entity (AE) named: `MY_METER`.

As soon as it gets created, the server will register the AE and have the updated view in
http://localhost:8080/webpage

```
http://localhost:8080/in-cse
-- in-name
    -- MY_METER
```

The C client will also create a `DESCRIPTOR` and `DATA`. These are containers for describing the DATA as well as sending data to.

```
http://localhost:8080/in-cse
-- in-name
    -- MY_METER
         -- DATA           <--- where data will be sent
         -- PING_METER     <--- where to post to receive Bi-Directional calls from webapp
         -- DESCRIPTOR     <--- describes the data
            .cin870875431  
```

When the client is ready to send data, it will create a `contentInstance` which is an object that contains a content attriubute with the data to be sent. Example content:

```json
{
    "kWH": 23.0
}
```

The server will register and update accordingly to the following view:
```
http://localhost:8080/in-cse
-- in-name
    -- MY_METER
         -- DATA           
            .con851334533  <--- an object which contains a content
         -- DESCRIPTOR     
            .cin870875431  
```

## Prerequisites

The following are prerequisites to run custom IoT software utilizing the OS-IoT-BG96 C library on the BG96 hardware using an LTE network.  

* A Quectel UMTS&LTE Evaluation board obtained from Quectel Wireless Solutions.  
* A BG96 module for the Evaluation board obtained from Quectel. 
* An LTE antenna and GPS that connects to the BG96.  
* A SIM card from a carrier for an LTE network.  
* 2x USB-to-serial connections for communication with the BG96 hardware. 
* USB cable for power to the BG96 board. 
* A windows environment to set up communication with the BG96 board.  
* The SDK for the BG96 board obtained from Quectel.  
* Upgraded firmware to 3.0 on the BG96.  

Note that the SDK package contains several tools for communicating with the BG96 hardware.  The SDK also contains several example C programs and build scripts to compile a custom C IoT application.  

## How to install

*Clone the Interop-Cellular-IoT repository to a machine connected to a BG96.

*Copy MeterMain.bin and oem_app_path.ini files to the BG96:

    -Files are located in Interop-Cellular-IoT/client_c/src/bin folder. 
	-For directions on how to copy files to the BG96, refer to the _Copying Files to BG96_ section of _Getting Started with the SDK for BG96_ at [IOT Docs]( https://docs.google.com/document/d/1bdaJGqPfhoHhJmOrMT27-p9aeQxM8OO03uTmEpWH-no/edit#heading=h.uko3ow9rlo32 "Team 6 Google Drive")

## How to run

*Ensure no unnecessary files are on the BG96

Any previous .ini files may prevent the client application from running.

*Start the BG96 by pressing the power button or restart the BG96 using the following AT command:

```
  AT+CFUN=1,1
```
