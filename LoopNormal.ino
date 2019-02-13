void loopNormal() {
  //Checks if the internet connection was broken
  if (WiFi.status() != WL_CONNECTED)
  {
      WIFI_Connect();
  }
  
  //Update Internet Weather & Outside Weather every 60 seconds
  if (currentTick == 6000){
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=toronto&appid=d6c59d818ec8c12f9bf85002f8fc87e4&mode=json");
    //webServer();
    queryWeather();
    Serial.print("Successfully obtained weather: ");
    Serial.print(year());
    Serial.print("/");
    Serial.print(month());
    Serial.print("/");
    Serial.print(day());
    Serial.print("@");
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.println(" UTC");
    mqttConnected = MQTT_connect(); 
    http.end();
    currentTick = 0;

    //Update Outside Temperature
    Adafruit_MQTT_Subscribe *subscription;
    bool tempReceived;
    while ((subscription = mqtt.readSubscription())) {
      if (subscription == &weather_out) {
        outTemp = (char *) weather_out.lastread;
        tempReceived = true;
      }
    }
    //Checks if outside temperature has been received, or else change info to XX.XX
    if (!tempReceived) outTemp = "XX.XX";
  } else {
    currentTick++;
  }
  //Print Internet Weather
  if (displayTick == 0) {
    lcd.clear();
    lcd.home();                   // go home
    lcd.print(weather0_main);
    lcd.setCursor (15, 0);
    lcd.print("i");
    lcd.setCursor ( 0, 1 );        // go to the next line
    windkph = String(wind_speed); 
    tph = windkph + "k/h";
    lcd.print (tph);
    lcd.setCursor ( 9, 1 );        // go to the next line
    iTemp = String(main_temp);
    lcd.print(iTemp);
    lcd.write(byte(0));
    lcd.print("C");
    String weStr = "Internet Weather: ";
    String finalStr = weStr + weather0_main;
    char finalChar[finalStr.length() + 1];
    finalStr.toCharArray(finalChar, finalStr.length() + 1);
    String wsStr = "Internet Wind Speed: ";
    String kphStr = " km/h";
    finalStr = wsStr + windkph + kphStr;
    char final2Char[finalStr.length() + 1];
    finalStr.toCharArray(final2Char, finalStr.length() + 1);
    String tempStr = "Internet Temperature: ";
    String celciusStr = "°C";
    finalStr = tempStr + iTemp + celciusStr;
    char final3Char[finalStr.length() + 1];
    finalStr.toCharArray(final3Char, finalStr.length() + 1);
    if (mqttConnected) {
      weather_esp.publish(finalChar);
      weather_esp.publish(final2Char);
      weather_esp.publish(final3Char);
    }
    displayTick++;
  //Print Real Temperatures
  } else if (displayTick == 500) {
    roomTemp = bmp.readTemperature();
    lcd.clear();
    lcd.home();                   // go home
    lcd.print("Rm:");
    lcd.setCursor (5, 0);
    lcd.print(roomTemp);
    lcd.write(byte(0));
    lcd.print("C");
    lcd.setCursor (15, 0);
    lcd.print("R");
    lcd.setCursor (0, 1);
    lcd.print("Out:");
    lcd.setCursor (5, 1);
    lcd.print(outTemp);
    lcd.write(byte(0));
    lcd.print("C");
    String weStr = "Room Temp: ";
    String finalStr = weStr + roomTemp;
    char finalChar[finalStr.length() + 1];
    finalStr.toCharArray(finalChar, finalStr.length() + 1);
    if (mqttConnected) {
      weather_esp.publish(finalChar);
    }
    displayTick++;
  } else if (displayTick >= 1000) {
    displayTick = 0;
  } else {
    displayTick++;
  }
}

