/*
 * Blink
 * Turns on the onboard LED on for one second, then off for one second,
 * repeatedly.
 * This uses delay() to pause between LED toggles.
 */
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUM_LEDS 8
#define PIN 12
#define BRIGHTNESS 20

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Network SSID
const char* ssid = "ssid";
const char* password = "password";

uint16_t btime = now();
 
void setup() {
  // Connect WiFi
  Serial.begin(115200);
  delay(10);
 
  // Connect WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.hostname("Test-Wemos-D1");
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Print the IP address
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());

  pinMode(LED_BUILTIN, OUTPUT);  // initialize onboard LED as output

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  
}

void check_homelink() {
  HTTPClient http;    //Declare object of class HTTPClient
   
  //GET Data

  String data = "SELECT last(homelink_nearby) from vehicle_state";
  
  String Link = "http://192.168.90.3:8086/query?pretty=true&u=influx-user&p=influx-password&db=tesla&q=" + data;

  http.begin(WiFi, Link);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
}

void check_time() {
}
  

void loop() {
  digitalWrite(BUILTIN_LED, HIGH);  // turn on LED with voltage HIGH
  delay(1000);                      // wait one second
  digitalWrite(BUILTIN_LED, LOW);   // turn off LED with voltage LOW
  delay(1000);                      // wait one second
  if(WiFi.isConnected()){
  }
  rainbowCycle(20);
}
