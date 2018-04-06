#include <AuthClient.h>
#include <MicroGear.h>
#include <MQTTClient.h>
#include <SHA1.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

const char* ssid     = "atz";
const char* password = "11111111";

#define APPID       "ubiproject"
#define GEARKEY     "YrQAHjAaFKXWnhi"
#define GEARSECRET  "l3hr4el6plFSOLxCIcBdTwfVB"
#define ALIAS       "nodemcu1"
#define TargetWeb "HTML_web"

WiFiClient client;
AuthClient *authclient;


int timer = 0;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen)
{
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen)
{
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

void setup() {
  Serial.begin(115200);

  //  dht.begin();
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);

  Serial.println("Starting...");

  if (WiFi.begin(ssid, password)) {

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //uncomment the line below if you want to reset token -->
    //    microgear.resetToken();
    microgear.init(GEARKEY, GEARSECRET, ALIAS);
    microgear.connect(APPID);
  }
}

void loop() {
  if (microgear.connected())
  {
    microgear.loop();
    Serial.println("connected");

    float Humidity = random(0, 100);
    float Temp = random(19, 37);  // Read temperature as Celsius (the default)
    String data = "/" + String(Humidity) + "/" + String(Temp);
    char msg[128];
    data.toCharArray(msg, data.length());
    Serial.println(msg);

    microgear.chat(TargetWeb , msg);
  }
  else
  {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
  }
  delay(1500);
}
