int httpCode;
const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 400;
void queryWeather() {
  httpCode = http.GET();
  Serial.println(httpCode);                                                       
  while (httpCode <= 0) {
    Serial.print("!");
    delay(20);
  }
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(http.getString());
  weather0_main = root["weather"][0]["main"].asString();
  main_temp = root["main"]["temp"].as<float>() - 273;
  main_pressure = root["main"]["pressure"].as<int>();
  main_humidity = root["main"]["humidity"].as<int>();
  wind_speed = (root["wind"]["speed"].as<float>() * 3600) / 1000;
}
