#include "config.h"

// set timezone offset from UTC
int timeZone = -4; // UTC - 4 eastern daylight time (nyc)
int interval = 4; // trigger every X hours
int hour = 0; // current hour

void timecallback(uint32_t current) {
  //Serial.println("TimeCallback");
  Serial.println(current);

  // stash previous hour
  int previous = hour;

  // adjust to local time zone
  current += (timeZone * 60 * 60);

  // calculate current hour
  hour = (current / 60 / 60) % 24;

  // only trigger on interval
  if((hour != previous) && (hour % interval) == 0) {
    Serial.println("Run your code here");
  }

}

void setup() {

  Serial.begin(115200);
  delay(10);

  Serial.print(F("Adafruit IO Time Demo"));

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(" WiFi connected."));

  timefeed.setCallback(timecallback);
  mqtt.subscribe(&timefeed);

}

void loop() {

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // wait 10 seconds for subscription messages
  // since we have no other tasks in this example.
  mqtt.processPackets(10000);

  // keep the connection alive
  mqtt.ping();

}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}
