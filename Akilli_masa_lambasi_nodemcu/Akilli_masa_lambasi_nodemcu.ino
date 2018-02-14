

#include<EEPROM.h>

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"



String wifikullaniciad = "ssid"; //WİFİ SİFRESİ BURADAN DEĞİŞTİRECEKSİN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< HASAN ABİ BURASI
String wifisifre = "internet şifresi";


#define ARB_SERVER      "192.168.1.102"
#define ARB_SERVERPORT  1883                   // use 8883 for SSL
#define ARB_USERNAME    "TestUser"
#define ARB_PW         "TestUser"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, ARB_SERVER, ARB_SERVERPORT, ARB_USERNAME, ARB_PW);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, "/cihaz", MQTT_QOS_1);

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, "/cihaz/cihaz1", MQTT_QOS_1);
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, "/cihaz/saat",MQTT_QOS_4);
/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void wifibegin()
{
  char *wk = const_cast<char*>(wifikullaniciad.c_str());
  char *ws = const_cast<char*>(wifisifre.c_str());
  WiFi.begin(wk, ws);
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&onoffbutton);
  
}
void MQTT_connect();

void setup() {
  
  Serial.begin(115200);


  wifibegin();
 
}



void loop() {

 
  MQTT_connect();
  if(mqtt.connected())
  {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(600))) {
    if (subscription == &onoffbutton) {
     
      String gelenveri = String((char *)onoffbutton.lastread);
       Serial.println(gelenveri);
  }
  }
 
 



 
    String gidecek = "1";
    char *vr = const_cast<char*>(gidecek.c_str());
    if (! photocell.publish(vr)) {
      // Serial.println(F("Failed"));
    } else {
      // Serial.println(F("OK!"));
    }
  }


}
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

 // Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  if ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    //Serial.println(mqtt.connectErrorString(ret));
    //Serial.println("Retrying MQTT connection in 5 seconds...");
   // mqtt.disconnect();
    //delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      //while (1);
    }
  }
  //Serial.println("MQTT Connected!");
  if (mqtt.connected()) {
    
  }
  
}
