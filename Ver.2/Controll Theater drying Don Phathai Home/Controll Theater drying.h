#define BLYNK_TEMPLATE_ID "TMPL6y1ykabJR"
#define BLYNK_TEMPLATE_NAME "Building2"
#define BLYNK_AUTH_TOKEN "GNoegbNIrh_wmll8hpQHt46r1qZZP-cA"

#define BLYNK_PRINT Serial
#define BLYNK_FIRMWARE_VERSION "0.2.0"
#define BLYNK_DEBUG 
#define APP_DEBUG
 
/*RELAY 4 Channel */
#define RELAY_1  D3
#define RELAY_2  D5
#define RELAY_3  D6
#define RELAY_4  D7

/*SHT1X*/
#define dataPin  D1
#define clockPin D2

#define LED_WIFI D4

/* Library */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SHT1x-ESP.h>
// #include <SimpleTimer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <Ticker.h>


/* Connection */
char auth[] = BLYNK_AUTH_TOKEN; //การประกาศการเชื่อมต่อ Blynk

const char *SSID_NAME = "Smart Don Phathai Home";
const char *PASSWORD = "";

// int first_run = 0;

float temp_c;    // ใช้เก็บค่าจากเซนเซอร์
float humidity;  // ใช้เก็บค่าจากเซนเซอร์

float Input_humidityOpen = 55;
float Input_humidityClose = 55;

float Input_TempOpen = 55;
float Input_TempClose = 55;

SHT1x sht1x(dataPin, clockPin);
// SimpleTimer timer_run;
BlynkTimer timer;

WiFiManager wifiManager;
Ticker tickerWifi;

