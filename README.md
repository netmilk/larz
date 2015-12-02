# Larz

Get hot and steamy with Larz, your sauna control buddy. Larz is here to help you set and maintain a nice and toasty sauna for an ultimate sweating experience. Over wifi. And clouds of the internets. From the smartphone. Boots.

![Sauna emoji](https://i.guim.co.uk/img/static/sys-images/Guardian/Pix/pictures/2015/11/4/1446659930454/bff5484a-481b-4287-a6df-8c8c29f56ecf-2060x1236.jpeg?w=1920&q=85&auto=format&sharp=10&s=648362f3c38bb3df3aa09c25d3a6cd9e)

### What Exactly Can Larz Do?

Larz is simply the bestest sauna buddy. All of Larz's operations deal with Arduino Yun's HTTP interface for its key-value store. [Check out Arduino's guide for more information and examples](https://www.arduino.cc/en/Guide/ArduinoYun#toc5).

Some of Larz's basic features include:
- Retrieving current sauna temperature
- Checking the current temperature outside of the sauna
- Countdown to desired sauna temperature
- Powering on the sauna with desired temperature
- Powering off the sauna to the default temperature
- Review session logs of sauna activity along with temperature status

## Javascript Client Library / SDK

The SDK is encapsuling Larz's HTTP API with a async interface.

If the function is called with the `callback` function passed as the last argument, the `callack` is called with:
- the error object as a first argument or null if no error happend
- the return data as the second argument

### Creating the instance and connecting

```
var larz = new Larz("http://10.0.0.112");
```

### Get current status

```
larz.status(callback);
>
{
  "temperature_out" : "25.69",
  "heater" : "1",
  "session" : "1448927714",
  "temperature_in" : "26.06",
  "hysteresis" : "1.00",
  "temperature" : "27",
  "power" : "1"
}
```


### Turn the sauna on and off

Turn the sauna on

```
lars.turnOn(callback);
> true
```

Turn the sauna off
```
lars.turnOff(callback);
> true
```

### Set the temperature

```
lars.setTemperature(95, callback);
> true
```

### Sessions

#### Get the list of all available sessions

```
larz.sessions(callback);
> ['1448927628', '1448927714', '1448929716', '1449011515']
```

#### Get specific session data

```
lars.session("1448927628", callback);
>
{
  "1448927628": {
    "heater": true,
    "temperature_in": "27.3"
    "temperature_out:" "12.1"
  },
  "1448927648": {
    "heater": true,
    "temperature_in": "28.3"
    "temperature_out:" "12.1"
  },
  "1448927658": {
    "heater": true,
    "temperature_in": "29.3"
    "temperature_out:" "12.1"
  },
  ...
}
```

## HTTP API

To access Larz simply go to `http://10.0.0.112`.

### Auth

Larz is at your service when you use `basic` authentication with the username `root` along with using the same password as per our wifi.

### Checking the Status of Larz

Ask Larz how he's doing by using `/data/get`.

**Example Request:**

```
curl -s -u root:<password> http://10.0.0.112/data/get
```

**Example Response:**

```
{
   "value" : {
      "temperature_out" : "25.69",
      "heater" : "1",
      "session" : "1448927714",
      "temperature_in" : "26.06",
      "hysteresis" : "1.00",
      "temperature" : "27",
      "power" : "1"
   },
   "response" : "get"
}
```

#### Controlling the Sauna

To control the sauna's current state change the `boolean` value for the `power` key within the key value store.
- Power ON the sauna by using `1`
- Power OFF the sauna by using `0`

**Example Power ON Request:**
```
curl -s -u root:<password> http://10.0.0.112/data/put/power/1
```

**Example Power OFF Request:**
```
curl -s -u root:<password> http://10.0.0.112/data/put/power/0
```

#### Setting the Sauna Temperature

WARNING! The default temperature is very low, so even if you turn the sauna on, the heater will remain off. To make sure the sauna is nice and hot you'll have to set a higher temperature then the current temperature inside sauna to make sure the heater is heating!

Setting the temperature is updating the Arduino Yun HTTP key-value store key `temperature` with integer value representing the target temperature in degrees Celsius.

**Example Temperature Setting Request**
```
curl -s -u root:<password> http://10.0.0.112/data/put/temperature/27
```

### All Sauna Data with Larz

Larz stores the data for every sauna session.

#### Fetching All Sauna Sessions

Each line within the response represents one session and its start date and time in the [Unix time format](https://en.wikipedia.org/wiki/Unix_time)

Example Request:

```
curl -s -u root:<password> http://10.0.0.112/sd/sessions/sessions.txt
```

Example Response:

```
1448927628
1448927714
```

#### Fetching Sesssion Details

Session details are CSV-like data delimited by a semicolon (`;`) logged every 10 seconds.

After delimiting:
- the first value is a boolean indicating if the heater is on (`1`) or off (`0`)
- the second value is a float with inner temperature in degrees Celsius
- the third value is a float with outer temperature in degrees Celsius

The name of the `CSV` file is the session date (in unix time format) + the `.csv` fromat suffix.

e.g. `1448927628.csv` for the first item in the previous example.

**Example Request**

```
curl -s -u root:<password> http://10.0.0.112/sd/sessions/1448927628.csv
```

**Example Response**
```
0;26.38;25.81
0;26.31;25.75
0;26.25;25.75
0;26.25;25.81
0;26.25;25.75
0;26.25;25.75
0;26.25;25.75
0;26.31;25.75
0;26.31;25.75
0;26.31;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.38;25.75
0;26.31;25.75
0;26.31;25.75
0;26.31;25.75
0;26.31;25.81
0;26.31;25.81
0;26.31;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.88
0;26.25;25.81
0;26.25;25.88
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.31;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.81
0;26.25;25.75
0;26.31;25.81
0;26.31;25.81
0;26.31;25.81
0;26.31;25.81
0;26.38;25.88
0;26.38;25.88
```

## Arduino code

## Electronic parts & PCB

## Mechanical parts and boxes

### Prototyping

![Breadboard photo](https://raw.githubusercontent.com/netmilk/larz/master/img/IMG_3182.jpg)

## Yun Linux
