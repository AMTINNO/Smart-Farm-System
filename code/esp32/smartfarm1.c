#include <WiFi.h>
#include <PubSubClient.h>
#include "time.h"                // Required for NTP 
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>     //The basedline lib from Adafruit
#include <DHT.h>                 //add-on DHT sensor
#include <DHT_U.h>               //universal  dht    
#define DHTTYPE    DHT11         // DHT 22 (AM2302)
#define ESP_ID        "001"
#define WIFI_STA_NAME "SIAM Hi -Speed"
#define WIFI_STA_PASS ""
//#define WIFI_STA_NAME "DSEL208_2G"
//#define WIFI_STA_PASS "piglet1234"
#define MQTT_SERVER   "smartfarm.eazycloud.xyz"   // mqtt broker @ Digital System Engineering Laboratory,  Faculty of Engineering,  Siam university
#define MQTT_PORT     1883
#define MQTT_USERNAME "dsel0"         // Authenticated by usernnname & password
#define MQTT_PASSWORD "piglet1234"    //
#define MQTT_NAME     "ESP32.xx.yyy"  // เวลาใช้งานให้ใส่ตัวเลขแทน xx ด้วยหมายเลขลำดับรายชื่อของท่าน และ  yyy ด้วยหมายเลขบอร์ด เพื่อป้องกันปัญหาชื่อซ้ำกัน
#define LED_PIN 2
#define CH1_PIN 25
#define CH2_PIN 26
#define CH3_PIN 32
#define CH4_PIN 33

#define DHT_PIN 27
#define SOIL_PIN 34

#define PERIOD 10000


#define INI 0
#define WAITING 1
#define STAYWAITING 2
#define DHTS 3
#define SOIL 4
#define ONCH1 5
#define ONCH2 6
#define ONCH3 7
#define ONCH4 8
#define OFFCH1 9
#define OFFCH2 10
#define OFFCH3 11
#define OFFCH4 12
#define PUBLISH 13
DynamicJsonDocument doc(1024);
// global varible and set intitial value
int state=INI;
int led_status=0;
int CH1=0;
int CH2=0;
int CH3=0;
int CH4=0;
float HUMI=20.0;
float TEMP=27.0;
int MOIS=32767;
int ActiveLow[] = {1, 0};
const char* ntpServer = "pool.ntp.org";    // NTP SERVER
const long  gmtOffset_sec = 25200;         // Timezone Bangkok GMT+ 7  ( 7hrs *60 mins *60secs)
const int   daylightOffset_sec = 0;        // There is no Daylight saving time in Thailand.


unsigned long myTime; 
WiFiClient client;                  // ใช้ lib WIFI
PubSubClient mqtt(client);          // ใช้ lib mqtt

DHT_Unified dht(DHT_PIN, DHTTYPE);   // เซต ตัวเซนเซอร์

void printLocalTime()   //for display
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

char* get_time_str(void){   // for payload
  // string holding well formated time.
  static char my_time_string[60];
  // structure holding the time
  // defination is as follow
  //     struct tm
  //    {
  //      int tm_sec;  // seconds from 0 to 59
  //      int tm_min;  // minutes from 0 to 59
  //      int tm_hour; // hours from 0 to 23
  //      int tm_mday; // day of the month from 1 to 30
  //      int tm_mon;  // month of the year from 0 to 11
  //      int tm_year; // number of years from 1900
  //      ... other variables not in the scope of the tutorial..
  //    };
  struct tm my_time;
  // read the time into timeinfo structure.
  if(!getLocalTime(&my_time)){
    Serial.println("Failed to obtain time");
    return NULL;
  }
  snprintf(my_time_string, // string to add the formated time to.
           60, // string length to prevent overflow
           "%04d-%02d-%02d %02d:%02d:%02d", // time string format
           (my_time.tm_year + 1900),
           (my_time.tm_mon  + 1),
            my_time.tm_mday,
            my_time.tm_hour,my_time.tm_min,my_time.tm_sec);

  // return the formated time string
  return my_time_string;     
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char*)payload;

  Serial.println("callback in topic [" + topic_str + "]: " + payload_str);
  
    DeserializationError error = deserializeJson(doc, payload_str);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  
  if(doc["id"]==ESP_ID) {
    Serial.println("\n\n");
    Serial.println("-----------------------------");
    Serial.print("Getting command from MQTT Server");
    if (doc["ch1"]=="on")   { state=ONCH1;  Serial.println(" CH1 On");}
    if (doc["ch1"]=="off")  { state=OFFCH1; Serial.println(" CH1 Off");}
    if (doc["ch2"]=="on")   { state=ONCH2;  Serial.println(" CH2 On");}
    if (doc["ch2"]=="off")  { state=OFFCH2; Serial.println(" CH2 Off");}
    if (doc["ch3"]=="on")   { state=ONCH3;  Serial.println(" CH3 On");}
    if (doc["ch3"]=="off")  { state=OFFCH3; Serial.println(" CH3 Off");}
    if (doc["ch4"]=="on")   { state=ONCH4;  Serial.println(" CH4 On");}
    if (doc["ch4"]=="off")  { state=OFFCH4; Serial.println(" CH4 Off");}
    Serial.println("-----------------------------");
   }
  
}



