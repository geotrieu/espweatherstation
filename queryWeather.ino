const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 400;
void queryWeather() {                                                  
  while (http.GET() <= 0) {
    Serial.print("!HTTP!");
    delay(100);
  }
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(http.getString());
  weather0_main = root["weather"][0]["main"].asString();
  main_temp = root["main"]["temp"].as<float>() - 273;
  main_pressure = root["main"]["pressure"].as<int>();
  main_humidity = root["main"]["humidity"].as<int>();
  wind_speed = (root["wind"]["speed"].as<float>() * 3600) / 1000;
  //Set the time for Serial Debugging/Reporting
  setTime(root["dt"].as<long>());

  //OLED Management
  if (oledSuccess) {
    drawOLED(weather0_main);
  }
}

