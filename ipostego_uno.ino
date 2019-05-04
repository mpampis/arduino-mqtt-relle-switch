#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
byte mac[] = {   0x00, 0xAB, 0xBC, 0xCA, 0xDE, 0x01 };
IPAddress ip(192, 168, 2, 203);
IPAddress server(192, 168, 2, 11);
EthernetClient ethClient;
PubSubClient client(ethClient);
const int buttonPin[] = {6, 7, 8, 9}; // switch pins
const int rellePin[] = {2, 3, 4, 5}; // relle pins
const int lenCnt = 4;
int switchNo = 0;
int switchStatus = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0; // previous state of the button
int relleStatus = 0;
char pin[5];
String str1, str2, str3;

void setup() {

  for (int p = 0; p < lenCnt; p++)
  {
    pinMode(buttonPin[p], INPUT); // Set the mode to INPUT
    pinMode(rellePin[p], OUTPUT); // Set the mode to OUTPUT

  }
  //   Serial.begin(9600);
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (int p = 0; p < lenCnt; p++)
  {
    buttonState = digitalRead(buttonPin[p]);
    lastButtonState = EEPROM.read(buttonPin[p]);
    relleStatus = EEPROM.read(buttonPin[p] - 4);
    if (buttonState != lastButtonState) {
      EEPROM.update(buttonPin[p] , buttonState);
      EEPROM.update(buttonPin[p] - 4, !relleStatus);
      digitalWrite(buttonPin[p] - 4, !relleStatus);

      //     Serial.println("loop");
      //  Serial.println(buttonPin[p]);
      //     Serial.println(lastButtonState);

      if (buttonPin[p] == 6 && relleStatus == 0) {
        client.publish("ipostego1", "ON");

      }
      else if (buttonPin[p]  == 6 && relleStatus == 1) {
        client.publish("ipostego1", "OFF");
      }
      if (buttonPin[p] == 7 && relleStatus == 0) {
        client.publish("ipostego2", "ON");

      }
      else if (buttonPin[p]  == 7 && relleStatus == 1) {
        client.publish("ipostego2", "OFF");
      }
      if (buttonPin[p] == 8 && relleStatus == 0) {
        client.publish("ipostego3", "ON");

      }
      else if (buttonPin[p]  == 8 && relleStatus == 1) {
        client.publish("ipostego3", "OFF");
      }
      if (buttonPin[p] == 9 && relleStatus == 0) {
        client.publish("ipostego4", "ON");

      }
      else if (buttonPin[p]  == 9 && relleStatus  == 1) {
        client.publish("ipostego4", "OFF");
      }


    }
    delay(100);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //   Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoipostego")) {
      //     Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("tele/ipostego/LWT", "Online");
      // ... and resubscribe
      client.subscribe("cmnd/ipostego/power");
    } else {
      //      Serial.print("failed, rc=");
      //      Serial.print(client.state());
      //      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  switchNo = (payload[0] & 0xf) * 10 + (payload[1] & 0xf);
  switchStatus = (payload[2] & 0xf);
  digitalWrite(switchNo, switchStatus);
  EEPROM.update(switchNo , switchStatus);
  //      Serial.println("callback");
  //   Serial.println(switchNo);
  if (switchNo == 02 && switchStatus == 1) {
    client.publish("ipostego1", "ON");

  }
  else if (switchNo == 02 && switchStatus == 0) {
    client.publish("ipostego1", "OFF");
  }
  if (switchNo == 03 && switchStatus == 1) {
    client.publish("ipostego2", "ON");

  }
  else if (switchNo == 03 && switchStatus == 0) {
    client.publish("ipostego2", "OFF");
  }
  if (switchNo == 04 && switchStatus == 1) {
    client.publish("ipostego3", "ON");

  }
  else if (switchNo == 04 && switchStatus == 0) {
    client.publish("ipostego3", "OFF");
  }
  if (switchNo == 05 && switchStatus == 1) {
    client.publish("ipostego4", "ON");

  }
  else if (switchNo == 05 && switchStatus == 0) {
    client.publish("ipostego4", "OFF");
  }
}


