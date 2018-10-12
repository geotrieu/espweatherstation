bool MQTT_connect() { 
 int8_t ret; 
 // Stop if already connected. 
 if (mqtt.connected()) { 
   return true; 
 } 
 Serial.print("Connecting to MQTT... "); 
 if ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected 
  Serial.println(mqtt.connectErrorString(ret)); 
  mqtt.disconnect(); 
  return false;
 } 
 Serial.println("MQTT Connected!");
 return true;
}  
