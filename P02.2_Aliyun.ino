#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>

#define PIN_LIGHT 4  

const char* ssid = "iPhone4Fans";    // your network SSID (name)
const char* password = "wy@661678";    // your network password (use for WPA, or use as key for WEP)


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[]    = "iot-06z00i6nd4au6cw.mqtt.iothub.aliyuncs.com";
int        port        = 1883;

const char inTopic[]   = "/sys/k28xzOfpxaK/esp32_dev/thing/service/property/set";
const char outTopic[]  = "/sys/k28xzOfpxaK/esp32_dev/thing/event/property/post";

const long interval = 10000;
unsigned long previousMillis = 0;

int count = 0;

String inputString ="";

void setup() {
  pinMode(4,OUTPUT);//Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print("."); }

  Serial.println();
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  mqttClient.setId("k28xzOfpxaK.esp32_dev|securemode=2,signmethod=hmacsha256,timestamp=1737077399458|");                    //mqtt 连接客户端id
  mqttClient.setUsernamePassword("esp32_dev&k28xzOfpxaK", "cce8ccdf5e38ba667825e323a35cc0139f613609f0e770050e6a5ad55b3c8321");        //mqtt 连接用户名、密码

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(inTopic);
  Serial.println();

  int subscribeQos = 1;

  mqttClient.subscribe(inTopic, subscribeQos);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(inTopic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(inTopic);
  Serial.println();
}

//{"deviceType":"CustomCategory","iotId":"fwJnX0wS4z4MbrzkyRlt000000","requestId":"1689847083906","checkFailedData":{},"productKey":"a1gfbOdKycU","gmtCreate":1689847087015,"deviceName":"app_dev","items":{"led":{"value":0,"time":1689847086996}}}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', duplicate = ");
  Serial.print(mqttClient.messageDup() ? "true" : "false");
  Serial.print(", QoS = ");
  Serial.print(mqttClient.messageQoS());
  Serial.print(", retained = ");
  Serial.print(mqttClient.messageRetain() ? "true" : "false");
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    char inChar =(char)mqttClient.read();
    inputString +=inChar;
    if(inputString.length()==messageSize) {

      DynamicJsonDocument json_msg(1024);
      DynamicJsonDocument json_item(1024);
      DynamicJsonDocument json_value(1024);

      deserializeJson(json_msg,inputString);

      String items = json_msg["items"];

      deserializeJson(json_item,items);
      String led = json_item["LED"];

      deserializeJson(json_value,led);
      bool value = json_value["value"];

      if(value ==0) {
        //关
        Serial.println("off");
        digitalWrite(4,LOW);
      } else {
        //开
        Serial.println("on");
        digitalWrite(4,HIGH);
      }
      inputString="";
    }


  }
  Serial.println();

  Serial.println();
}
void loop(){ mqttClient.poll();}
