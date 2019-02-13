void WIFI_Connect()
{
  WiFi.disconnect();
  
  //LCD Messages
  lcd.clear();
  lcd.home ();
  lcd.print("Connecting to: ");
  lcd.setCursor ( 0, 1 );
  lcd.print(ssid);

  //Serial Messages
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (oledSuccess) {
    drawOLED("wifi0");
  }

  //Connect
  WiFi.begin(ssid, password);
  int oledProgress = 1;
  String oledWIFI;
  while(WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print( "." );
    if (oledSuccess) {
      oledWIFI = "wifi";
      oledWIFI += ((oledProgress++)%4);
      drawOLED(oledWIFI);
    }
    delay(250);
  }
  
  //LCD Connected Messages
  lcd.clear();
  lcd.setCursor ( 0, 1 );
  lcd.print("Connected!");

  //OLED Connected Messages
  #ifdef ENABLEOLED
      drawOLED("wificheck");
  #endif

  //Serial Connected Messages
  Serial.println("Connected!");
  delay(500);
}
