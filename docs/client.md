# Client

## Introduction

The `client` folder will represent the device application. This code can be placed in any resource constrained device, the following project was tested on a raspberry pi model 3 with wifi enabled access.


## Archeciture Design

The client application will create an application entity (AE) named: `MY_METER`.

As soon as it gets created, the server will register the AE and have the updated view in
http://localhost:8080/webpage

```
http://localhost:8080/in-cse
-- in-name
    -- MY_METER
```

The client will also create a `DESCRIPTOR` and `DATA`. These are containers for describing the DATA as well as sending data to.

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

**Option #1 - Build Script**

*Ensure that the pre-requisites are met:

  - Debian Linux installed.
  - The ClientBuild.sh script must be in the client/src repository directory.
  - The "src" sub-folder is required to contain MeterMain.cpp, UtilityMeter.cpp, and UtilityMeter.h.
  - The g++ compiler installed.
  - The "lib/xsd/cxx" subfolder must contain the codesynthesis header files.
  - The "git" program in order to install the OS-IoT library as a sub-module in the sub-folder "lib/atis-os-iot".
  - The programs:  libxerces-c-dev, libcurl4-openssl-dev, libssl1.0-dev.
  - Execute with Sudo or root.

*Execute the script with the command:

```bash
  sudo sh ClientBuild.sh
```

**Option #2 - Manual Install**

*Clone the Interop-Cellular-IoT repository to the client:
```bash
  git clone https://github.com/TheJordanWine/Interop-Cellular-IoT.git
```

*Copy the Codesynthesis package headers to /usr/local/include:
```bash
  Interop-Cellular-IoT$ sudo cp -r client/lib/xsd /usr/local/include
```

*Dynamically link the OS-IoT library to our project.

```bash
  Interop-Cellular-IoT$ cd client/lib
  Interop-Cellular-IoT/client/lib$ . ./importosiot.sh
  Interop-Cellular-IoT/client/lib$ cd atis-os-iot
  Interop-Cellular-IoT/client/lib/atis-os-iot$ make libosiotdyn
  Interop-Cellular-IoT/client/lib/atis-os-iot$ mkdir ../../src/cdt
  Interop-Cellular-IoT/client/lib/atis-os-iot$ cp libosiot.so ../../src/
  Interop-Cellular-IoT/client/lib/atis-os-iot$ cp CDT-*.hxx ../../src/
  Interop-Cellular-IoT/client/lib/atis-os-iot$ cp onem2m.hxx ../../src/
  Interop-Cellular-IoT/client/lib/atis-os-iot$ cp cdtHeads.hxx ../../src/
  Interop-Cellular-IoT/client/lib/atis-os-iot$ cp cdt/*.hxx ../../src/cdt/
  Interop-Cellular-IoT/client/lib/atis-os-iot$ cd ../../../
```

*Compile the program with g++

```bash
  Interop-Cellular-IoT/client/src$ g++ -v -o main MeterMain.cpp UtilityMeter.cpp -Wl,-rpath=. -L. -losiot -lssl -lcrypto -lxerces-c -lcurl -lpthread
```

## How to run

*The following syntax shows how to run the executable binary file named "main" with default parameters:

```bash
  ./main
```

*The default parameters are shown below along with optional command-line arguments to change such default values:

```
-h <hostName> = "127.0.0.1:8080"      # The IP:Port of OM2M server.
-l <loginCred> = "admin:admin"        # The OM2M Server login credentials.
-n <aeName> = "MY_METER"              # Name of the AE Resource to create.
-a <aeAppId> = "app1"                 # Name of the AE App Id.
-c <contName> = "DATA"                # Data Container Name.
-L <location> = "Home"                # Location of Utility Meter.
-s <saveConfig> = false               # Save config settings to file.
-r <cseRootAddr> = "/in-cse/in-name"  # SP-Relative address.
-d <secondsToDelay> = 10              # Seconds between meter-value updates.
-r <runtime> = 2                      # Run application for 2 minutes.
```

*The following syntax shows how to run the application with some flags set.

```bash
./main -h 192.168.1.100:8080 -l lab:lab123 -n MY_METER -d 30 -r 5 -s
```
