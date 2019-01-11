#include "TransparentXbeeWifiClient.h"
#include "MQTT.h"

Xbee xbee(Serial1);
TransparentXbeeWifiClient wifiClient(xbee);
MQTTClient mqttClient;


void connect() {
  Serial.print("\nconnecting...");

  mqttClient.setWill("arduino", "Desconectado");

  while (!mqttClient.connect("arduino", "lab", "lab1234")) {
    Serial.print(".");
    Serial.println(mqttClient.lastError());
    delay(1000);
  }

  Serial.println("\nconnected!");

  mqttClient.subscribe("arduino");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);

    xbee.setDebugSerial(Serial);
    mqttClient.begin("C0A80132", wifiClient);
    mqttClient.onMessage(messageReceived);

    connect();
}

void loop() {
    static unsigned long lastMillis = 0;

    mqttClient.loop();

    if(!mqttClient.connected()) {
        connect();
    }

    if (millis() - lastMillis > 5000) {
        lastMillis = millis();
        mqttClient.publish("arduino", "Arduino Alive");
    }
}