#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//https://github.com/knolleary/pubsubclient/blob/master/src/PubSubClient.h
// Update these with values suitable for your network.
const char* ssid = "Guoxiaomei-office";
const char* password = "xiaomei520";
const char* mqtt_server = "10.12.2.120";
const char* mqtt_user_name = "admin";
const char* mqtt_user_password ="admin1";

WiFiClient espClient;
PubSubClient client(espClient);

int SwitchedPin = 0;
String strTopic;
const char* strStateTopic = "ha/test2/state";
const char* strCommandTopic = "ha/test2/command";

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
  Serial.print("Message arrived [");
  strTopic = String((char*)topic);
  Serial.print(strTopic);
  Serial.print("] ");
  //结尾符，必须要，payload为指针类型，
  //如果不加的话，第一次接受到12345678，第二次接受到aa时，还是会输出aa345678,因为指针类型，在遇到截止符号时才结束
  payload[length] = '\0';
  String message = String((char*)payload);
  Serial.println(message);
   
  if(strTopic == "ha/switch1")
    {
   // switch1 = String((char*)payload);
    if(message == "ON")
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
     Serial.println("end");
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
      Serial.print("subscribeState topic:");
      Serial.print(strStateTopic);
      Serial.print(" result :");
      Serial.println(subscribeStateResult);
      /*
      bool publishResult = client.publish(strCommandTopic,"ON");
      Serial.print("publishResult:");
      Serial.println(publishResult);
      */
      tryCount = 0;

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
