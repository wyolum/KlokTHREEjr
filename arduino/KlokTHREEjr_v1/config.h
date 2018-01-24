#include <credentials.h>

// comment out the following two lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/************************* IO Setup *********************************/
//WiFiClientSecure client;
//Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_USERNAME, IO_KEY);
//Adafruit_MQTT_Subscribe time_seconds = Adafruit_MQTT_Subscribe(&mqtt, "time/seconds"); // used NTPClient instead
AdafruitIO_Feed *timezone = io.feed("Timezone1");
AdafruitIO_Feed *brightness = io.feed("klok_brightness");


bool FLIP_DISPLAY = true; // true: cord out bottom, false: cord out top
