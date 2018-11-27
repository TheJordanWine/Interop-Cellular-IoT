# Webapp

## Introduction

The Web application subscribes to the IN-CSE Server each time there is DATA getting sent to it. And records the data into into 

## Basic Archeciture Design

The has the following endpoints:

**<RESOURCE_NAME>** is MY_METER for this project



`localhost:3000/` - Index Page

`localhost:3000/data?ae=<RESOURCE_NAME>` - Data view for a paticular resource

`localhost:3000/api/get/<RESOURCE_NAME>` - Gets the JSON Data for that paticular resource

`localhost:3000/status` - Checks to see the server status for the IN-CSE Server

`localhost:3000/monitor` - **TODO**




## How to install

### Prerequisites:
Make sure you have the LTS Version of NodeJS and NPM, to verify type in the terminal:

```sh
$ node -v
v10.13.0
$ npm -v
6.4.1
```


Navigate to the root directory of `webapp/`

And run the following command to install the dependcies:

```sh
$ npm install
```


## How to run

By default, the webapp will run on localhost port 3000 without HTTPS, you can edit it via the `app.js` file

Run the webapp with 
```
$ npm start
```

