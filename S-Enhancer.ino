/*
███████╗      ███████╗███╗   ██╗██╗  ██╗ █████╗ ███╗   ██╗ ██████╗███████╗██████╗ 
██╔════╝      ██╔════╝████╗  ██║██║  ██║██╔══██╗████╗  ██║██╔════╝██╔════╝██╔══██╗
███████╗█████╗█████╗  ██╔██╗ ██║███████║███████║██╔██╗ ██║██║     █████╗  ██████╔╝
╚════██║╚════╝██╔══╝  ██║╚██╗██║██╔══██║██╔══██║██║╚██╗██║██║     ██╔══╝  ██╔══██╗
███████║      ███████╗██║ ╚████║██║  ██║██║  ██║██║ ╚████║╚██████╗███████╗██║  ██║
╚══════╝      ╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝╚═╝  ╚═╝
                                                                               

S-Enhancer is a sandal designed to prevent Ulcer formation on patients with Diabetes. 
This is the main control program used for the detection of long periods of time with high pressure on
specif areas of the foot. 

The project uses Blynk as a Data visualization tool on the user's smartphone. 

Andres Rico - 2018

*/
#define BLYNK_PRINT Serial //Include Blynk library. 
#include <ESP8266WiFi.h> //Include ESP8266 libraries. 
#include <BlynkSimpleEsp8266.h>

char auth[] = "d6817874e77b44f395071ff3150cdc0a"; //Autho token give by blynk to setup phone interface. 

char ssid[] = "102"; //WiFI credentials. Using a mobile hotspot is recommended. 
char pass[] = "pornhub.com";

BlynkTimer timer; // Create timer object for sending data over Blynk's server. 

///////////

int sensor1 = D8; //Ankle
int sensor2 = D7; //Front Left Sensor
int sensor3 = D6; //Front Right Sensor
int lecturasensores = A0; //Decalre analog pin for reading sensor data. 
int Threshold = 550; //Waiting treshold in milliseconds. 
float s1, s2, s3;
boolean swi1, swi2, swi3;
long starttime1, elapsedtime1, currenttime1;
long starttime2, elapsedtime2, currenttime2;
long starttime3, elapsedtime3, currenttime3;

//////////

int readsensor1() {
  digitalWrite(sensor1, HIGH);
  delay(10); 
  int v1 = analogRead(lecturasensores);
  digitalWrite(sensor1, LOW);
    return v1;
}

int readsensor2() {
  digitalWrite(sensor2, HIGH);
  delay(10);
  int v2 = analogRead(lecturasensores);
  digitalWrite(sensor2, LOW);
    return v2;
}

int readsensor3() {
  digitalWrite(sensor3, HIGH);
  delay(10);
  int v3 = analogRead(lecturasensores);
  digitalWrite(sensor3, LOW);
    return v3;
}

void myTimerEvent() {
    Blynk.virtualWrite(V0, s1);
    Blynk.virtualWrite(V1, s2);
    Blynk.virtualWrite(V2, s3);
    Serial.print(s1);
    Serial.print(" , ");
    Serial.print(s2);
    Serial.print(" , ");
    Serial.println(s3);
  }

void setup()
{  
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(200L, myTimerEvent);
  pinMode(sensor1, OUTPUT);
  pinMode(sensor2, OUTPUT);
  pinMode(sensor3, OUTPUT);
  swi1 = false;
}

void loop()
{
  Blynk.run();
  timer.run();
  s1 = ((readsensor1() * 185.022) / 120);
  s2 = ((readsensor2() * 185.022) / 120);
  s3 = ((readsensor3() * 185.022) / 120);

  if (swi1 == false && s1 > 140) {
    starttime1 = millis();
    swi1 = true;
  }

  if (swi2 == false && s2 > 140) {
    starttime2 = millis();
    swi2 = true;
  }

  if (swi3 == false && s3 > 140) {
    starttime3 = millis();
    swi3 = true;
  }

  if (swi1 == true && s1 < 140) {
    swi1 = false;
  }

  if (swi2 == true && s2 < 140) {
    swi2 = false;
  }

  if (swi3 == true && s3 < 140) {
    swi3 = false;
  }


  elapsedtime1 = millis() - starttime1;
  elapsedtime2 = millis() - starttime2;
  elapsedtime3 = millis() - starttime3;

  if (elapsedtime1 > 10000 && swi1 == true) { //Planta derecha
    Blynk.notify("There is irregular pressure on the front right of your foot.");
    swi1 = false;
  }

  if (elapsedtime2 > 10000 && swi2 == true) { //Tobillo
    Blynk.notify("There is irregular pressure on your ankle.");
    swi2 = false;
  }

  if (elapsedtime3 > 10000 && swi3 == true) { //Planta Izquierda
    Blynk.notify("There is irregular pressure on the front left of your foot.");
    swi3 = false;
  }
 
}

