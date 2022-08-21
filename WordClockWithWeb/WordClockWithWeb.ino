// ###########################################################################################################################################
// #
// # WordClock code for the thingiverse WordClock project: https://www.thingiverse.com/thing:4693081
// #
// # Code by https://github.com/N1cls and https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0 https://github.com/N1cls/Wordclock/blob/master/LICENSE.md
// #
// ###########################################################################################################################################


// ###########################################################################################################################################
// # Includes:
// #
// # You will need to add the following libraries to your Arduino IDE to use the project:
// # - Adafruit BusIO                 // by Adafruit:                     https://github.com/adafruit/Adafruit_BusIO
// # - Adafruit NeoPixel              // by Adafruit:                     https://github.com/adafruit/Adafruit_NeoPixel
// # - DS3231                         // by Andrew Wickert:               https://github.com/NorthernWidget/DS3231
// # - RTClib                         // by Adafruit:                     https://github.com/adafruit/RTClib
// # - WiFiManager                    // by tablatronix / tzapu:          https://github.com/tzapu/WiFiManager
// # - ESP8266Ping                    // by dancol90                      https://github.com/dancol90/ESP8266Ping
// #
// ###########################################################################################################################################
#include <ESP8266WiFi.h>              // Used to connect the ESP8266 NODE MCU to your WiFi
#include <DNSServer.h>                // Used for name resolution for the internal webserver
#include <ESP8266WebServer.h>         // Used for the internal webserver
#include <WiFiManager.h>              // Used for the WiFi Manager option to be able to connect the WordClock to your WiFi without code changes
#include <EEPROM.h>                   // Used to store the in the internal configuration page set configuration on the ESP8266 internal storage
#include <Adafruit_NeoPixel.h>        // Used to drive the NeoPixel LEDs
#include <time.h>                     // Used to get the time from the internet
#include <Wire.h>                     // Used to connect the RTC board
#include <ESP8266mDNS.h>              // Used for the internal update function
#include <ESP8266HTTPUpdateServer.h>  // Used for the internal update function
#include "RTClib.h"                   // Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <ESP8266Ping.h>              // Used to send ping requests to a IP-address (of your smart phone) to detect if you have left your home
#include "settings.h"                 // Settings are stored in a seperate file to make to code better readable and to be able to switch to other settings faster
// ###########################################################################################################################################


// ###########################################################################################################################################
// # Version number of the code:
// ###########################################################################################################################################
const char* WORD_CLOCK_VERSION = "V4.1";


// ###########################################################################################################################################
// # Declartions and variables used in the functions:
// ###########################################################################################################################################
String header; // Variable to store the HTTP request
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);       // LED strip settings
RTC_DS3231 rtc;                                                                           // rtc communication object
int lastRequest = 0;                                                                      // Variable to control RTC requests
int rtcStarted = 0;                                                                       // Variable to control whether RTC has been initialized
int delayval = 250;                                                                       // delay in milliseconds
int iYear, iMonth, iDay, iHour, iMinute, iSecond, iWeekDay;                               // variables for RTC-module read time:
String timeZone = DEFAULT_TIMEZONE;                                                       // Time server settings
String ntpServer = DEFAULT_NTP_SERVER;                                                    // Time server settings
String UpdatePath = "-";                                                                  // Update via Hostname
String UpdatePathIP = "-";                                                                // Update via IP-address
ESP8266HTTPUpdateServer httpUpdater;                                                      // Update server
IPAddress remote_ip(PING_IP_ADDR_O1, PING_IP_ADDR_O2, PING_IP_ADDR_O3, PING_IP_ADDR_O4);  // IP-addres to monitor 2x per minute to turn LEDs off if IP-addres is offline for a defined time
int PING_ATTEMPTS = PING_TIMEOUTNUM;                                                      // Attempts of missed PING requests
bool LEDsON = true;                                                                       // Global flag to turn LEDs on or off - Used for the ping function


// ###########################################################################################################################################
// # Parameter record to store to the EEPROM of the ESP8266:
// ###########################################################################################################################################
struct parmRec
{
  int  pRed;
  int  pGreen;
  int  pBlue;
  int  pIntensity;
  int  pIntensityNight;
  int  pShowDate;
  char pdisplayonmaxMO;
  char pdisplayonminMO;
  char pdisplayonmaxTU;
  char pdisplayonminTU;
  char pdisplayonmaxWE;
  char pdisplayonminWE;
  char pdisplayonmaxTH;
  char pdisplayonminTH;
  char pdisplayonmaxFR;
  char pdisplayonminFR;
  char pdisplayonmaxSA;
  char pdisplayonminSA;
  char pdisplayonmaxSU;
  char pdisplayonminSU;
  int  pdisplayoff;
  int  puseNightLEDs;
  int  puseupdate;
  int  puseledtest;
  int  pusesetwlan;
  int  puseshowip;
  int  pswitchRainBow;
  int  pswitchLEDOrder;
  int  pwchostnamenum;
  int  pDCWFlag;
  int  pBlinkTime;
  int pPING_IP_ADDR_O1;
  int pPING_IP_ADDR_O2;
  int pPING_IP_ADDR_O3;
  int pPING_IP_ADDR_O4;
  int pPING_TIMEOUTNUM;
  int pPING_DEBUG_MODE;
  int pPING_USEMONITOR;
  char pTimeZone[50];
  char pNTPServer[50];
  int  pCheckSum;             // This checkSum is used to find out whether we have valid parameters
} parameter;


// ###########################################################################################################################################
// # Setup function that runs once at startup of the ESP8266:
// ###########################################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Start Monitor...");
  Serial.print("WordClock Version: ");
  Serial.println(WORD_CLOCK_VERSION);

  pixels.begin();  // Init the NeoPixel library

  readEEPROM(); // get persistent data from EEPROM

  // LED test --> no blank display if WiFi was not set yet:
  if (useledtest) {
    Serial.println("Display Test...");
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      setLED(i, i, 1);
      pixels.show();
      delay(25);
    }
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      setLED(i, i, 0);
      pixels.show();
      delay(25);
    }
    delay(1500);
  }

  // Show "SET WLAN" --> no blank display if WiFi was not set yet:
  if (usesetwlan) {
    Serial.println("Show SET WLAN...");
    setLED(6, 6, 1);      // S
    setLED(12, 12, 1);    // E
    setLED(24, 24, 1);    // T
    setLED(63, 63, 1);    // W
    setLED(83, 83, 1);    // L
    setLED(84, 84, 1);    // A
    setLED(93, 93, 1);    // N
    setLED(110, 110, 1);  // Corner 1
    setLED(111, 111, 1);  // Corner 2
    setLED(112, 112, 1);  // Corner 3
    setLED(113, 113, 1);  // Corner 4
    pixels.show();
  }

  // WiFiManager:
  bool res;
  WiFiManager wifiManager;
  dunkel(); // Switch display black
  configNTPTime(); // Set timezone
  wifiManager.setConfigPortalTimeout(AP_TIMEOUT); // Max wait for 3 minutes
  Serial.println("Before autoConnect...");
  res = wifiManager.autoConnect(DEFAULT_AP_NAME);
  if (!res) {
    Serial.println("Failed to connect to WiFi");
  }
  else {
    Serial.println("Connected to WiFi.");
    if (useshowip)
    {
      showIP();
    }
  }

  // Web update function setup:
  if (useupdate) {
    MDNS.begin(wchostname + wchostnamenum);
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", 2022);
    MDNS.addService("http", "tcp", 80);
    UpdatePath = "http://" + String(wchostname + wchostnamenum) + ".local:2022/update";
    Serial.print("Web Update Link: ");
    Serial.println(UpdatePath);
    UpdatePathIP = "http://" + WiFi.localIP().toString() + ":2022/update";
    Serial.print("Web Update Link via IP-address: ");
    Serial.println(UpdatePathIP);
  }
  server.begin();
}


