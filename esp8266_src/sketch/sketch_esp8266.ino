/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  Sample Code from: 
  https://examples.blynk.cc/?board=ESP8266&shield=ESP8266%20WiFi&example=Widgets%2FLED%2FLED_Blink
  
  Blynk using a LED widget on your phone!

  App project setup:
    LED widget on V1
    
 *************************************************************/

/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial
#define GPIO_LED_D0 16


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/***OTA Code Include***/
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
/********************/


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ADD_AUTH_TOKEN_HERE";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ADD_WIFI_SSID_HERE";
char pass[] = "ADD_WIFI_PASSKEY_HERE"

WidgetLED led1(V1);

BlynkTimer timer;

// V1 LED Widget is blinking
void blinkLedWidget()
{
  if (led1.getValue()) {
    led1.off();
    //digitalWrite(GPIO_LED_D0, LOW);   // Turn the LED on (Note that LOW is the voltage level )
//    Serial.println("LED on V1: off");
  } else {
    led1.on();
    //digitalWrite(GPIO_LED_D0, HIGH);   // Turn the LED on (Note that LOW is the voltage level )
//    Serial.println("LED on V1: on");
  }
}

void setup()
{
  /**************OTA includes***************/
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  //  ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  /**************************************/
  // Debug console
  //Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,178,23), 8080);

  timer.setInterval(1000L, blinkLedWidget);

  pinMode(GPIO_LED_D0, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void loop()
{
  Blynk.run();
  timer.run();
  ArduinoOTA.handle();
}
