void loadNormal() {
  //Initialize OLED
  #ifdef ENABLEOLED
    lcd.clear();
    lcd.home();
    lcd.print("Init OLED");
    Serial.println("Initializing OLED");
    if (oledSuccess = initOLED()) {
      lcd.setCursor ( 0, 1 );
      Serial.println("OLED Successfully initialized!");
      lcd.print("Success!");
    } else {
      lcd.setCursor ( 0, 1 );
      Serial.println("Initializing OLED failed!");
      lcd.print("Failed");
    }
    delay(500);
  #endif
  
  //Boot into EEPROM saved credentials
  WiFi.mode(WIFI_STA);
  WIFI_Connect();
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Initialized MQTT
  lcd.clear();
  lcd.home();
  lcd.print("Init MQTT");
  Serial.println("Initializing MQTT");
  lcd.setCursor ( 0, 1 );
  mqtt.subscribe(&weather_out);
  mqttConnected = MQTT_connect();
  if (mqttConnected) {
    Serial.println("MQTT Connected!");
    lcd.print("Connected");
  } else {
    Serial.println("MQTT Failed!");
    lcd.print("Failed");
  }
  delay(500);

  //Initialized Temperature Sensor
  lcd.clear();
  lcd.home();
  lcd.print("Init BMP280");
  Serial.println("Initializing BMP280");
  while (!bmp.begin()) {  
    lcd.setCursor ( 0, 1 );
    lcd.print("Failed");
    Serial.println("BMP280 Sensor Failed!");
    Serial.println("Retrying in 5 Seconds!");
    delay(5000);
  }
  Serial.println("BMP280 Sensor Connected!");
  lcd.setCursor ( 0, 1 );
  lcd.print("Connected!");
  delay(500);

  //Success
  Serial.println("Weather Station Loaded Successfully!");
  lcd.clear();
  lcd.home();
  lcd.print("Arduino WX");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("By: George Trieu ");
  delay(500);
}

