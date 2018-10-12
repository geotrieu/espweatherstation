void webServer() {
   WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    String operation = "";
    int numCRLF = 0;
    int tries;
    String postARGS = "";
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (header.length() <= 3) {
          operation += c;
        }

        if (c != '\r' && c != '\n') {
          currentLine += c;
        }

        if (numCRLF >= 1) {
          postARGS += c;
        }
        
        if (operation == "GET") {
          if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              printPage(client, "default", "default");
              break;
            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          }
        } else if (operation == "POS"){
          if (c == '\n') {
            if (currentLine.length() == 0) {
              numCRLF++;
            } else {
              currentLine = "";
            }
          }
        }
      } else if (numCRLF >= 1) {
        if (tries >= 10) {
          //Check for Post VALUES
          char post[postARGS.length()];
          postARGS.toCharArray(post, postARGS.length() + 1);
          String curArg = "";
          String inputSSID = "";
          String inputPW = "";
          int flag;
          for(int i = 0; i < postARGS.length(); i++) {
            if (post[i] == '&') {
              curArg = "";
            } else if (post[i] == '=') {
              continue;
            } else {
              curArg += post[i];
            }
            if (curArg == ""){
              flag = 0;
              continue;
            } else if (curArg == "ssid") {
              flag = 1;
              continue;
            } else if (curArg == "pswd") {
              flag = 2;
              continue;
            }
            if (flag == 1) {
              if (post[i] == '+') {
                inputSSID += " ";
              } else {
                inputSSID += post[i];
              }
            } else if (flag == 2) {
              if (post[i] == '+') {
                inputPW += " ";
              } else {
                inputPW += post[i];
              }
            }
          }
          printPage(client, inputSSID, inputPW);
          if (inputSSID != "" && inputPW != "") {
            //Success
            char arrayToStore[20];
            inputSSID.toCharArray(arrayToStore, inputSSID.length()+1);
            EEPROM.put(0x00, arrayToStore);
            inputPW.toCharArray(arrayToStore, inputPW.length()+1);
            EEPROM.put(0x23, arrayToStore);
            EEPROM.put(0x64, true);
            EEPROM.commit();
            delay(100);
            ESP.restart();
          }
          tries = 0;
          break;
        } else {
          tries++;
        }
      }
    }
    // Clear the header variable
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void printPage(WiFiClient client, String inputSSID, String inputPW) {
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            Serial.println("Sending HTTP");
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            //DOCUMENT BEGIN
            client.println("<!DOCTYPE html>");

            //HTML BEGIN
            client.println("<html>");

              //HEAD BEGIN
              client.println("<head>");
                client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("</head>");
              //HEAD END
              
              // CSS to style the on/off buttons 
              // Feel free to change the background-color and font-size attributes to fit your preferences
              /*client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #77878A;}</style>");*/
              
              //BODY BEGIN
              client.println("<body>");
              
                client.println("<div class=container-fluid>");
                  client.println("<div class=row>");
                    client.println("<div class=col-md-2>");
                    client.println("</div>");
                    client.println("<div class=col-md-8>");
                      client.println("<h3>");
                        client.println(macaddress);
                        client.println(" Setup");
                      client.println("</h3>");
                      client.println("<p>");
                        client.println("In order for this ESP to work, it must be provided details to connect to your home network.");
                        client.println("Below, is the required information in order for the ESP to communicate in your network.");
                      client.println("</p>");
                      client.println("<form method=post>");
                        client.println("SSID:<br>");
                        client.println("<input type=text name=ssid><br>");
                        client.println("Password:<br>");
                        client.println("<input type=password name=pswd><br><br>");
                        client.println("<input type=submit value=Submit>");
                      client.println("</form>");
                    client.println("</div>");
                  client.println("<div class=col-md-2>");
                  client.println("</div>");
                client.println("</div>");

                client.println("<div");
                if (inputSSID == "") {
                  //No Username
                  client.print("<h4>Error: No Username</h4>");
                } else if (inputPW == "") {
                  //No Password
                  client.print("<h4>Error: No Password</h4>");
                } else if (inputSSID != "default" && inputPW != "default") {
                  //Success
                  client.print("<h4>Success! Restarting device with network settings...</h4>");
                }
                client.println("</div>");
                
              client.println("</div>");
              
              client.println("</body>");
              //BODY END

            client.println("</html>");
            //HTML END
            
            // The HTTP response ends with another blank line
            client.println();
}
