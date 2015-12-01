# Larz

Sauna control buddy

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
larz.status(callback())
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
lars.turnOn(callback)
> true
```

Turn the sauna off
```
lars.turnOn(callback)
> true
```

### Sessions

#### Get the list of all available sessions

```
larz.sessions(callback)
> ['1448927628', '1448927714', '1448929716', '1449011515']
```

#### Get specific session data

```
lars.session("1448927628", callback)
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

Larz lives at the address `http://10.0.0.112`

### Auth

Larz will be at your service when you use `basic` authentication with username `root` and the same password as for our wifi.

### The status of Larz

Larz will tell you how's he doing when you ask him with `/data/get`

**Example request:**

```
curl -s -u root:<password> http://10.0.0.112/data/get
```

**Example response:**

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

### What can Larz do for you

All operations with Larz is just [dealing with Arduino Yun's HTTP interface for its key-value store](https://www.arduino.cc/en/Guide/ArduinoYun#toc5).

#### Turn the sauna on and off

Just change the `boolean` value for the `power` key in the key value store to `1` for on or to `0` for off.

**Turing on:**
```
curl -s -u root:<password> http://10.0.0.112/data/put/power/1
```

**Turning off:**
```
curl -s -u root:<password> http://10.0.0.112/data/put/power/0
```

#### Set the inner temperature

WARNING! The default temperature is very low, so even if you turn the sauna on, the heater will remain off. You have to set some higher temperature then the temperature inside sauna to turn tha sauna heater really on!

Setting temperature is updating the Arduino Yun HTTP key-value store key `temperature` with integer value represering the target temperature in degrees Celsius.

**Example**
```
curl -s -u root:<password> http://10.0.0.112/data/put/temperature/27
```

### All the data of Larz

Larz stores the data for every sauna session.

#### Fetch all the sessions

Each line in the response represents one session and its start date and time in the [unix time format](https://en.wikipedia.org/wiki/Unix_time)

Example Request:

```
curl -s -u root:<password> http://10.0.0.112/sd/sessions/sessions.txt
```

Example Response:

```
1448927628
1448927714
```

#### Fetch Sesssion details

Session datails are CSV-like data delimited by a semicolon (`;`) logged every 10 seconds.

After delimiting:
- the first value is a boolean indicating if the heater is on (`1`) or off (`0`)
- the second value is a float with inner temperature in degree Celsius
- the third value is a float with outer temperature in degree Celsius

The name of the `CSV` file is the session date  (in unix time format) + the `.csv` fromat suffix.

e.g. `1448927628.csv` for the first item in the previous example.

**Example**

```
curl -s -u root:hubert11 http://10.0.0.112/sd/sessions/1448927628.csv
```

**Example response**
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


## Aruino code

## Electronic parts & PCB

### Prototyping

![Breadboard photo](https://raw.githubusercontent.com/netmilk/larz/master/img/IMG_3182.jpg)

## Yun Linux







