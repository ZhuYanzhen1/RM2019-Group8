#include <ESP8266WiFi.h>
#define MAX_SRV_CLIENTS 3

const char *ssid = "RC_Transmitter";
const char *password = "abc0987654321";
WiFiServer server(8080);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup()
{
  Serial.begin(115200);
  delay(10);
  //Serial.print("Configuring access point...");  
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  server.begin();
  server.setNoDelay(true);
}

void loop()
{
  uint8_t i;
  if(server.hasClient())
  {
  for(i=0; i <MAX_SRV_CLIENTS;i++)
  {
    if(!serverClients[i]||!serverClients[i].connected())
    {
      if(serverClients[i]) serverClients[i].stop();
      serverClients[i]=server.available();
      continue;
    }
  }
  WiFiClient serverClient=server.available();
  serverClient.stop();
  }
  for (i=0; i<MAX_SRV_CLIENTS;i++)
  {
    if (serverClients[i]&&serverClients[i].connected())
    {
      digitalWrite(16,0);
      if(serverClients[i].available())
      {
        while(serverClients[i].available())
        Serial.write(serverClients[i ].read());
      }
    }
  }
  if (Serial.available())
  {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    for (i = 0;i < MAX_SRV_CLIENTS;i++)
    {
      if(serverClients[i]&&serverClients[i].connected())
      {
        serverClients[i].write(sbuf,len);
        delay(1);
      }
    }
  }
}
