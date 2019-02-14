# Webapp

## Introduction

The Web application subscribes to the IN-CSE Server each time there is DATA getting sent to it. And records the data into into 

## Basic Archeciture Design

The has the following endpoints:

**<RESOURCE_NAME>** is MY_METER for this project


### Webpages

`localhost:3000/` - Index Page

`localhost:3000/login` - Login Page with default login admin:admin

`localhost:3000/logout` - Logout page endpoint to logout

`localhost:3000/data?ae=<RESOURCE_NAME>` - Used to display data visually 


### API 

`localhost:3000/api/get/<RESOURCE_NAME>` - Gets the JSON Data for that paticular resource

`localhost:3000/status` - Checks to see the server status for the IN-CSE 

`localhost:3000/monitor` - OM2M posts to this endpoint after a Content Instance 

`localhost:3000/api/pingResource` - Pings an AE to its PING_METER container

`localhost:3000/api/getResource` - Returns a resource data within JSON Format

`localhost:3000/api/downloadResource` - Returns the JSON converted to an excel sheet

`localhost:3000/api/postDelete` - Used to delete an AE

`localhost:3000/api/postSubscribe` - Manually subscribes to a server



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