void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);  // เปิดใช้ Serial port 
  pinMode(LED_PIN, OUTPUT);
  pinMode(CH1_PIN,OUTPUT);
  pinMode(CH2_PIN,OUTPUT);
  pinMode(CH3_PIN,OUTPUT);
  pinMode(CH4_PIN,OUTPUT);
  
  Serial.println( "\n");
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
 


  mqtt.setServer(MQTT_SERVER, MQTT_PORT);   // connect to mqtt broker
  mqtt.setCallback(callback);               // set service for sub
  state=INI;



}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN,led_status);
  led_status=!led_status;
  delay(500);

switch (state) {

case INI: { 
            Serial.println("Initializing");
            digitalWrite(CH1_PIN,ActiveLow[CH1]);
            digitalWrite(CH2_PIN,ActiveLow[CH2]);
            digitalWrite(CH1_PIN,ActiveLow[CH3]);
            digitalWrite(CH2_PIN,ActiveLow[CH4]);
            Serial.print(" CH1:");
            Serial.print(ActiveLow[CH1]);
            Serial.print(", CH2:");
            Serial.print(ActiveLow[CH2]);
            Serial.print(", CH3:");
            Serial.print(ActiveLow[CH3]);
            Serial.print(", CH4:");
            Serial.println(ActiveLow[CH4]);


            state=WAITING;
            break; }
case WAITING: { 
                myTime = millis();
                Serial.print("Waiting State ");
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());
                state=STAYWAITING;
                delay(500);
                break;}

case STAYWAITING: {
                if ((millis()-myTime)>=PERIOD) {state=DHTS;myTime=millis();} 
                //delay(500);
                break;}
case DHTS: {
            Serial.println("Reading DHT sensor");
            int err=1;int retry=0;
            sensors_event_t event;
            Serial.println("GET TEMP");
            while (err==1) {
                dht.temperature().getEvent(&event);
                if (isnan(event.temperature)) {
                   err=1; 
                   delay(100);
                   Serial.print(".");
                   retry+=1;
                   if (retry>=20) err=0;}
                else {TEMP=event.temperature;err=0;
                      Serial.println();} } 
            err=1;retry=0;
            Serial.println("GET HUMI");
            while (err==1) {    
                dht.humidity().getEvent(&event);
                if (isnan(event.relative_humidity)) {
                  err=1;
                  delay(100);
                  Serial.print(".");
                   retry=retry+1;
                   if (retry>=20) {err=0;}
                  }
                else {HUMI=event.relative_humidity;err=0;
                      Serial.println();}}
                  
            Serial.print("Time Stamp: ");
            Serial.println(get_time_str());

            state=SOIL;  
            break; }
case SOIL: {
            Serial.println("Reading Soil sensor");
            MOIS = analogRead(SOIL_PIN);
            Serial.print("Soil : ");
            Serial.println(MOIS);

            state=PUBLISH;
            break; }
case ONCH1: {
              Serial.println("Turn CH1 On");
              CH1=1;
              digitalWrite(CH1_PIN,ActiveLow[CH1]);
              state=PUBLISH;
              break; }
case ONCH2: {
              Serial.println("Turn CH2 On");
              CH2=1;
              digitalWrite(CH2_PIN,ActiveLow[CH2]);
              state=PUBLISH;
              break; }
case ONCH3: {
              Serial.println("Turn CH3 On");
              CH3=1;
              digitalWrite(CH3_PIN,ActiveLow[CH3]);
              state=PUBLISH;
              break; }
case ONCH4: {
              Serial.println("Turn CH4 On");
              CH4=1;
              digitalWrite(CH4_PIN,ActiveLow[CH4]);              
              state=PUBLISH;
              break; }

case OFFCH1: {
              Serial.println("Turn CH1 Off");
              CH1=0;
              digitalWrite(CH1_PIN,ActiveLow[CH1]);
              state=PUBLISH;
              break; }
case OFFCH2: {
              Serial.println("Turn CH2 Off");
              CH2=0;
              digitalWrite(CH2_PIN,!CH2);              
              state=PUBLISH;  
              break; }
case OFFCH3: {
              Serial.println("Turn CH3 Off");
              CH3=0;
              digitalWrite(CH3_PIN,ActiveLow[CH3]);              
              state=PUBLISH;
              break; }
case OFFCH4: {Serial.println("Turn CH4 Off");
              CH4=0;
              digitalWrite(CH4_PIN,ActiveLow[CH4]);
              state=PUBLISH;
              break; }

case PUBLISH: {
 String  payload_str ;
                payload_str="{\"id\":\""+String(ESP_ID,3)+"\",\"temp\":\""+String(TEMP,2)+"\",\"humi\":\""+String(HUMI,2)+"\",\"mois\":\""+String(MOIS)+"\",\"timestamp\":\""+get_time_str()+"\",\"ch1\":\""+String(CH1)+"\",\"ch2\":\""+String(CH2)+"\",\"ch3\":\""+String(CH3)+"\",\"ch4\":\""+String(CH4)+"\",\"message\":\"bye\"}";
                
                char payload[150];
                payload_str.toCharArray(payload, 150);

                
                Serial.print("MQTT connection... ");
                 while(mqtt.connected() == false) {
                      
                      if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
                      Serial.println("connected");
                      mqtt.subscribe("smartfarm/control");

                      } else {
                      Serial.println("failed");
                      delay(500);
                      }
                  }
                mqtt.publish("smartfarm/status",payload);
                Serial.print("Topic smartfarm/status: " );
                Serial.println(payload);
                printLocalTime();
                state=WAITING;
                break;}
            }
            mqtt.loop();



}