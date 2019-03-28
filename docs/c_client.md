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

## How to install

TODO

## How to run

TODO
