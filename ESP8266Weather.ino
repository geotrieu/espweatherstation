  /* ESP8266 Web Server
 * By: George Trieu
 * Started On: May 15th, 2018
 * Last Edited On: February 12th, 2019
 * Purpose: To display the weather on the LCD display,
 *          and forward data to the Raspberry Pi using
 *          MQTT. */
// Load Wi-Fi library
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//Load I2C Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//Load Arduino JSON Library
#include <ArduinoJson.h>
//Load MQTT Libraries
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
//Load BMP280 Libraries
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
//Load Time Library
#include <Time.h>
#include <TimeLib.h>

//Network Details
char ssid[20];
char password[30];
bool normal;
String macaddress;

//OLED (BETA)
//Comment out if OLED is disabled
#define ENABLEOLED
bool oledSuccess = false;

//MQTT Details
#define MQTT_SERVER "192.168.1.3"
#define MQTT_PORT 1883                    
#define MQTT_USERNAME "" 
#define MQTT_PASSWORD "" 

//Pin Details
#define RESET_PIN 14

//BMP280 Sensor
Adafruit_BMP280 bmp; // I2C

//Web Server
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//HTTP Request
HTTPClient http;

// Create an ESP8266 WiFiClient class to connect to the MQTT server. 
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 
Adafruit_MQTT_Publish weather_esp = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/weather/roomesp");
Adafruit_MQTT_Subscribe weather_out = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/weather/outesp");

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte degree[8] = {
  B00000,
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
};

String weather0_main;
bool mqttConnected;
float main_temp;
int main_pressure;
int main_humidity;
float wind_speed;
String iTemp;
String pressure;
String humidity;
String windkph;
String tph;
double roomTemp;
String outTemp = "XX.XX";
int currentTick = 6000;
int displayTick;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(840);

  pinMode(RESET_PIN, INPUT_PULLUP);

  if (digitalRead(RESET_PIN) == LOW) {
    Serial.println("Resetting Settings...");
    for (int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
  }

  //OBTAIN NETWORK INFORMATION FROM EEPROM
  EEPROM.get(0x00, ssid);
  EEPROM.get(0x23, password);
  EEPROM.get(0x64, normal);
  
  //LCD Init
  lcd.begin(16,2);               // initialize the lcd 
  lcd.createChar(0, degree);
  lcd.clear();

  Serial.println(ssid);
  Serial.println(password);
  Serial.println(normal);

  //CHECK IF IT IS INITIAL SETUP
  if (!normal) {
    macaddress = "ESP-";
    macaddress += WiFi.macAddress().substring(0,8);
    char chrmac[macaddress.length()];
    macaddress.toCharArray(chrmac, macaddress.length() + 1);
    lcd.home();
    lcd.print("Weather Station");
    lcd.setCursor ( 0, 1 );
    lcd.print("Initial Setup");
    //Boot into Access Point Setup
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192,168,1,1), IPAddress(192,168,1,1), IPAddress(255, 255, 255, 0));
    WiFi.softAP(chrmac, "d6adcx2s");
    server.begin();
    return;
  } else {
    //Boot into EEPROM saved credentials
    WiFi.mode(WIFI_STA);
    WIFI_Connect();
  }
  delay(500);

  loadNormal();
}

void loop(){
  if (normal) {
    loopNormal();
  } else {
    if (displayTick == 0) {
      lcd.clear();
      lcd.home();
      lcd.print("Weather Station");
      lcd.setCursor ( 0, 1 );
      lcd.print("Initial Setup");
      displayTick++;
    } else if (displayTick == 500) {
      lcd.clear();
      lcd.home();
      lcd.print("AP: ");
      lcd.print(macaddress);
      lcd.setCursor ( 0, 1 );
      lcd.print("PW: d6adcx2s");
      displayTick++;
    } else if (displayTick >= 1000) {
      displayTick = 0;
    } else {
      displayTick++;
    }
    webServer();
  }
  delay(10);
}