// ###########################################################################################################################################
// # Loop function which runs all the time after the startup was done:
// ###########################################################################################################################################
void loop() {
  // Check, whether something has been entered on Config Page
  checkClient();
  ESP.wdtFeed();  // Reset watchdog timer
  handleTime(); // handle NTP / RTC time
  delay(750);

  if (switchRainBow) { // RainBow effect active - color change every new minute
    if (iSecond == 0) {
      redVal = random(255);
      greenVal = random(255);
      blueVal = random(255);
    }
  } else {
    redVal    =  parameter.pRed;
    greenVal  =  parameter.pGreen;
    blueVal   =  parameter.pBlue;
  }

  // Show the display only during the set Min/Max time if option is set
  if (displayoff) {
    switch (iWeekDay) {
      case 0:     // Sunday
        DayNightMode(displayonminSU, displayonmaxSU);
        break;
      case 1:     // Monday
        DayNightMode(displayonminMO, displayonmaxMO);
        break;
      case 2:     // Tuesday
        DayNightMode(displayonminTU, displayonmaxTU);
        break;
      case 3:     // Wednesday
        DayNightMode(displayonminWE, displayonmaxWE);
        break;
      case 4:     // Thursday
        DayNightMode(displayonminTH, displayonmaxTH);
        break;
      case 5:     // Friday
        DayNightMode(displayonminFR, displayonmaxFR);
        break;
      case 6:     // Saturday
        DayNightMode(displayonminSA, displayonmaxSA);
        break;
    }
  }
  else
  {
    pixels.setBrightness(intensity); // DAY brightness
    ShowTheTime();
  }

  ESP.wdtFeed();  // Reset watchdog timer
  delay(delayval);
  ESP.wdtFeed();  // Reset watchdog timer

  // Web update start:
  httpServer.handleClient();
  MDNS.update();

  // Ping the set IP-address... Turn off the LED by presence status of an IP-address (of your smart phone) monitored by a PING request 2 times perminute:
  if (PING_USEMONITOR == 1)
  {
    if (iSecond == 45 || iSecond == 15) {

      if (PING_DEBUG_MODE == 1) {
        Serial.print("IP gets pinged now: " + String(iHour) + ":" + String(iMinute) + ":" + String(iSecond) + " - IP: ");
        Serial.print(remote_ip);
        Serial.print(" --> ");
      }
      if (Ping.ping(remote_ip))
      {
        if (PING_DEBUG_MODE == 1) Serial.print("online - remaining attempts = ");
        PING_ATTEMPTS = PING_TIMEOUTNUM; // Reset to configured value
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTS);
        pixels.setBrightness(intensity);
        LEDsON = true;
      } else {
        if (PING_ATTEMPTS >= 1) PING_ATTEMPTS = PING_ATTEMPTS - 1;
        if (PING_DEBUG_MODE == 1) Serial.print("OFFLINE - remaining attempts = ");
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTS);
      }
      if (PING_ATTEMPTS == 0) {
        if (PING_DEBUG_MODE == 1) Serial.print(PING_ATTEMPTS);
        if (PING_DEBUG_MODE == 1) Serial.println(" remaining attempts --> LEDs = OFF until the IP-address can be reached again...");
        pixels.setBrightness(0);
        pixels.show();
        LEDsON = false;
      }
    }
  }
  if (LEDsON == true) pixels.show(); // This sends the updated pixel color to the hardware.
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void setLED(int ledNrFrom, int ledNrTo, int switchOn) {
  if (switchOn) {
    if  (ledNrFrom > ledNrTo) {
      setLED(ledNrTo, ledNrFrom, switchOn); //sets LED numbers in correct order (because of the date programming below)
    } else {
      for (int i = ledNrFrom; i <= ledNrTo; i++) {
        if ((i >= 0) &&
            (i < NUMPIXELS))
          pixels.setPixelColor(i, pixels.Color(redVal, greenVal, blueVal));
      }
    }
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void setLEDHour(int ledNrFrom, int ledNrTo, int switchOn) {
  // Every Hour blink orange
  if ((blinkTime) &&
      (switchOn) &&
      (iMinute == 0) &&
      (iSecond < 10)) {
    if ((iSecond % 2) == 1) {
      for (int i = ledNrFrom; i <= ledNrTo; i++) {
        if ((i >= 0) &&
            (i < NUMPIXELS))
          pixels.setPixelColor(i, pixels.Color(255, 128, 0));
      }
    } else
      setLED(ledNrFrom, ledNrTo, switchOn);
  } else
    setLED(ledNrFrom, ledNrTo, switchOn);
}


// ###########################################################################################################################################
// # Switch a horizontal sequence of LEDs ON or OFF, depending on boolean value switchOn:
// ###########################################################################################################################################
void setLEDLine(int xFrom, int xTo, int y, int switchOn) {
  if (xFrom > xTo)
    setLEDLine(xTo, xFrom, y, switchOn);
  else {
    for (int x = xFrom; x <= xTo; x++) {
      setLED(ledXY(x, y), ledXY(x, y), switchOn);
    }
  }
}


// ###########################################################################################################################################
// # Try to read settings from FLASH - initialize if WLAN ID read from flash is invalid:
// ###########################################################################################################################################
void readEEPROM() {
  Serial.print("Copy ");
  Serial.print(sizeof(parameter));
  Serial.println(" bytes from flash memory to EPROM buffer: ");

  // initialize space to read parameter
  EEPROM.begin(sizeof(parameter));
  delay(10);

  // Copy bytes from EEPROM
  byte* p = (byte*)(void*)&parameter;

  int check = 0;
  for (int L = 0; L < sizeof(parameter); ++L) {
    byte b = EEPROM.read(L);

    // Don't take the checkSum itself into account
    if (L < sizeof(parameter) - sizeof(parameter.pCheckSum))
      check = check + b;

    *p++ = b;
    // Serial.print("Read FLASH Byte ");
    // Serial.print(L);
    // Serial.print(" = ");
    // Serial.println(b);
  }

  // Check checksum
  Serial.print("Compare checksums: ");
  Serial.print(check);
  Serial.print("/");
  Serial.println(parameter.pCheckSum);

  if (check == parameter.pCheckSum) {
    Serial.println("Checksum match! set parameter from EEPROM");
    redVal    =  parameter.pRed;
    greenVal  =  parameter.pGreen;
    blueVal   =  parameter.pBlue;
    showDate = parameter.pShowDate;
    displayoff = parameter.pdisplayoff;
    useNightLEDs = parameter.puseNightLEDs;
    displayonmaxMO = parameter.pdisplayonmaxMO;
    displayonminMO = parameter.pdisplayonminMO;
    displayonmaxTU = parameter.pdisplayonmaxTU;
    displayonminTU = parameter.pdisplayonminTU;
    displayonmaxWE = parameter.pdisplayonmaxWE;
    displayonminWE = parameter.pdisplayonminWE;
    displayonmaxTH = parameter.pdisplayonmaxTH;
    displayonminTH = parameter.pdisplayonminTH;
    displayonmaxFR = parameter.pdisplayonmaxFR;
    displayonminFR = parameter.pdisplayonminFR;
    displayonmaxSA = parameter.pdisplayonmaxSA;
    displayonminSA = parameter.pdisplayonminSA;
    displayonmaxSU = parameter.pdisplayonmaxSU;
    displayonminSU = parameter.pdisplayonminSU;
    wchostnamenum = parameter.pwchostnamenum;
    useupdate = parameter.puseupdate;
    useledtest = parameter.puseledtest;
    usesetwlan = parameter.pusesetwlan;
    useshowip = parameter.puseshowip;
    switchRainBow = parameter.pswitchRainBow;
    switchLEDOrder = parameter.pswitchLEDOrder;
    blinkTime =  parameter.pBlinkTime;
    dcwFlag =  parameter.pDCWFlag;
    intensity =  parameter.pIntensity;
    intensityNight =  parameter.pIntensityNight;

    PING_IP_ADDR_O1 = parameter.pPING_IP_ADDR_O1;
    PING_IP_ADDR_O2 = parameter.pPING_IP_ADDR_O2;
    PING_IP_ADDR_O3 = parameter.pPING_IP_ADDR_O3;
    PING_IP_ADDR_O4 = parameter.pPING_IP_ADDR_O4;
    PING_TIMEOUTNUM = parameter.pPING_TIMEOUTNUM;
    PING_DEBUG_MODE = parameter.pPING_DEBUG_MODE;
    PING_USEMONITOR = parameter.pPING_USEMONITOR;

    String ntp(parameter.pNTPServer);
    ntpServer = ntp;
    String tz(parameter.pTimeZone);
    timeZone = tz;
  }
}


// ###########################################################################################################################################
// # Write current parameter settings to flash:
// ###########################################################################################################################################
void writeEEPROM() {
  Serial.println("Write parameter into EEPRom");
  parameter.pRed       = redVal;
  parameter.pGreen     = greenVal;
  parameter.pBlue      = blueVal;
  parameter.pIntensity = intensity;
  parameter.pIntensityNight = intensityNight;
  parameter.pDCWFlag   = dcwFlag;
  parameter.pBlinkTime = blinkTime;
  ntpServer.toCharArray(parameter.pNTPServer, sizeof(parameter.pNTPServer));
  timeZone.toCharArray(parameter.pTimeZone, sizeof(parameter.pTimeZone));
  parameter.pShowDate  = showDate;
  parameter.pdisplayoff  = displayoff;
  parameter.puseNightLEDs  = useNightLEDs;
  parameter.pdisplayonmaxMO = displayonmaxMO;
  parameter.pdisplayonminMO = displayonminMO;
  parameter.pdisplayonmaxTU = displayonmaxTU;
  parameter.pdisplayonminTU = displayonminTU;
  parameter.pdisplayonmaxWE = displayonmaxWE;
  parameter.pdisplayonminWE = displayonminWE;
  parameter.pdisplayonmaxTH = displayonmaxTH;
  parameter.pdisplayonminTH = displayonminTH;
  parameter.pdisplayonmaxFR = displayonmaxFR;
  parameter.pdisplayonminFR = displayonminFR;
  parameter.pdisplayonmaxSA = displayonmaxSA;
  parameter.pdisplayonminSA = displayonminSA;
  parameter.pdisplayonmaxSU = displayonmaxSU;
  parameter.pdisplayonminSU = displayonminSU;
  parameter.pwchostnamenum = wchostnamenum;
  parameter.puseupdate  = useupdate;
  parameter.puseledtest  = useledtest;
  parameter.pusesetwlan  = usesetwlan;
  parameter.puseshowip   = useshowip;
  parameter.pswitchRainBow = switchRainBow;
  parameter.pswitchLEDOrder = switchLEDOrder;

  parameter.pPING_IP_ADDR_O1 = PING_IP_ADDR_O1;
  parameter.pPING_IP_ADDR_O2 = PING_IP_ADDR_O2;
  parameter.pPING_IP_ADDR_O3 = PING_IP_ADDR_O3;
  parameter.pPING_IP_ADDR_O4 = PING_IP_ADDR_O4;
  parameter.pPING_TIMEOUTNUM = PING_TIMEOUTNUM;
  parameter.pPING_DEBUG_MODE = PING_DEBUG_MODE;
  parameter.pPING_USEMONITOR = PING_USEMONITOR;

  // calculate checksum
  byte* p = (byte*)(void*)&parameter;
  parameter.pCheckSum = 0;
  for (int L = 0; L < sizeof(parameter) - sizeof(parameter.pCheckSum); ++L) {
    byte b = *p++;
    parameter.pCheckSum = parameter.pCheckSum + b;
  }
  // Write data to EEPROM
  p = (byte*)(void*)&parameter;
  for (int L = 0; L < sizeof(parameter); ++L) {
    byte b = *p++;
    EEPROM.write(L, b);
    // Serial.print("Write FLASH Byte ");
    // Serial.print(L);
    // Serial.print(" = ");
    // Serial.println(b);
  }
  EEPROM.commit();
}


// ###########################################################################################################################################
// # Check for HTML respond from Client logged on to Web page:
// ###########################################################################################################################################
void checkClient() {
  //Serial.println("check for client");
  WiFiClient client = server.available();   // Listen for incoming clients // @suppress("Abstract class cannot be instantiated")

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>" + wchostname + wchostnamenum + " - " + WORD_CLOCK_VERSION + "</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            String title = "<body><h1>WordClock '" + wchostname + wchostnamenum + "' Einstellungen ";
            title = title + WORD_CLOCK_VERSION;
            title = title + "</h1>";
            client.println(title);
            client.println("<form action=\"/setWC.php\">");

            // Convert color into hex settings
            char hex[7] = {0};
            sprintf(hex, "#%02X%02X%02X", redVal, greenVal, blueVal);

            client.println("<hr><h2>LED Einstellungen</h2><br>");
            client.println("<label for=\"favcolor\">Farbe: </label>");
            client.print("<input type=\"color\" id=\"favcolor\" name=\"favcolor\" value=\"");
            client.print(hex);
            client.print("\"><br><br>");

            client.print("<label for=\"intensity\">Helligkeit am Tag: </label>");
            client.print("<input type=\"range\" id=\"intensity\" name=\"intensity\" min=\"1\" max=\"255\" value=\"");
            client.print(intensity);
            client.print("\">");
            client.println("<br><br>");

            client.print("<label for=\"intensityNight\">Helligkeit bei Nacht: </label>");
            client.print("<input type=\"range\" id=\"intensityNight\" name=\"intensityNight\" min=\"1\" max=\"255\" value=\"");
            client.print(intensityNight);
            client.print("\">");
            client.println("<br><hr>");

            client.println("<h2>Volle Stunde blinken</h2><br>");
            client.println("<label for=\"showdate\">Stundenangabe soll zur vollen Stunde blinken?</label>");
            client.print("<input type=\"checkbox\" id=\"blinktime\" name=\"blinktime\"");
            if (blinkTime)
              client.print(" checked");
            client.print("><br><hr>");

            client.println("<h2>Datumsanzeige</h2><br>");
            client.println("<label for=\"showdate\">Alle 30 Sekunden anzeigen?</label>");
            client.print("<input type=\"checkbox\" id=\"showdate\" name=\"showdate\"");
            if (showDate)
              client.print(" checked");
            if (!checkRTC()) {
              client.println("<br><br><h2 style=\"text-decoration:blink;color:red\">No RTC found!</h2><br><br>");
            }
            client.print("><br><hr>");

            client.println("<h2>Display abschalten oder dunkler schalten?</h2><br>");
            client.println("<label for=\"displayoff\">Display komplett abschalten ...</label>");
            client.print("<input type=\"checkbox\" id=\"displayoff\" name=\"displayoff\"");
            if (displayoff) {
              client.print(" checked");
            }
            client.print("><br><br>");


            client.println("<label for=\"useNightLEDs\">... oder nur dunkler schalten auf Wert der Helligkeit bei Nacht?</label>");
            client.print("<input type=\"checkbox\" id=\"useNightLEDs\" name=\"useNightLEDs\"");
            if (useNightLEDs) {
              client.print(" checked");
            }
            client.print("><br><br>");

            client.println("<label for=\"displayonmaxMO\">Montag - Display aus ab: </label>");
            client.println("<select id=\"displayonmaxMO\" name=\"displayonmaxMO\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonmaxMO);
            client.println("</option>");
            client.println("<option>15</option>");
            client.println("<option>16</option>");
            client.println("<option>17</option>");
            client.println("<option>18</option>");
            client.println("<option>19</option>");
            client.println("<option>20</option>");
            client.println("<option>21</option>");
            client.println("<option>22</option>");
            client.println("<option>23</option>");
            client.println("</select>:00 bis ");

            client.println("<label for=\"displayonminMO\"></label>");
            client.println("<select id=\"displayonminMO\" name=\"displayonminMO\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonminMO);
            client.println("</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select>:59 Uhr <br>");
            client.print("<br>");

            client.println("<label for=\"displayonmaxTU\">Dienstag - Display aus ab: </label>");
            client.println("<select id=\"displayonmaxTU\" name=\"displayonmaxTU\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonmaxTU);
            client.println("</option>");
            client.println("<option>15</option>");
            client.println("<option>16</option>");
            client.println("<option>17</option>");
            client.println("<option>18</option>");
            client.println("<option>19</option>");
            client.println("<option>20</option>");
            client.println("<option>21</option>");
            client.println("<option>22</option>");
            client.println("<option>23</option>");
            client.println("</select>:00 bis ");

            client.println("<label for=\"displayonminTU\"></label>");
            client.println("<select id=\"displayonminTU\" name=\"displayonminTU\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonminTU);
            client.println("</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select>:59 Uhr <br>");
            client.print("<br>");

            client.println("<label for=\"displayonmaxWE\">Mittwoch - Display aus ab: </label>");
            client.println("<select id=\"displayonmaxWE\" name=\"displayonmaxWE\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonmaxWE);
            client.println("</option>");
            client.println("<option>15</option>");
            client.println("<option>16</option>");
            client.println("<option>17</option>");
            client.println("<option>18</option>");
            client.println("<option>19</option>");
            client.println("<option>20</option>");
            client.println("<option>21</option>");
            client.println("<option>22</option>");
            client.println("<option>23</option>");
            client.println("</select>:00 bis ");

            client.println("<label for=\"displayonminWE\"></label>");
            client.println("<select id=\"displayonminWE\" name=\"displayonminWE\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonminWE);
            client.println("</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select>:59 Uhr <br>");
            client.print("<br>");

            client.println("<label for=\"displayonmaxTH\">Donnerstag - Display aus ab: </label>");
            client.println("<select id=\"displayonmaxTH\" name=\"displayonmaxTH\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonmaxTH);
            client.println("</option>");
            client.println("<option>15</option>");
            client.println("<option>16</option>");
            client.println("<option>17</option>");
            client.println("<option>18</option>");
            client.println("<option>19</option>");
            client.println("<option>20</option>");
            client.println("<option>21</option>");
            client.println("<option>22</option>");
            client.println("<option>23</option>");
            client.println("</select>:00 bis ");

            client.println("<label for=\"displayonminTH\"></label>");
            client.println("<select id=\"displayonminTH\" name=\"displayonminTH\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonminTH);
            client.println("</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select>:59 Uhr <br>");
            client.print("<br>");

            client.println("<label for=\"displayonmaxFR\">Freitag - Display aus ab: </label>");
            client.println("<select id=\"displayonmaxFR\" name=\"displayonmaxFR\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonmaxFR);
            client.println("</option>");
            client.println("<option>15</option>");
            client.println("<option>16</option>");
            client.println("<option>17</option>");
            client.println("<option>18</option>");
            client.println("<option>19</option>");
            client.println("<option>20</option>");
            client.println("<option>21</option>");
            client.println("<option>22</option>");
            client.println("<option>23</option>");
            client.println("</select>:00 bis ");

            client.println("<label for=\"displayonminFR\"></label>");
            client.println("<select id=\"displayonminFR\" name=\"displayonminFR\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonminFR);
            client.println("</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select>:59 Uhr <br>");
            client.print("<br>");

            client.println("<label for=\"displayonmaxSA\">Samstag - Display aus ab: </label>");
            client.println("<select id=\"displayonmaxSA\" name=\"displayonmaxSA\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonmaxSA);
            client.println("</option>");
            client.println("<option>15</option>");
            client.println("<option>16</option>");
            client.println("<option>17</option>");
            client.println("<option>18</option>");
            client.println("<option>19</option>");
            client.println("<option>20</option>");
            client.println("<option>21</option>");
            client.println("<option>22</option>");
            client.println("<option>23</option>");
            client.println("</select>:00 bis ");

            client.println("<label for=\"displayonminSA\"></label>");
            client.println("<select id=\"displayonminSA\" name=\"displayonminSA\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonminSA);
            client.println("</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select>:59 Uhr <br>");
            client.print("<br>");

            client.println("<label for=\"displayonmaxSU\">Sonntag - Display aus ab: </label>");
            client.println("<select id=\"displayonmaxSU\" name=\"displayonmaxSU\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonmaxSU);
            client.println("</option>");
            client.println("<option>15</option>");
            client.println("<option>16</option>");
            client.println("<option>17</option>");
            client.println("<option>18</option>");
            client.println("<option>19</option>");
            client.println("<option>20</option>");
            client.println("<option>21</option>");
            client.println("<option>22</option>");
            client.println("<option>23</option>");
            client.println("</select>:00 bis ");

            client.println("<label for=\"displayonminSU\"></label>");
            client.println("<select id=\"displayonminSU\" name=\"displayonminSU\" >");
            client.println("<option selected=\"selected\">");
            client.println(displayonminSU);
            client.println("</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select>:59 Uhr <br>");
            client.print("<br><hr>");

            client.println("<h2>WordClock Update</h2>");
            client.println("<label for=\"useupdate\">Update Funktion verwenden?</label>");
            client.print("<input type=\"checkbox\" id=\"useupdate\" name=\"useupdate\"");
            if (useupdate) {
              client.print(" checked");
              client.print("><br><br>");
              client.println("<label>Ueber einen der folgenden Links kann die WordClock ueber den Browser ohne Arduino aktualisiert werden:</label><br>");
              client.println("<br>");
              client.print("<a href=");
              client.print(UpdatePath);
              client.print(" target='_blank'>");
              client.print(UpdatePath);
              client.println("</a><br><br>");
              client.print("<a href=");
              client.print(UpdatePathIP);
              client.print(" target='_blank'>");
              client.print(UpdatePathIP);
              client.println("</a><br><br>");
              client.println("<label>Hinweis: Es wird eine in Arduino mit Strg+Alt+S zuvor erstellte .BIN Datei des Sketches benoetigt,<br>die ueber die Option 'Update Firmware' hochgeladen werden kann.</label>");
            }
            else
            {
              client.println("><br><br><label>Die Update Option ist aktuell deaktiviert.</label>");
            }
            client.print("<br><hr>");

            client.println("<h2>LED Anzeigen und Startverhalten</h2>");
            client.println("<label for=\"useledtest\">LED Start Test anzeigen?</label>");
            client.print("<input type=\"checkbox\" id=\"useledtest\" name=\"useledtest\"");
            if (useledtest) {
              client.print(" checked");
              client.print("><br><br>");
            }
            else
            {
              client.print("><br><br>");
            }
            client.println("<label for=\"usesetwlan\">SET WLAN beim Start anzeigen?</label>");
            client.print("<input type=\"checkbox\" id=\"usesetwlan\" name=\"usesetwlan\"");
            if (usesetwlan) {
              client.print(" checked");
              client.print("><br><br>");
            }
            else
            {
              client.print("><br><br>");
            }

            client.println("<label for=\"useshowip\">IP-Addresse beim Start anzeigen?</label>");
            client.print("<input type=\"checkbox\" id=\"useshowip\" name=\"useshowip\"");
            if (useshowip) {
              client.print(" checked");
              client.print("><br><br>");
            }
            else
            {
              client.print("><br><br>");
            }

            client.println("<br><label for=\"switchRainBow\">Regenbogen Farbeffekt anzeigen?</label>");
            client.print("<input type=\"checkbox\" id=\"switchRainBow\" name=\"switchRainBow\"");
            if (switchRainBow) {
              client.print(" checked");
              client.print(">");
            }
            else
            {
              client.print(">");
            }
            client.println("<br><br>Wenn diese Option gesetzt wird, wechselt die Farbe der Uhr im Zufallsmodus zu jeder neuen Minute.<br>");
            client.println("Die oben eingestellte Hauptfarbe wird dann ignoriert.<br><br>");

            client.println("<br><label for=\"switchLEDOrder\">Minuten LEDs Ecken Reihenfolge im Uhrzeigersinn?</label>");
            client.print("<input type=\"checkbox\" id=\"switchLEDOrder\" name=\"switchLEDOrder\"");
            if (switchLEDOrder) {
              client.print(" checked");
              client.print(">");
            }
            else
            {
              client.print(">");
            }
            client.println("<br><br>Wenn diese Option gesetzt wird, werden die Minuten-LEDs in den 4 Ecken<br>");
            client.println("im Uhrzeigersinn angezeigt, ansonsten entgegen dem Uhrzeigersinn.<br>");
            client.print("<hr>");


            client.println("<h2>WLAN Einstellungen zuruecksetzen</h2> <br>");
            client.println("<label for = \"wifireset\">WLAN Einstellungen zuruecksetzen und Uhr neu starten?</label>");
            client.print("<input type=\"checkbox\" id=\"wifireset\" name=\"wifireset\"");
            if (wifireset) {
              client.print(" checked");
            }
            client.print("><br>");
            client.println("<br>! Wenn diese Option gesetzt wird, werden die WLAN Einstellungen einmalig geloescht !<br>");
            client.print("<br><hr>");


            // PING IP ADDRESS:
            client.println("<h2>PING Monitor IP-Adresse -> LEDs abschalten wenn IP laenger offline</h2>");
            client.println("<label for=\"PING_USEMONITOR\">PING Monitor Funktion verwenden?</label>");
            client.print("<input type=\"checkbox\" id=\"PING_USEMONITOR\" name=\"PING_USEMONITOR\"");
            if (PING_USEMONITOR) {
              client.print(" checked");
              client.print("><br><br>");
            }
            else
            {
              client.print("><br><br>");
            }
            client.println("<label>Bitte hier die zu ueberwachende IP-Adresse eintragen:</label><br>");
            client.println("<label for=\"PING_IP_ADDR_O1\">IP-Adresse:</label>");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR_O1\" name=\"PING_IP_ADDR_O1\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR_O1);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR_O2\" name=\"PING_IP_ADDR_O2\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR_O2);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR_O3\" name=\"PING_IP_ADDR_O3\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR_O3);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR_O4\" name=\"PING_IP_ADDR_O4\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR_O4);
            client.println("\"><br><br>");

            client.println("<br><label for=\"PING_TIMEOUTNUM\">Anzahl PING Versuche bis die LEDs abgeschaltet werden:</label>");
            client.print("<input type=\"text\" id=\"PING_TIMEOUTNUM\" name=\"PING_TIMEOUTNUM\" size=\"3\" value=\"");
            client.print(PING_TIMEOUTNUM);
            client.println("\">");
            client.println("<br><label>Hinweis: Anzahl = 10 bedeutet einen 5 Minuten Timeout, da 2 PING Versuche pro Minute erfolgen...</label><br><br>");

            client.println("<label for=\"PING_DEBUG_MODE\">DEBUG PING Monitor Funktion verwenden?</label>");
            client.print("<input type=\"checkbox\" id=\"PING_DEBUG_MODE\" name=\"PING_DEBUG_MODE\"");
            if (PING_DEBUG_MODE) {
              client.print(" checked");
              client.print("><br>");
            }
            else
            {
              client.print("><br>");
            }
            client.print("<br><hr>");


            client.println("<h2>WordClock Hostname anpassen</h2><br>");
            client.println("<label for=\"wchostnamenum\">Hostname: " + wchostname + "</label>");
            client.println("<select id=\"wchostnamenum\" name=\"wchostnamenum\" >");
            client.println("<option selected=\"selected\">");
            client.println(wchostnamenum);
            client.println("</option>");
            client.println("<option>0</option>");
            client.println("<option>1</option>");
            client.println("<option>2</option>");
            client.println("<option>3</option>");
            client.println("<option>4</option>");
            client.println("<option>5</option>");
            client.println("<option>6</option>");
            client.println("<option>7</option>");
            client.println("<option>8</option>");
            client.println("<option>9</option>");
            client.println("</select><br>");
            client.print("<br><hr>");

            client.println("<h2>WordClock neustarten</h2> <br>");
            client.println("<label for = \"clockreset\">WordClock neu starten?</label>");
            client.print("<input type=\"checkbox\" id=\"clockreset\" name=\"clockreset\"");
            if (clockreset) {
              client.print(" checked");
            }
            client.print("><br>");
            client.println("<br>! Wenn diese Option gesetzt wird, wird die Uhr einmalig neu gestartet !<br>");
            client.print("<br><hr>");

            client.println("<h2>Zeitzone &amp; NTP-Server</h2><br>");
            client.println("<label for=\"ntpserver\"></label>");
            client.print("<input type=\"text\" id=\"ntpserver\" name=\"ntpserver\" size=\"45\" value=\"");
            client.print(ntpServer);
            client.println("\"><br>");
            client.println("<br><label for=\"timezone\"></label>");
            client.print("<input type=\"text\" id=\"timezone\" name=\"timezone\" size=\"45\" value=\"");
            client.print(timeZone);
            client.println("\"><br><br>");
            client.print("<br>Standardwerte:<br><br>");
            client.print(DEFAULT_NTP_SERVER);
            client.println("<br><br>");
            client.print(DEFAULT_TIMEZONE);
            client.println("<br><br>");
            client.print("<br><a href=\"");
            client.println(TZ_WEB_SITE);
            client.println("\" target=\"_blank\">Erkl&auml;rung zur Einstellung der Zeitzone</a><br>");
            client.print("<br><hr><br>");

            client.println("<br><br><input type=\"submit\" value=\"Einstellungen speichern\">");
            client.print("<br><br><br><hr><br>");
            client.println("</form>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            if (currentLine.startsWith("GET /setWC.php?")) {
              // Serial.print("Current request:  ");
              // Serial.println(currentLine);

              // Check for color settings
              int pos = currentLine.indexOf("favcolor=%23");
              if (pos >= 0) {
                char * succ;
                String newColStr = currentLine.substring(pos + 12, pos + 18);
                // Serial.print("Change color to ");
                // Serial.print(newColStr);
                String newRed   = newColStr.substring(0, 2);
                redVal = strtol(newRed.begin(), &succ, 16);
                // Serial.print(", Red=");
                // Serial.print(newRed);
                // Serial.print("/");
                // Serial.print(redVal);
                String newGreen = newColStr.substring(2, 4);
                greenVal = strtol(newGreen.begin(), &succ, 16);
                // Serial.print(", Green=");
                // Serial.print(newGreen);
                // Serial.print("/");
                // Serial.print(greenVal);
                String newBlue  = newColStr.substring(4, 6);
                blueVal = strtol(newBlue.begin(), &succ, 16);
                // Serial.print(", Blue=");
                // Serial.print(newBlue);
                // Serial.print("/");
                // Serial.println(blueVal);
              }

              // Check for blink time
              // Serial.print("BlinkTime switched  ");
              if (currentLine.indexOf("&blinktime=on&") >= 0) {
                blinkTime = -1;
                // Serial.println("on");
              } else {
                blinkTime = 0;
                // Serial.println("off");
              }

              // Check for date display
              // Serial.print("ShowDate switched  ");
              if (currentLine.indexOf("&showdate=on&") >= 0) {
                showDate = -1;
                // Serial.println("on");
              } else {
                showDate = 0;
                // Serial.println("off");
              }

              // Check for DisplayOff switch
              // Serial.print("DisplayOff switched  ");
              if (currentLine.indexOf("&displayoff=on&") >= 0) {
                displayoff = -1;
                // Serial.println("on");
              } else {
                displayoff = 0;
                // Serial.println("off");
              }

              // Check for useNightLEDs switch
              // Serial.print("useNightLEDs switched  ");
              if (currentLine.indexOf("&useNightLEDs=on&") >= 0) {
                useNightLEDs = -1;
                // Serial.println("on");
              } else {
                useNightLEDs = 0;
                // Serial.println("off");
              }

              // Pick up Display On Max
              pos = currentLine.indexOf("&displayonmaxMO=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                // Serial.print("MO - Display On Max set to ");
                // Serial.println(maxStr);
                displayonmaxMO = maxStr.toInt();
              }
              // Pick up Display On Min
              pos = currentLine.indexOf("&displayonminMO=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                // Serial.print("MO - Display On Min set to ");
                // Serial.println(minStr);
                displayonminMO = minStr.toInt();
              }

              // Pick up Display On Max
              pos = currentLine.indexOf("&displayonmaxTU=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                // Serial.print("TU - Display On Max set to ");
                // Serial.println(maxStr);
                displayonmaxTU = maxStr.toInt();
              }
              // Pick up Display On Min
              pos = currentLine.indexOf("&displayonminTU=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                // Serial.print("TU - Display On Min set to ");
                // Serial.println(minStr);
                displayonminTU = minStr.toInt();
              }

              // Pick up Display On Max
              pos = currentLine.indexOf("&displayonmaxWE=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                // Serial.print("WE - Display On Max set to ");
                // Serial.println(maxStr);
                displayonmaxWE = maxStr.toInt();
              }
              // Pick up Display On Min
              pos = currentLine.indexOf("&displayonminWE=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                // Serial.print("WE - Display On Min set to ");
                // Serial.println(minStr);
                displayonminWE = minStr.toInt();
              }

              // Pick up Display On Max
              pos = currentLine.indexOf("&displayonmaxTH=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                // Serial.print("TH - Display On Max set to ");
                // Serial.println(maxStr);
                displayonmaxTH = maxStr.toInt();
              }
              // Pick up Display On Min
              pos = currentLine.indexOf("&displayonminTH=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                // Serial.print("TH - Display On Min set to ");
                // Serial.println(minStr);
                displayonminTH = minStr.toInt();
              }

              // Pick up Display On Max
              pos = currentLine.indexOf("&displayonmaxFR=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                // Serial.print("FR - Display On Max set to ");
                // Serial.println(maxStr);
                displayonmaxFR = maxStr.toInt();
              }
              // Pick up Display On Min
              pos = currentLine.indexOf("&displayonminFR=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                // Serial.print("FR - Display On Min set to ");
                // Serial.println(minStr);
                displayonminFR = minStr.toInt();
              }

              // Pick up Display On Max
              pos = currentLine.indexOf("&displayonmaxSA=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                // Serial.print("SA - Display On Max set to ");
                // Serial.println(maxStr);
                displayonmaxSA = maxStr.toInt();
              }
              // Pick up Display On Min
              pos = currentLine.indexOf("&displayonminSA=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                // Serial.print("SA - Display On Min set to ");
                // Serial.println(minStr);
                displayonminSA = minStr.toInt();
              }

              // Pick up Display On Max
              pos = currentLine.indexOf("&displayonmaxSU=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                // Serial.print("SU - Display On Max set to ");
                // Serial.println(maxStr);
                displayonmaxSU = maxStr.toInt();
              }
              // Pick up Display On Min
              pos = currentLine.indexOf("&displayonminSU=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                // Serial.print("SU - Display On Min set to ");
                // Serial.println(minStr);
                displayonminSU = minStr.toInt();
              }

              // Pick up WordClock HostName
              pos = currentLine.indexOf("&wchostnamenum=");
              if (pos >= 0) {
                String hostStr = currentLine.substring(pos + 15);
                pos = hostStr.indexOf("&");
                if (pos > 0)
                  hostStr = hostStr.substring(0, pos);
                // Serial.print("WordClock HostName set to " + wchostname);
                // Serial.println(hostStr);
                wchostnamenum = hostStr.toInt();
              }

              // Check for UseUpdate switch
              // Serial.print("UseUpdate switched  ");
              if (currentLine.indexOf("&useupdate=on&") >= 0) {
                useupdate = -1;
                // Serial.println("on");
                MDNS.begin(wchostname + wchostnamenum);
                httpUpdater.setup(&httpServer);
                httpServer.begin();
                MDNS.addService("http", "tcp", 2022);
                MDNS.addService("http", "tcp", 80);
                UpdatePath = "http://" + String(wchostname + wchostnamenum) + ".local:2022/update";
                Serial.print("Web Update Link: ");
                Serial.println(UpdatePath);
                UpdatePathIP = "http://" + WiFi.localIP().toString() + ":2022/update";
                Serial.print("Web Update Link via IP-address: ");
                Serial.println(UpdatePathIP);
              } else {
                useupdate = 0;
                // Serial.println("off");
                httpUpdater.setup(&httpServer);
                httpServer.stop();
              }

              // Check for Use LED test switch
              // Serial.print("Use LED test switched  ");
              if (currentLine.indexOf("&useledtest=on&") >= 0) {
                useledtest = -1;
                // Serial.println("on");
              } else {
                useledtest = 0;
                // Serial.println("off");
              }

              // Check for Use SET WLAN switch
              // Serial.print("Use SET WLAN switched  ");
              if (currentLine.indexOf("&usesetwlan=on&") >= 0) {
                usesetwlan = -1;
                // Serial.println("on");
              } else {
                usesetwlan = 0;
                // Serial.println("off");
              }

              // Check for Use SHOW IP switch
              // Serial.print("Use SHOW IP switched  ");
              if (currentLine.indexOf("&useshowip=on&") >= 0) {
                useshowip = -1;
                // Serial.println("on");
              } else {
                useshowip = 0;
                // Serial.println("off");
              }

              // Check for RainBox switch
              // Serial.print("Use RainBow mode switched  ");
              if (currentLine.indexOf("&switchRainBow=on&") >= 0) {
                switchRainBow = -1;
                // Serial.println("on");
              } else {
                switchRainBow = 0;
                // Serial.println("off");
              }

              // Check for Minute LEDs order switch
              // Serial.print("Use minutes LED mode switch  ");
              if (currentLine.indexOf("&switchLEDOrder=on&") >= 0) {
                switchLEDOrder = -1;
                // Serial.println("on");
              } else {
                switchLEDOrder = 0;
                // Serial.println("off");
              }

              // Check for WiFi settings RESET switch
              // Serial.print("WIFI settings RESET switched  ");
              if (currentLine.indexOf("&wifireset=on&") >= 0) {
                // Serial.println("on");
                wifireset = -1;
                dunkel();
                // Show "SET WLAN" --> no blank display
                Serial.println("Show SET WLAN after WiFi reset...");
                setLED(6, 6, 1);      // S
                setLED(12, 12, 1);    // E
                setLED(24, 24, 1);    // T
                setLED(63, 63, 1);    // W
                setLED(83, 83, 1);    // L
                setLED(84, 84, 1);    // A
                setLED(93, 93, 1);    // N
                pixels.show();
                WiFi.disconnect(true);
                delay(1500);
                WiFiManager wifiManager;
                delay(1500);
                wifiManager.resetSettings();
                delay(1500);
                Serial.println("######################################################################################################");
                Serial.println("# WIFI SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE RECONFIGURE WIFI AGAIN... #");
                Serial.println("######################################################################################################");
                delay(3000);
                ESP.restart();
              } else {
                wifireset = 0;
                // Serial.println("off");
              }

              // Check for WordClock RESET switch
              // Serial.print("WordClock RESTART switched  ");
              if (currentLine.indexOf("&clockreset=on&") >= 0) {
                // Serial.println("on");
                clockreset = -1;
                dunkel();
                // Show "RESET" --> no blank display
                Serial.println("Show RESET before board reset...");
                setLED(30, 31, 1);    // RE
                setLED(58, 58, 1);    // S
                setLED(64, 64, 1);    // E
                setLED(87, 87, 1);    // T
                pixels.show();
                Serial.println("##########################################");
                Serial.println("# WORDCLOCK WILL RESTART IN 3 SECONDS... #");
                Serial.println("##########################################");
                delay(3000);
                ESP.restart();
              } else {
                clockreset = 0;
                // Serial.println("off");
              }

              // Check for DCW flag
              if (currentLine.indexOf("DCW=ON") >= 0) {
                dcwFlag = -1;
                // Serial.println("DCW Flag switched ON");
              } else if (currentLine.indexOf("DCW=OFF") >= 0) {
                dcwFlag = 0;
                // Serial.println("DCW Flag switched OFF");
              }

              // get intensity DAY
              pos = currentLine.indexOf("&intensity=");
              if (pos >= 0) {
                String intStr = currentLine.substring(pos + 11, pos + 14);
                pos = intStr.indexOf("&");
                if (pos > 0)
                  intStr = intStr.substring(0, pos);
                // Serial.print("Intensity at day time set to ");
                // Serial.print(intStr);
                intensity = intStr.toInt();
              }

              // get intensity NIGHT
              pos = currentLine.indexOf("&intensityNight=");
              if (pos >= 0) {
                String intStr = currentLine.substring(pos + 16, pos + 19);
                pos = intStr.indexOf("&");
                if (pos > 0)
                  intStr = intStr.substring(0, pos);
                // Serial.print("Intensity at night time set to ");
                // Serial.print(intStr);
                intensityNight = intStr.toInt();
              }

              // Use PING function:
              if (currentLine.indexOf("&PING_USEMONITOR=on&") >= 0) {
                PING_USEMONITOR = 1;
              } else {
                PING_USEMONITOR = 0;
              }

              // Use PING DEBUG function:
              if (currentLine.indexOf("&PING_DEBUG_MODE=on&") >= 0) {
                PING_DEBUG_MODE = 1;
              } else {
                PING_DEBUG_MODE = 0;
              }

              // IP-address octet 1:
              pos = currentLine.indexOf("&PING_IP_ADDR_O1=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 17);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR_O1 = maxStr.toInt();
              }

              // IP-address octet 2:
              pos = currentLine.indexOf("&PING_IP_ADDR_O2=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 17);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR_O2 = maxStr.toInt();
              }

              // IP-address octet 3:
              pos = currentLine.indexOf("&PING_IP_ADDR_O3=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 17);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR_O3 = maxStr.toInt();
              }

              // IP-address octet 4:
              pos = currentLine.indexOf("&PING_IP_ADDR_O4=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 17);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR_O4 = maxStr.toInt();
              }

              // Max PING attempts:
              pos = currentLine.indexOf("&PING_TIMEOUTNUM=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 17);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_TIMEOUTNUM = maxStr.toInt();
              }


              // get NTP Server
              pos = currentLine.indexOf("&ntpserver=");
              if (pos >= 0) {
                String ntpStr = currentLine.substring(pos + 11);
                pos = ntpStr.indexOf("&");                            // "&" !!!
                if (pos > 0)
                  ntpStr = ntpStr.substring(0, pos);
                // Serial.print("NTP Server set to ");
                // Serial.println(ntpStr);
                ntpServer = ntpStr;
              }

              // Pick up TimeZone definition
              pos = currentLine.indexOf("timezone=");
              if (pos >= 0) {
                String tz = currentLine.substring(pos + 9);
                // check for unwanted suffix
                pos = tz.indexOf(" ");                                // " " !!!
                if (pos > 0) {
                  tz = tz.substring(0, pos);
                }
                timeZone = urldecode(tz);
                // Serial.print("Timezone set to ");
                // Serial.println(timeZone);
              }

              writeEEPROM(); // save DATA to EEPROM
              configNTPTime(); // Reset NTP
            }
            currentLine = ""; // Clear the current command line
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    header = ""; // Clear the header variable
    client.stop(); // Close the connection
    Serial.println("Web client disconnected.");
    Serial.println("######################################################################################################");
  }
}


// ###########################################################################################################################################
// # Read current date & time from RTC:
// ###########################################################################################################################################
void rtcReadTime() {
  if (checkRTC()) {

    //Serial.println("rtcReadTime() - calling rtc.now()");
    DateTime now = rtc.now();

    int oldHour = iHour;
    iYear  = (int)(now.year());
    iMonth = (int)(now.month());
    iDay   = (int)(now.day());
    iHour   = (int)(now.hour());
    iMinute = (int)(now.minute());
    iSecond = (int)(now.second());
    iWeekDay = (int)(now.dayOfTheWeek());

    // Print out time if minute has changed
    if (iHour != oldHour) {
      Serial.print("rtcReadTime() - get time from RTC : (");
      Serial.print(oldHour);
      Serial.print("/");
      Serial.print(iHour);
      Serial.println(") ");
      Serial.print(iYear);
      Serial.print("-");
      Serial.print(iMonth);
      Serial.print("-");
      Serial.print(iDay);
      Serial.print(" ");
      Serial.print(iHour);
      Serial.print(":");
      Serial.print(iMinute);
      Serial.print(":");
      Serial.println(iSecond);
    }
  }
}


// ###########################################################################################################################################
// # Show the IP-address on the display:
// ###########################################################################################################################################
void showIP () {
  IPAddress ip = WiFi.localIP();
  Serial.print("Displaying IP address ");
  Serial.println(ip);

  for (int x = 11; x > -75; x--) {
    dunkel();
    int offSet = x;
    // Loop over 4 bytes of IP Address
    for (int idx = 0; idx < 4; idx++) {
      uint8_t octet = ip[idx];
      printAt (octet / 100, offSet, 2);
      octet = octet % 100;
      offSet = offSet + 6;
      printAt (octet / 10, offSet, 2);
      offSet = offSet + 6;
      printAt (octet % 10, offSet, 2);
      offSet = offSet + 6;
      // add delimiter between octets
      if (idx < 3) {
        setLED(ledXY(offSet, 2), ledXY(offSet, 2), -1);  //sets point
        offSet++;
      }
    }
    pixels.show();
    delay (150);    // set speed of timeshift
  }
}


// ###########################################################################################################################################
// #  Switch off all LEDs:
// ###########################################################################################################################################
void dunkel() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Switch off all LEDs
  }
}


// ###########################################################################################################################################
// # Switch on default Text "ES IST":
// ###########################################################################################################################################
void defaultText() {
  pixels.setPixelColor( 5, pixels.Color(redVal, greenVal, blueVal)); // set on default text
  pixels.setPixelColor( 6, pixels.Color(redVal, greenVal, blueVal)); // set on default text
  pixels.setPixelColor( 7, pixels.Color(redVal, greenVal, blueVal)); // set on default text
  pixels.setPixelColor( 9, pixels.Color(redVal, greenVal, blueVal)); // set on default text
  pixels.setPixelColor(10, pixels.Color(redVal, greenVal, blueVal)); // set on default text
}


// ###########################################################################################################################################
// # Convert x/y coordinates into LED number return -1 for invalid coordinate:
// ###########################################################################################################################################
int ledXY (int x, int y) {
  // Test for valid coordinates
  // If outside panel return -1
  if ((x < 0)  ||
      (x > 10) ||
      (y < 0)  ||
      (y > 9))
    return -1;
  int ledNr = (9 - y) * 11;
  if ((y % 2) == 0)
    ledNr = ledNr + x;
  else
    ledNr = ledNr + 10 - x;
  return ledNr;
}


// ###########################################################################################################################################
// # Sets, where the numbers from 1 to 9 are printed:
// ###########################################################################################################################################
void printAt (int ziffer, int x, int y) {
  switch (ziffer) {
    case 0:   //number 0
      setLEDLine(x + 1, x + 3, y + 6, -1); //-1 is true, so switchOn
      for (int yd = 1; yd < 6; yd++) {
        setLED(ledXY(x,   y + yd), ledXY(x,   y + yd), -1);
        setLED(ledXY(x + 4, y + yd), ledXY(x + 4, y + yd), -1);
      }
      setLEDLine(x + 1, x + 3, y, -1);
      break;

    case 1:   //number 1
      setLED(ledXY(x + 3, y + 5), ledXY(x + 3, y + 5), -1);
      setLED(ledXY(x + 2, y + 4), ledXY(x + 2, y + 4), -1);
      for (int yd = 0; yd <= 6; yd++) {
        setLED(ledXY(x + 4,   y + yd), ledXY(x + 4,   y + yd), -1);
      }
      break;

    case 2:   //number 2
      for (int d = 1; d <= 4; d++) {
        setLED(ledXY(x + d,   y + d), ledXY(x + d,   y + d), -1);
      }
      setLEDLine(x, x + 4, y, -1);
      setLED(ledXY(x, y + 5),   ledXY(x, y + 5), -1);
      setLED(ledXY(x + 4, y + 5), ledXY(x + 4, y + 5), -1);
      setLEDLine(x + 1, x + 3, y + 6, -1);
      break;

    case 3:   //number 3
      for (int yd = 1; yd <= 2; yd++) {
        setLED(ledXY(x + 4, y + yd + 3), ledXY(x + 4, y + yd + 3), -1);
        setLED(ledXY(x + 4, y + yd),   ledXY(x + 4, y + yd), -1);
      }
      for (int yd = 0; yd < 7; yd = yd + 3) {
        setLEDLine(x + 1, x + 3, y + yd, -1);
      }
      setLED(ledXY(x, y + 1), ledXY(x, y + 1), -1);
      setLED(ledXY(x, y + 5), ledXY(x, y + 5), -1);
      break;

    case 4:   //number 4
      for (int d = 0; d <= 3; d++) {
        setLED(ledXY(x + d,   y + d + 3), ledXY(x + d,   y + d + 3), -1);
      }
      for (int yd = 0; yd <= 3; yd++) {
        setLED(ledXY(x + 3,   y + yd), ledXY(x + 3,   y + yd), -1);
      }
      setLEDLine(x, x + 4, y + 2, -1);
      break;

    case 5:   //number 5
      setLEDLine(x, x + 4, y + 6, -1);
      setLED(ledXY(x  , y + 5), ledXY(x  , y + 5), -1);
      setLED(ledXY(x  , y + 4), ledXY(x  , y + 4), -1);
      setLEDLine(x, x + 3, y + 3, -1);
      setLED(ledXY(x + 4, y + 2), ledXY(x + 4, y + 2), -1);
      setLED(ledXY(x + 4, y + 1), ledXY(x + 4, y + 1), -1);
      setLEDLine(x, x + 3, y, -1);
      break;

    case 6:   //number 6
      for (int d = 0; d <= 3; d++) {
        setLED(ledXY(x + d,   y + d + 3), ledXY(x + d,   y + d + 3), -1);
      }
      for (int yd = 0; yd < 4; yd = yd + 3) {
        setLEDLine(x + 1, x + 3, y + yd, -1);
      }
      setLED(ledXY(x, y + 1), ledXY(x, y + 1), -1);
      setLED(ledXY(x, y + 2), ledXY(x, y + 2), -1);
      setLED(ledXY(x + 4, y + 1), ledXY(x + 4, y + 1), -1);
      setLED(ledXY(x + 4, y + 2), ledXY(x + 4, y + 2), -1);
      break;

    case 7:   //number 7
      for (int yd = 0; yd <= 6; yd++) {
        setLED(ledXY(x + 3,   y + yd), ledXY(x + 3,   y + yd), -1);
      }
      setLEDLine(x + 1, x + 4, y + 3, -1);
      setLEDLine(x, x + 3, y + 6, -1);
      break;

    case 8:   //number 8
      for (int yd = 1; yd <= 2; yd++) {
        setLED(ledXY(x + 4, y + yd + 3), ledXY(x + 4, y + yd + 3), -1);
        setLED(ledXY(x + 4, y + yd),   ledXY(x + 4, y + yd), -1);
        setLED(ledXY(x, y + yd + 3),   ledXY(x, y + yd + 3), -1);
        setLED(ledXY(x, y + yd),     ledXY(x, y + yd), -1);
      }
      for (int yd = 0; yd < 7; yd = yd + 3) {
        setLEDLine(x + 1, x + 3, y + yd, -1);
      }
      break;

    case 9:   //number 9
      for (int d = 0; d <= 3; d++) {
        setLED(ledXY(x + d + 1,   y + d),  ledXY(x + d + 1,   y + d), -1);
      }
      for (int yd = 4; yd <= 5; yd++) {
        setLED(ledXY(x, y + yd),     ledXY(x, y + yd), -1);
        setLED(ledXY(x + 4, y + yd),   ledXY(x + 4, y + yd), -1);
      }
      setLEDLine(x + 1, x + 3, y + 6, -1);
      setLEDLine(x + 1, x + 4, y + 3, -1);
      break;
  }
}


// ###########################################################################################################################################
// # Turns on the outer four LEDs (one per minute):
// ###########################################################################################################################################
void showMinutes(int minutes) {
  int minMod = (minutes % 5);
  for (int i = 1; i < 5; i++) {
    int ledNr = 0;
    if (switchLEDOrder) {               // clockwise
      switch (i) {
        case 1: ledNr = 110; break;
        case 2: ledNr = 111; break;
        case 3: ledNr = 112; break;
        case 4: ledNr = 113; break;
      }
    } else {                            // anti clockwise
      switch (i) {
        case 1: ledNr = 113; break;
        case 2: ledNr = 112; break;
        case 3: ledNr = 111; break;
        case 4: ledNr = 110; break;
      }
    }
    if (minMod < i)
      pixels.setPixelColor(ledNr, pixels.Color(0, 0, 0));
    else
      pixels.setPixelColor(ledNr, pixels.Color(redVal, greenVal, blueVal));
  }
}


// ###########################################################################################################################################
// # Show current date on clock with moving digits:
// ###########################################################################################################################################
void showCurrentDate () {
  for (int x = 11; x > -50; x--) {
    dunkel();
    printAt (iDay / 10,   x,    2);
    printAt (iDay % 10,   x + 6,  2);
    setLED(ledXY(x + 11, 2), ledXY(x + 11, 2), -1); //sets first point
    printAt (iMonth / 10, x + 13, 2);
    printAt (iMonth % 10, x + 19, 2);
    if (iYear < 1000)
      iYear = iYear + 2000;
    setLED(ledXY(x + 24, 2), ledXY(x + 24, 2), -1); //sets second point
    printAt (iYear / 1000, x + 26, 2);
    iYear = iYear % 1000;
    printAt (iYear / 100,  x + 32, 2);
    iYear = iYear % 100;
    printAt (iYear / 10,  x + 38, 2);
    printAt (iYear % 10,  x + 44, 2);
    pixels.show();
    delay (150);// set speed of timeshift
  }
}


// ###########################################################################################################################################
// # Display current time:
// ###########################################################################################################################################
void showCurrentTime() {
  dunkel();      // switch off all LEDs
  defaultText(); // Switch on ES IST
  // divide minute by 5 to get value for display control
  int minDiv = iMinute / 5;

  // Fuenf (Minuten)
  setLED(  0,   3, ((minDiv ==  1) ||
                    (minDiv ==  5) ||
                    (minDiv ==  7) ||
                    (minDiv == 11)));

  // Viertel
  setLED( 22,  28, ((minDiv == 3) ||
                    (minDiv == 9)));

  // Zehn (Minuten)
  setLED( 11,  14, ((minDiv ==  2) ||
                    (minDiv == 10)));

  // Zwanzig
  setLED( 15,  21, ((minDiv == 4) ||
                    (minDiv == 8)));

  // Nach
  setLED( 40,  43, ((minDiv == 1) ||
                    (minDiv == 2) ||
                    (minDiv == 3) ||
                    (minDiv == 4) ||
                    (minDiv == 7)));

  // Vor
  setLED( 33,  35, ((minDiv ==  5) ||
                    (minDiv ==  8) ||
                    (minDiv ==  9) ||
                    (minDiv == 10) ||
                    (minDiv == 11)));

  // Halb
  setLED(  51, 54, ((minDiv == 5) ||
                    (minDiv == 6) ||
                    (minDiv == 7)));

  // Eck-LEDs: 1 pro Minute
  showMinutes(iMinute);

  //set hour from 1 to 12 (at noon, or midnight)
  int xHour = (iHour % 12);
  if (xHour == 0)
    xHour = 12;

  // at minute 25 hour needs to be counted up:
  // fuenf vor halb 2 = 13:25
  if (iMinute >= 25) {
    if (xHour == 12)
      xHour = 1;
    else
      xHour++;
  }

  // Uhr
  setLED(107, 109, (iMinute < 5));

  // Ein
  setLEDHour( 55,  57, (xHour == 1));

  // einS (S in EINS) (just used if not point 1 o'clock)
  setLEDHour( 58,  58, ((xHour == 1) &&
                        (iMinute > 4)));

  // Zwei
  setLEDHour( 62,  65, (xHour == 2));

  // Drei
  setLEDHour( 73,  76, (xHour == 3));

  // Vier
  setLEDHour( 66,  69, (xHour == 4));

  // Fuenf
  setLEDHour( 44,  47, (xHour == 5));

  // Sechs
  setLEDHour( 77,  81, (xHour == 6));

  // Sieben
  setLEDHour( 93,  98, (xHour == 7));

  // Acht
  setLEDHour( 84,  87, (xHour == 8));

  // Neun
  setLEDHour(102, 105, (xHour == 9));

  // Zehn (Stunden)
  setLEDHour( 99, 102, (xHour == 10));

  // Elf
  setLEDHour( 47,  49, (xHour == 11));

  // Zwoelf
  setLEDHour( 88,  92, (xHour == 12));
}


// ###########################################################################################################################################
// # Converts decimal to binary signs:
// ###########################################################################################################################################
byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}


// ###########################################################################################################################################
// # Function to write / set the clock:
// ###########################################################################################################################################
void rtcWriteTime(int jahr, int monat, int tag, int stunde, int minute, int sekunde) {
  if (checkRTC()) {
    Serial.println("Wire.write()...");
    Wire.beginTransmission(RTC_I2C_ADDRESS);
    Wire.write(0); //count 0 activates RTC module
    Wire.write(decToBcd(sekunde));
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(stunde));
    Wire.write(decToBcd(0)); // weekdays not respected
    Wire.write(decToBcd(tag));
    Wire.write(decToBcd(monat));
    Wire.write(decToBcd(jahr - 2000));
    Wire.endTransmission();
    Serial.println("Configuration page now available...");
  }
}


// ###########################################################################################################################################
// # Handle the time from the NTP server and write it to the RTC:
// ###########################################################################################################################################
void handleTime() {
  // Check, whether we are connected to WLAN
  if ((WiFi.status() == WL_CONNECTED)) {
    time_t now;
    time(&now);                       // read the current time
    struct tm  ti;
    localtime_r(&now, &ti);
    int i = ti.tm_year + ti.tm_mon + ti.tm_mday + ti.tm_hour;
    if (i != lastRequest) {
      Serial.print("Set RTC to current time: ");
      lastRequest = i;
      // check for update
      uint16_t ye = ti.tm_year + 1900;
      uint8_t  mo = ti.tm_mon + 1;
      uint8_t  da = ti.tm_mday;
      int ho  = ti.tm_hour;
      int mi  = ti.tm_min;
      int sec = ti.tm_sec;
      Serial.print (ho);
      Serial.print(':');
      Serial.print (mi);
      Serial.print(':');
      Serial.print (sec);
      Serial.print("   ==  ");
      Serial.print(ye);
      Serial.print('-');
      Serial.print(mo);
      Serial.print('-');
      Serial.println(da);
      // set working timestamp
      iYear  = ye;
      iMonth = mo;
      iDay   = da;
      iHour   = ho;
      iMinute = mi;
      iSecond = sec;
      rtcWriteTime(ye, mo, da, ho, mi, sec);
    }
  }
  rtcReadTime();
}


// ###########################################################################################################################################
// # Every Hour blink orange DCW - if switched on:
// ###########################################################################################################################################
void showDCW() {
  if ((dcwFlag) &&
      (iMinute == 0) &&
      (iSecond < 10)) {
    if ((iSecond % 2) == 0) {
      pixels.setPixelColor( 59, pixels.Color(255, 128, 0));
      pixels.setPixelColor( 60, pixels.Color(255, 128, 0));
      pixels.setPixelColor( 61, pixels.Color(255, 128, 0));
    } else {
      pixels.setPixelColor( 59, pixels.Color(0, 0, 0));
      pixels.setPixelColor( 60, pixels.Color(0, 0, 0));
      pixels.setPixelColor( 61, pixels.Color(0, 0, 0));
    }
  }
}


// ###########################################################################################################################################
// # Display the time:
// ###########################################################################################################################################
void ShowTheTime() {
  if (iSecond == 30) {
    if (showDate)
      showCurrentDate();
  }
  showCurrentTime();
  showDCW();
}


// ###########################################################################################################################################
// # Handle day and night time mode:
// ###########################################################################################################################################
void DayNightMode(int displayonMin, int displayonMax) {
  if (iHour > displayonMin && iHour < displayonMax) {
    ShowTheTime();
  }
  else
  {
    if (useNightLEDs == -1) {
      pixels.setBrightness(intensityNight); // Night brightness
      ShowTheTime();
    }
    else
    {
      dunkel();
    }
  }
}


// ###########################################################################################################################################
// # NTP time function:
// ###########################################################################################################################################
void configNTPTime() {
  Serial.print("Set time zone to ");
  Serial.println(timeZone);
  Serial.print("Set time server to ");
  Serial.println(ntpServer);
  configTime(timeZone.begin(), ntpServer.begin());
}


// ###########################################################################################################################################
// # Convert hex digit to int value:
// ###########################################################################################################################################
unsigned char h2int(char c)
{
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}


// ###########################################################################################################################################
// # Get RTC - if any:
// ###########################################################################################################################################
int checkRTC() {
  // Initialize & check RTC
  if (!rtcStarted) {
    if (rtc.begin()) {
      rtcStarted = -1;
      // start RTC Communication via Wire.h library
      Serial.println("Start RTC communication");
      Wire.begin();
    } else {
      Serial.println("Couldn't find RTC");
    }
  }
  return rtcStarted;
}


// ###########################################################################################################################################
// # Decode %xx values in String - comming from URL / HTTP:
// ###########################################################################################################################################
String urldecode(String str)
{
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {
      encodedString += c;
    }
    yield();
  }
  return encodedString;
}


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################
