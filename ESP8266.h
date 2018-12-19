#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//https://github.com/knolleary/pubsubclient/blob/master/src/PubSubClient.h
// Update these with values suitable for your network.
const char* ssid = "XX";
const char* password = "XX";
const char* mqtt_server = "XX";
const char* mqtt_user_name = "XX";
const char* mqtt_user_password ="XX";

WiFiClient espClient;
PubSubClient client(espClient);

int SwitchedPin = 0;
String switch1;
String strTopic;
const char* strStateTopic = "ha/test1/state";
const char* strCommandTopic = "ha/test1/command";

int tryCount = 0;

String strPayload;
void setup_wifi() {
 Serial.begin(115200);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  strTopic = String((char*)topic);
  
  Serial.println(); 
  Serial.print("topic:");  
  Serial.print(strTopic);
  
  switch1 = String((char*)payload);

  Serial.print("message:");  
  Serial.println(switch1);
  
  if(strTopic == "ha/switch1")
    {
   // switch1 = String((char*)payload);
    if(switch1 == "ON")
      {
        Serial.println("ON");
        digitalWrite(SwitchedPin, HIGH);
      }
    else
      {
        Serial.println("OFF");
        digitalWrite(SwitchedPin, LOW);
      }
    }
}
 
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient",mqtt_user_name,mqtt_user_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      bool subscribeStateResult = client.subscribe(strStateTopic);
      Serial.print("subscribeStateResult:");
      Serial.print(subscribeStateResult);
     
      bool publishResult = client.publish(strCommandTopic,"ON");
      Serial.print("publishResult:");
      Serial.print(publishResult);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print(" tried times:");
      Serial.print(++tryCount);
      Serial.println(" try again in 5 seconds");
      
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(SwitchedPin, OUTPUT);
  digitalWrite(SwitchedPin, LOW);
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
