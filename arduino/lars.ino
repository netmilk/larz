// the setup function runs once when you press reset or power the board

#include <stdlib.h>

#include <Bridge.h>
#include <FileIO.h>
#include <Process.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float temperatureIn;
float temperatureOut;

float temperature; 
float hysteresis = 1.0;

String previousPower = "0";
String heater = "0";

String currentSession = "";
String sessionsPath = "/mnt/sda1/arduino/www/sessions/";
String sessionIndexFilename = "sessions.txt";
String delimiter = ";";
String sessionSuffix = ".csv";

unsigned long writeInterval = 10000;
unsigned long lastWrite = 0;  

Process date;

void setup() {
  Bridge.begin();
  FileSystem.begin();
  
  // initialize digital pins as output
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  char sessionsPathChar[256];
  sessionsPath.toCharArray(sessionsPathChar, 256);

  FileSystem.mkdir(sessionsPathChar);

  Bridge.put(String("power"), String(0));
  Bridge.put(String("heater"), heater);  
  Bridge.put(String("hysteresis"), String(hysteresis));
  Bridge.put(String("session"), currentSession );
}

String getTimestamp() {
  date.begin("date");
  date.addParameter("+%s");
  date.run();
  while(date.running());
  String timestampString = date.readString();
  timestampString.replace("\n","");
  return timestampString;
}

void turnOn() {
  digitalWrite(12, HIGH);  
  digitalWrite(11, HIGH);  
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);   
  heater = String(1);
  Bridge.put(String("heater"), heater);
}

void turnOff() {
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);    
  heater = String(0);
  Bridge.put(String("heater"), heater);
}

void getTemperatures() {
  temperatureOut = sensors.getTempCByIndex(0);
  temperatureIn = sensors.getTempCByIndex(1);

  Bridge.put(String("temperature_out"), String(temperatureOut));
  Bridge.put(String("temperature_in"), String(temperatureIn));
  sensors.requestTemperatures();
}

void thermostat() {
  char temperatureChar[10];
  Bridge.get("temperature", temperatureChar, 10);
  temperature = atof(temperatureChar);
  
  if (temperatureIn > (temperature + hysteresis)) {
   turnOff();
  
  } else if (temperatureIn < (temperature - hysteresis)) { 
   turnOn();
  }

  writeLog();
}

void writeLog() {
  if(millis() < lastWrite) {
    //hnadle overflow - last write is in future
    lastWrite = millis();
  }

  if(millis() > (lastWrite + writeInterval)) {
    String line = String(heater + delimiter + temperatureIn + delimiter + temperatureOut);
     
    char sessionFileChar[256];
    String(sessionsPath + currentSession + sessionSuffix).toCharArray(sessionFileChar, 256);
    File sessionFile = FileSystem.open(sessionFileChar, FILE_APPEND);    
    sessionFile.println(line);
    sessionFile.close();
    lastWrite = millis();
  }
}


void endSession() {
 currentSession = "";
 Bridge.put(String("session"), currentSession);
}

void startSession() {
 currentSession = getTimestamp();
 Bridge.put(String("session"), currentSession);
 addSessionToIndex(currentSession);
}


void addSessionToIndex(String session) {
 char sessionsIndexFileChar[256];
 String(sessionsPath + sessionIndexFilename).toCharArray(sessionsIndexFileChar, 256);
 File sessionsIndexFile = FileSystem.open(sessionsIndexFileChar, FILE_APPEND);
 sessionsIndexFile.println(session);
 sessionsIndexFile.close();
}

// the loop function runs over and over again forever
void loop() {
  getTemperatures();
  
  char powerChar[10];
  Bridge.get("power", powerChar, 10);
  String power = String(powerChar);
  
  if(power == "1"){
    if(previousPower == "0"){
      startSession();
    }
    
    thermostat();

  } else {
    if(previousPower == "1"){
      endSession(); 
    }
    
    turnOff();
  }

  previousPower = power;
}
