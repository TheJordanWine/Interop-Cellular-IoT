# C Client

## Introduction

The `client_c` folder will represent the device application. This code was designed to be placed on a BG96 with LTE Cat M1 cellular enabled access.


## Archeciture Design

The client application will create an application entity (AE) named: `MY_METER`.

As soon as it is created, the server will register the AE and have the updated view at
http://localhost:8080/webpage

```
http://localhost:8080/in-cse
-- in-name
    -- MY_METER
```

The client will also create a `DESCRIPTOR` and `DATA`. These are containers for describing the device as well as sending data to.

```
http://localhost:8080/in-cse
-- in-name
    -- MY_METER
         -- DATA           <--- where data will be sent
         -- PING_METER     <--- where to post to receive Bi-Directional calls from webapp
         -- DESCRIPTOR     <--- describes the data
            .cin870875431  
```

When the client is ready to send data, it will create a `contentInstance` which is an object that contains a content attribute with the data to be sent. Example content:

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

For setup and configuration instructions refer to _Getting Started with Quectel BG96 board_ at [IOT Docs]( https://docs.google.com/document/d/1E091LTgkJBjlXAKSNabQt804qBQCf_KshEoIAmmLL7Y/edit "Team 6 Google Drive")

*A Quectel hardware board that contains the BG96 module.

*A Windows environment setup to communicate with the BG96. 

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

