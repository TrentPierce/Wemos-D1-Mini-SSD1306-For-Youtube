/*********************************************************************

This program is for a 64x48 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Written by Trent Pierce on 12-30-2017.
https://github.com/TrentPierce/Wemos-D1-Mini-SSD1306-For-Youtube
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <YoutubeApi.h>            // https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>           // https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! ------
char ssid[] = "SSID"; // your network SSID (name)
char password[] = "password";   // your network password

// google API key
// create yours: https://support.google.com/cloud/answer/6158862?hl=en
#define API_KEY "API Key"

// youtube channel ID
// find yours: https://support.google.com/youtube/answer/3250431?hl=en
#define CHANNEL_ID "UCsbUoLYjVhurR3UjUSk_fmw"


// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 1000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done

long subs = 0;

void setup()   {
  Serial.begin(57600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  IPAddress ip = WiFi.localIP();

}


void loop() {
if (millis() > api_lasttime + api_mtbs)  {
      display.display();
      display.clearDisplay();
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("Sub Count:");
      display.setTextSize(2);
      display.setCursor(25,20);
      subs = api.channelStats.subscriberCount;
      display.println(api.channelStats.subscriberCount);
      display.display();
    }
    api_lasttime = millis();
  }
}
