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

  //Connect
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print( "." );
    delay(250);
  }
  
  //LCD Connected Messages
  lcd.clear();
  lcd.setCursor ( 0, 1 );
  lcd.print("Connected!");

  //Serial Connected Messages
  Serial.println("Connected!");
}
