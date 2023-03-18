// ###########################################################################################################################################
// #
// # WordClock code for the thingiverse WordClock project: https://www.thingiverse.com/thing:4693081
// #
// # Code by https://github.com/N1cls and https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0 https://github.com/N1cls/Wordclock/blob/master/LICENSE.md
// #
// ###########################################################################################################################################
/*
      ___           ___           ___           ___           ___           ___       ___           ___           ___     
     /\__\         /\  \         /\  \         /\  \         /\  \         /\__\     /\  \         /\  \         /\__\    
    /:/ _/_       /::\  \       /::\  \       /::\  \       /::\  \       /:/  /    /::\  \       /::\  \       /:/  /    
   /:/ /\__\     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/  /    /:/\:\  \     /:/\:\  \     /:/__/     
  /:/ /:/ _/_   /:/  \:\  \   /::\~\:\  \   /:/  \:\__\   /:/  \:\  \   /:/  /    /:/  \:\  \   /:/  \:\  \   /::\__\____ 
 /:/_/:/ /\__\ /:/__/ \:\__\ /:/\:\ \:\__\ /:/__/ \:|__| /:/__/ \:\__\ /:/__/    /:/__/ \:\__\ /:/__/ \:\__\ /:/\:::::\__\
 \:\/:/ /:/  / \:\  \ /:/  / \/_|::\/:/  / \:\  \ /:/  / \:\  \  \/__/ \:\  \    \:\  \ /:/  / \:\  \  \/__/ \/_|:|~~|~   
  \::/_/:/  /   \:\  /:/  /     |:|::/  /   \:\  /:/  /   \:\  \        \:\  \    \:\  /:/  /   \:\  \          |:|  |    
   \:\/:/  /     \:\/:/  /      |:|\/__/     \:\/:/  /     \:\  \        \:\  \    \:\/:/  /     \:\  \         |:|  |    
    \::/  /       \::/  /       |:|  |        \::/__/       \:\__\        \:\__\    \::/  /       \:\__\        |:|  |    
     \/__/         \/__/         \|__|         ~~            \/__/         \/__/     \/__/         \/__/         \|__|    
*/


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
#include <EEPROM.h>                   // Used to store the in the internal configuration page set configuration on the ESP internal storage
#include <Adafruit_NeoPixel.h>        // Used to drive the NeoPixel LEDs
#include <time.h>                     // Used to get the time from the internet
#include <Wire.h>                     // Used to connect the RTC board
#include <ESP8266mDNS.h>              // Used for the internal update function
#include <ESP8266httpUpdate.h>        // Used for the http update function
#include <ESP8266HTTPUpdateServer.h>  // Used for the internal update function
#include "RTClib.h"                   // Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <ESP8266Ping.h>              // Used to send ping requests to a IP-address (of your smart phone) to detect if you have left your home
#include "settings.h"                 // Settings are stored in a seperate file to make to code better readable and to be able to switch to other settings faster
#include "languages.h"                // Translation for texts for the HTML page


// ###########################################################################################################################################
// # Version number of the code:
// ###########################################################################################################################################
const char* WORD_CLOCK_VERSION = "V5.9.3";


// ###########################################################################################################################################
// # Declartions and variables used in the functions:
// ###########################################################################################################################################
String header;                                                                       // Variable to store the HTTP request
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // LED strip settings
RTC_DS3231 rtc;                                                                      // rtc communication object
int lastRequest = 0;                                                                 // Variable to control RTC requests
int rtcStarted = 0;                                                                  // Variable to control whether RTC has been initialized
int delayval = 250;                                                                  // delay in milliseconds
int iYear, iMonth, iDay, iHour, iMinute, iSecond, iWeekDay;                          // variables for RTC-module read time:
String timeZone = DEFAULT_TIMEZONE;                                                  // Time server settings
String ntpServer = DEFAULT_NTP_SERVER;                                               // Time server settings
String UpdatePath = "-";                                                             // Update via Hostname
String UpdatePathIP = "-";                                                           // Update via IP-address
ESP8266HTTPUpdateServer httpUpdater;                                                 // Update server
std::unique_ptr<ESP8266WebServer> server1;                                           // REST function web server
int PING_ATTEMPTSIP1 = PING_TIMEOUTNUM;                                              // 1st IP-addres attempts of missed PING requests
int PING_ATTEMPTSIP2 = PING_TIMEOUTNUM;                                              // 2nd IP-addres attempts of missed PING requests
int PING_ATTEMPTSIP3 = PING_TIMEOUTNUM;                                              // 3rd IP-addres attempts of missed PING requests
bool PingStatusIP1 = true;                                                           // Status flag 1st IP-address - PING function
bool PingStatusIP2 = true;                                                           // Status flag 2nd IP-address - PING function
bool PingStatusIP3 = true;                                                           // Status flag 3rd IP-address - PING function
bool LEDsON = true;                                                                  // Global flag to turn LEDs on or off - Used for the PING function
bool RESTmanLEDsON = true;                                                           // Global flag to turn LEDs manually on or off - Used for the REST function
bool UpdateAvailable = false;                                                        // Global flag to check for avaiable updates
String AvailableVersion = "-";                                                       // Global string to check for avaiable updates


// ###########################################################################################################################################
// # Parameter record to store to the EEPROM of the ESP:
// ###########################################################################################################################################
struct parmRec {
  int pRed;
  int pGreen;
  int pBlue;
  int pIntensity;
  int pIntensityNight;
  int pShowDate;
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
  int pdisplayoff;
  int puseNightLEDs;
  int puseupdate;
  int puseresturl;
  int ppowersupply;
  int puseledtest;
  int pusesetwlan;
  int puseshowip;
  int pswitchRainBow;
  int pswitchLangWeb;
  int pswitchLEDOrder;
  int pwchostnamenum;
  int pDCWFlag;
  int puseRTC;
  int pBlinkTime;
  int pPING_IP_ADDR1_O1;
  int pPING_IP_ADDR1_O2;
  int pPING_IP_ADDR1_O3;
  int pPING_IP_ADDR1_O4;
  int pPING_IP_ADDR2_O1;
  int pPING_IP_ADDR2_O2;
  int pPING_IP_ADDR2_O3;
  int pPING_IP_ADDR2_O4;
  int pPING_IP_ADDR3_O1;
  int pPING_IP_ADDR3_O2;
  int pPING_IP_ADDR3_O3;
  int pPING_IP_ADDR3_O4;
  int pPING_TIMEOUTNUM;
  int pPING_DEBUG_MODE;
  int pPING_USEMONITOR;
  int pDEspecial1;
  char pTimeZone[50];
  char pNTPServer[50];
  int pCheckSum;  // This checkSum is used to find out whether we have valid parameters
} parameter;


// ###########################################################################################################################################
// # Setup function that runs once at startup of the ESP:
// ###########################################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("######################################################################");
  Serial.print("# WordClock startup of version: ");
  Serial.println(WORD_CLOCK_VERSION);
  Serial.println("######################################################################");
  dunkel();                         // Switch display black
  pixels.begin();                   // Init the NeoPixel library
  readEEPROM();                     // get persistent data from EEPROM
  pixels.setBrightness(intensity);  // Set LED brightness
  DisplayTest();                    // Perform the LED test
  SetWLAN();                        // Show SET WLAN text
  WIFI_login();                     // WiFiManager
  if (useshowip) showIP();          // Show IP-address on display

  if (useupdate) {  // Local web update function setup
    MDNS.begin(wchostname + wchostnamenum);
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", 2022);
    MDNS.addService("http", "tcp", 80);
    UpdatePath = "http://" + String(wchostname + wchostnamenum) + ".local:2022/update";
    UpdatePathIP = "http://" + WiFi.localIP().toString() + ":2022/update";
  }
  server.begin();

  // REST function web server:
  if (useresturl) {
    server1.reset(new ESP8266WebServer(WiFi.localIP(), server1port));
    server1->on("/", handleRoot);
    server1->on("/ledson", ledsON);
    server1->on("/ledsoff", ledsOFF);
    server1->on("/clockrestart", ClockRestart);
    server1->on("/clockwifireset", ClockWifiReset);
    server1->on("/ledstatus", LedStatus);
    server1->on("/esphttpupdate", esphttpupdate);
    server1->begin();
  }

  setLanguage(switchLangWeb);          // Load set language
  if (useupdate == 2) readhttpfile();  // Check for updates on the http server if automatic update function used

  Serial.println("######################################################################");
  Serial.println("# WordClock startup finished...");
  Serial.println("######################################################################");
  Serial.print("# Configuration page now available: ");
  Serial.println("http://" + WiFi.localIP().toString());
  Serial.println("######################################################################");
}


// ###########################################################################################################################################
// # Loop function which runs all the time after the startup was done:
// ###########################################################################################################################################
void loop() {
  ESP.wdtFeed();  // Reset watchdog timer

  // Check WiFi connection and reconnect if needed:
  if (WiFi.status() != WL_CONNECTED) {
    WIFI_login();  // WiFi inactive --> Reconnect to it...
  } else {         // Wifi active --> Sort some atoms in the univers and show the time...
    // Check, whether something has been entered on Config Page
    checkClient();
    ESP.wdtFeed();  // Reset watchdog timer
    handleTime();   // handle NTP / RTC time
    delay(750);

    if (switchRainBow == 2) {  // RainBow variant 2 effect active - color change every new minute
      if (iSecond == 0) {
        redVal = random(255);
        greenVal = random(255);
        blueVal = random(255);
      }
    } else {
      redVal = redVal;
      greenVal = greenVal;
      blueVal = blueVal;
    }

    // Show the display only during the set Min/Max time if option is set
    if (displayoff) {
      switch (iWeekDay) {
        case 0:  // Sunday
          DayNightMode(displayonminSU, displayonmaxSU);
          break;
        case 1:  // Monday
          DayNightMode(displayonminMO, displayonmaxMO);
          break;
        case 2:  // Tuesday
          DayNightMode(displayonminTU, displayonmaxTU);
          break;
        case 3:  // Wednesday
          DayNightMode(displayonminWE, displayonmaxWE);
          break;
        case 4:  // Thursday
          DayNightMode(displayonminTH, displayonmaxTH);
          break;
        case 5:  // Friday
          DayNightMode(displayonminFR, displayonmaxFR);
          break;
        case 6:  // Saturday
          DayNightMode(displayonminSA, displayonmaxSA);
          break;
      }
    } else {
      pixels.setBrightness(intensity);  // DAY brightness
      ShowTheTime();
    }

    ESP.wdtFeed();  // Reset watchdog timer
    delay(delayval);
    ESP.wdtFeed();  // Reset watchdog timer

    // Web update start:
    httpServer.handleClient();
    MDNS.update();

    // PING function:
    if (PING_USEMONITOR == 1) PingIP();

    if (LEDsON == true && RESTmanLEDsON == true) pixels.show();  // This sends the updated pixel color to the hardware.
  }

  // REST function web server:
  if (useresturl) server1->handleClient();

  // Reset the previous read update information every new hour to refresh the information when the configuration page is manually loaded again
  if ((useupdate == 2) && (iMinute == 0) && (iSecond == 0)) {
    UpdateAvailable = false;
    AvailableVersion = "-";
  }
}


// ###########################################################################################################################################
// # Try to read settings from FLASH - initialize if WLAN ID read from flash is invalid:
// ###########################################################################################################################################
void readEEPROM() {
  //Serial.print("Copy ");
  //Serial.print(sizeof(parameter));
  //Serial.println(" bytes from flash memory to EPROM buffer: ");

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
    /*  Serial.print("Read FLASH Byte ");
      Serial.print(L);
      Serial.print(" = ");
      Serial.println(b);  */
  }

  // Check checksum
  //Serial.print("Compare checksums: ");
  //Serial.print(check);
  //Serial.print("/");
  //Serial.println(parameter.pCheckSum);

  if (check == parameter.pCheckSum) {
    // Serial.println("Checksum does match. Get parameter values from EEPROM");
    redVal = parameter.pRed;
    greenVal = parameter.pGreen;
    blueVal = parameter.pBlue;
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
    useresturl = parameter.puseresturl;
    powersupply = parameter.ppowersupply;
    useledtest = parameter.puseledtest;
    usesetwlan = parameter.pusesetwlan;
    useshowip = parameter.puseshowip;
    switchRainBow = parameter.pswitchRainBow;
    switchLangWeb = parameter.pswitchLangWeb;
    switchLEDOrder = parameter.pswitchLEDOrder;
    blinkTime = parameter.pBlinkTime;
    DEspecial1 = parameter.pDEspecial1;
    dcwFlag = parameter.pDCWFlag;
    useRTC = parameter.puseRTC;
    intensity = parameter.pIntensity;
    intensityNight = parameter.pIntensityNight;
    PING_IP_ADDR1_O1 = parameter.pPING_IP_ADDR1_O1;
    PING_IP_ADDR1_O2 = parameter.pPING_IP_ADDR1_O2;
    PING_IP_ADDR1_O3 = parameter.pPING_IP_ADDR1_O3;
    PING_IP_ADDR1_O4 = parameter.pPING_IP_ADDR1_O4;
    PING_IP_ADDR2_O1 = parameter.pPING_IP_ADDR2_O1;
    PING_IP_ADDR2_O2 = parameter.pPING_IP_ADDR2_O2;
    PING_IP_ADDR2_O3 = parameter.pPING_IP_ADDR2_O3;
    PING_IP_ADDR2_O4 = parameter.pPING_IP_ADDR2_O4;
    PING_IP_ADDR3_O1 = parameter.pPING_IP_ADDR3_O1;
    PING_IP_ADDR3_O2 = parameter.pPING_IP_ADDR3_O2;
    PING_IP_ADDR3_O3 = parameter.pPING_IP_ADDR3_O3;
    PING_IP_ADDR3_O4 = parameter.pPING_IP_ADDR3_O4;
    PING_TIMEOUTNUM = parameter.pPING_TIMEOUTNUM;
    PING_DEBUG_MODE = parameter.pPING_DEBUG_MODE;
    PING_USEMONITOR = parameter.pPING_USEMONITOR;
    String ntp(parameter.pNTPServer);
    ntpServer = ntp;
    String tz(parameter.pTimeZone);
    timeZone = tz;
  } else {
    Serial.println("Checksum does not match. New program version or new installed ESP detected...");
  }
}


// ###########################################################################################################################################
// # Write current parameter settings to flash:
// ###########################################################################################################################################
void writeEEPROM() {
  //Serial.println("Write parameter into EEPROM");
  parameter.pRed = redVal;
  parameter.pGreen = greenVal;
  parameter.pBlue = blueVal;
  parameter.pIntensity = intensity;
  parameter.pIntensityNight = intensityNight;
  parameter.pDCWFlag = dcwFlag;
  parameter.puseRTC = useRTC;
  parameter.pBlinkTime = blinkTime;
  parameter.pDEspecial1 = DEspecial1;
  ntpServer.toCharArray(parameter.pNTPServer, sizeof(parameter.pNTPServer));
  timeZone.toCharArray(parameter.pTimeZone, sizeof(parameter.pTimeZone));
  parameter.pShowDate = showDate;
  parameter.pdisplayoff = displayoff;
  parameter.puseNightLEDs = useNightLEDs;
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
  parameter.puseupdate = useupdate;
  parameter.puseresturl = useresturl;
  parameter.ppowersupply = powersupply;
  parameter.puseledtest = useledtest;
  parameter.pusesetwlan = usesetwlan;
  parameter.puseshowip = useshowip;
  parameter.pswitchRainBow = switchRainBow;
  parameter.pswitchLangWeb = switchLangWeb;
  parameter.pswitchLEDOrder = switchLEDOrder;
  parameter.pPING_IP_ADDR1_O1 = PING_IP_ADDR1_O1;
  parameter.pPING_IP_ADDR1_O2 = PING_IP_ADDR1_O2;
  parameter.pPING_IP_ADDR1_O3 = PING_IP_ADDR1_O3;
  parameter.pPING_IP_ADDR1_O4 = PING_IP_ADDR1_O4;
  parameter.pPING_IP_ADDR2_O1 = PING_IP_ADDR2_O1;
  parameter.pPING_IP_ADDR2_O2 = PING_IP_ADDR2_O2;
  parameter.pPING_IP_ADDR2_O3 = PING_IP_ADDR2_O3;
  parameter.pPING_IP_ADDR2_O4 = PING_IP_ADDR2_O4;
  parameter.pPING_IP_ADDR3_O1 = PING_IP_ADDR3_O1;
  parameter.pPING_IP_ADDR3_O2 = PING_IP_ADDR3_O2;
  parameter.pPING_IP_ADDR3_O3 = PING_IP_ADDR3_O3;
  parameter.pPING_IP_ADDR3_O4 = PING_IP_ADDR3_O4;
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
  WiFiClient client = server.available();  // Listen for incoming clients // @suppress("Abstract class cannot be instantiated")

  if (client) {  // If a new client connects,
    //Serial.println("New web client...");          // print a message out in the serial port
    String currentLine = "";      // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {   // if there's bytes to read from the client,
        char c = client.read();   // read a byte, then
        // Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            // Display the HTML web page:
            // ##########################
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>" + WordClockName + " - " + wchostname + wchostnamenum + "</title><meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");


            // Web Page Heading:
            // #################
            String title = "<body><h1>" + WordClockName + " '" + wchostname + wchostnamenum + "' " + txtSettings + ": ";
            title = title + WORD_CLOCK_VERSION;
            title = title + "</h1>";
            client.println(title);
            client.println("<form action=\"/setWC.php\">");


            // Convert color into hex settings:
            // ################################
            if (parameter.pRed >= 0) {  // Load defined color from EEPROM - important after Rainbow random function usage
              redVal = parameter.pRed;
              greenVal = parameter.pGreen;
              blueVal = parameter.pBlue;
            }
            char hex_main[7] = { 0 };
            sprintf(hex_main, "#%02X%02X%02X", redVal, greenVal, blueVal);
            client.println("<hr><h2>" + txtLEDsettings + ":</h2><br>");
            client.println("<label for=\"favcolor\">" + txtLEDcolor + ": </label>");
            client.print("<input type=\"color\" id=\"favcolor\" name=\"favcolor\" value=\"");
            client.print(hex_main);
            client.print("\"><br><br>");


            // Intensity Day:
            // ##############
            client.print("<label for=\"intensity\">" + txtIntensityDay + ": </label>");
            if (powersupply == 0) {
              client.print("<input type='range' id='intensity' name='intensity' min='1' max='128' value=");
            } else {
              client.print("<input type='range' id='intensity' name='intensity' min='1' max='255' value=");
            }
            client.print(intensity);
            client.println(" style='height:30px; width:200px' oninput='showValue(this.value)'>");
            client.print("<span id='valrange'>");
            client.print(intensity);
            client.println("</span>");
            client.println("<script type='text/javascript'>\r\nfunction showValue(newValue)\r\n{\r\ndocument.getElementById('valrange').innerHTML=newValue;\r\n}\r\n</script>\r\n");
            client.println("</label><br><br>");


            // Intensity Night:
            // ################
            client.print("<label for=\"intensity\">" + txtIntensityNight + ": </label>");
            if (powersupply == 0) {
              client.print("<input type='range' id='intensityNight' name='intensityNight' min='1' max='128' value=");
            } else {
              client.print("<input type='range' id='intensityNight' name='intensityNight' min='1' max='255' value=");
            }
            client.print(intensityNight);
            client.println(" style='height:30px; width:200px' oninput='showValueNight(this.value)'>");
            client.print("<span id='valrangeNight'>");
            client.print(intensityNight);
            client.println("</span>");
            client.println("<script type='text/javascript'>\r\nfunction showValueNight(newValue)\r\n{\r\ndocument.getElementById('valrangeNight').innerHTML=newValue;\r\n}\r\n</script>\r\n");
            client.println("</label><br><br>");

            if (powersupply == 0) {
              client.print("<label>" + txtPowerSupplyNote1 + "<br>" + txtPowerSupplyNote2 + "</label><br>");
              client.print("<br><label>" + txtPowerSupplyNote3 + ": </label>");
            } else {
              client.print("<label>" + txtPowerSupplyNote4 + ": </label>");
            }
            client.print("<input type=\"checkbox\" id=\"powersupply\" name=\"powersupply\"");
            if (powersupply)
              client.print(" checked");
            client.print("><br><hr>");


            // Flash hour value to every full hour:
            // ####################################
            client.println("<h2>" + txtFlashFullHour1 + ":</h2><br>");
            client.println("<label for=\"showdate\">" + txtFlashFullHour2 + "</label>");
            client.print("<input type=\"checkbox\" id=\"blinktime\" name=\"blinktime\"");
            if (blinkTime)
              client.print(" checked");
            client.print("><br><hr>");


            // Show date value as scrolling text every minute after 30s:
            // #########################################################
            client.println("<h2>" + txtShowDate1 + ":</h2><br>");
            client.println("<label for=\"showdate\">" + txtShowDate2 + ": </label>");
            client.print("<input type=\"checkbox\" id=\"showdate\" name=\"showdate\"");
            if (showDate)
              client.print(" checked");
            client.print(">");
            if (checkRTC() && useRTC == 0) {
              client.println("<br><h2 style=\"text-decoration:blink;color:green\">RTC found. RTC usage will be enabled after saving this page.</h2>");
              useRTC = 1;
              Serial.println("RTC board found. RTC usage will be enabled after saving the configuration page.");
            }
            if (!checkRTC() && useRTC == 1) {
              client.println("<br><h2 style=\"text-decoration:blink;color:red\">No RTC found. RTC usage will be disabled after saving this page.</h2>");
              useRTC = 0;
              Serial.println("No RTC found. RTC usage will be disabled after saving the configuration page!");
            }
            client.print("<br><hr>");


            // Night mode - display off:
            // ###########
            client.println("<h2>" + txtNightMode1 + ":</h2><br>");
            client.println("<label for=\"displayoff\">" + txtNightMode2 + " </label>");
            client.print("<input type=\"checkbox\" id=\"displayoff\" name=\"displayoff\"");
            if (displayoff) {
              client.print(" checked");
            }
            client.print("><br><br>");


            // Night mode - display darker:
            // ############################
            client.println("<label for=\"useNightLEDs\">" + txtNightMode3 + " </label>");
            client.print("<input type=\"checkbox\" id=\"useNightLEDs\" name=\"useNightLEDs\"");
            if (useNightLEDs) {
              client.print(" checked");
            }
            client.print("><br><br>");

            // Monday:
            // #######
            client.println("<label for=\"displayonmaxMO\">" + txtMO + " - " + txtNightModeOff + ": </label><select id=\"displayonmaxMO\" name=\"displayonmaxMO\" ><option selected=\"selected\">");
            client.println(displayonmaxMO);
            client.println("</option><option>15</option><option>16</option><option>17</option><option>18</option><option>19</option><option>20</option><option>21</option><option>22</option><option>23</option>");
            client.println("</select>:00 " + txtNightModeTo + " <select id=\"displayonminMO\" name=\"displayonminMO\" ><option selected=\"selected\">");
            client.println(displayonminMO);
            client.println("</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select>:59 " + txtNightModeClock + " <br><br>");

            // Tuesday:
            // ########
            client.println("<label for=\"displayonmaxTU\">" + txtTU + " - " + txtNightModeOff + ": </label><select id=\"displayonmaxTU\" name=\"displayonmaxTU\" ><option selected=\"selected\">");
            client.println(displayonmaxTU);
            client.println("</option><option>15</option><option>16</option><option>17</option><option>18</option><option>19</option><option>20</option><option>21</option><option>22</option><option>23</option>");
            client.println("</select>:00 " + txtNightModeTo + " <select id=\"displayonminTU\" name=\"displayonminTU\" ><option selected=\"selected\">");
            client.println(displayonminTU);
            client.println("</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select>:59 " + txtNightModeClock + " <br><br>");
            ;

            // Wednesday:
            // ##########
            client.println("<label for=\"displayonmaxWE\">" + txtWE + " - " + txtNightModeOff + ": </label><select id=\"displayonmaxWE\" name=\"displayonmaxWE\" ><option selected=\"selected\">");
            client.println(displayonmaxWE);
            client.println("</option><option>15</option><option>16</option><option>17</option><option>18</option><option>19</option><option>20</option><option>21</option><option>22</option><option>23</option>");
            client.println("</select>:00 " + txtNightModeTo + " <select id=\"displayonminWE\" name=\"displayonminWE\" ><option selected=\"selected\">");
            client.println(displayonminWE);
            client.println("</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select>:59 " + txtNightModeClock + " <br><br>");

            // Thursday:
            // #########
            client.println("<label for=\"displayonmaxTH\">" + txtTH + " - " + txtNightModeOff + ":  </label><select id=\"displayonmaxTH\" name=\"displayonmaxTH\" ><option selected=\"selected\">");
            client.println(displayonmaxTH);
            client.println("</option><option>15</option><option>16</option><option>17</option><option>18</option><option>19</option><option>20</option><option>21</option><option>22</option><option>23</option>");
            client.println("</select>:00 " + txtNightModeTo + " <select id=\"displayonminTH\" name=\"displayonminTH\" ><option selected=\"selected\">");
            client.println(displayonminTH);
            client.println("</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select>:59 " + txtNightModeClock + " <br><br>");

            // Friday:
            // #######
            client.println("<label for=\"displayonmaxFR\">" + txtFR + " - " + txtNightModeOff + ": </label><select id=\"displayonmaxFR\" name=\"displayonmaxFR\" ><option selected=\"selected\">");
            client.println(displayonmaxFR);
            client.println("</option><option>15</option><option>16</option><option>17</option><option>18</option><option>19</option><option>20</option><option>21</option><option>22</option><option>23</option>");
            client.println("</select>:00 " + txtNightModeTo + " <select id=\"displayonminFR\" name=\"displayonminFR\" ><option selected=\"selected\">");
            client.println(displayonminFR);
            client.println("</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select>:59 " + txtNightModeClock + " <br><br>");

            // Saturday:
            // #########
            client.println("<label for=\"displayonmaxSA\">" + txtSA + " - " + txtNightModeOff + ": </label><select id=\"displayonmaxSA\" name=\"displayonmaxSA\" ><option selected=\"selected\">");
            client.println(displayonmaxSA);
            client.println("</option><option>15</option><option>16</option><option>17</option><option>18</option><option>19</option><option>20</option><option>21</option><option>22</option><option>23</option>");
            client.println("</select>:00 " + txtNightModeTo + " <select id=\"displayonminSA\" name=\"displayonminSA\" ><option selected=\"selected\">");
            client.println(displayonminSA);
            client.println("</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select>:59 " + txtNightModeClock + " <br><br>");

            // Sunday:
            // #######
            client.println("<label for=\"displayonmaxSU\">" + txtSU + " - " + txtNightModeOff + ": </label><select id=\"displayonmaxSU\" name=\"displayonmaxSU\" ><option selected=\"selected\">");
            client.println(displayonmaxSU);
            client.println("</option><option>15</option><option>16</option><option>17</option><option>18</option><option>19</option><option>20</option><option>21</option><option>22</option><option>23</option>");
            client.println("</select>:00 " + txtNightModeTo + " <select id=\"displayonminSU\" name=\"displayonminSU\" ><option selected=\"selected\">");
            client.println(displayonminSU);
            client.println("</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select>:59 " + txtNightModeClock + " <br><br><hr>");


            // LED display and startup:
            // ########################

            // LED test:
            client.println("<h2>" + txtContentStartup + ":</h2>");
            client.println("<label for=\"useledtest\">" + txtUseLEDtest + " </label>");
            client.print("<input type=\"checkbox\" id=\"useledtest\" name=\"useledtest\"");
            if (useledtest) {
              client.print(" checked");
              client.print("><br><br>");
            } else {
              client.print("><br><br>");
            }

            // SET WLAN text:
            client.println("<label for=\"usesetwlan\">" + txtUSEsetWLAN + " </label>");
            client.print("<input type=\"checkbox\" id=\"usesetwlan\" name=\"usesetwlan\"");
            if (usesetwlan) {
              client.print(" checked");
              client.print("><br><br>");
            } else {
              client.print("><br><br>");
            }

            // Show IP-address:
            client.println("<label for=\"useshowip\">" + txtShowIP + " </label>");
            client.print("<input type=\"checkbox\" id=\"useshowip\" name=\"useshowip\"");
            if (useshowip) {
              client.print(" checked");
              client.print("><br><br>");
            } else {
              client.print("><br><br>");
            }

            // Rainbow:
            client.println("<br><label for=\"switchRainBow\">" + txtRainbow1 + ":</label>");
            client.println("<fieldset>");
            client.println("<div>");
            client.println("<input type='radio' id='id0' name='switchRainBow' value='0'");
            if (switchRainBow == 0) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='id0'>" + txtRainbow2 + "</label>");
            client.println("</div>");
            client.println("<div>");
            client.println("<input type='radio' id='id1' name='switchRainBow' value='1'");
            if (switchRainBow == 1) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='id1'>" + txtRainbow3 + "</label>");
            client.println("</div>");
            client.println("<div>");
            client.println("<input type='radio' id='id2' name='switchRainBow' value='2'");
            if (switchRainBow == 2) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='id2'>" + txtRainbow4 + "</label>");
            client.println("</div>");
            client.println("</fieldset>");

            // Minute direction:
            client.println("<br><br><label for=\"switchLEDOrder\">" + txtMinDir1 + "</label>");
            client.print("<input type=\"checkbox\" id=\"switchLEDOrder\" name=\"switchLEDOrder\"");
            if (switchLEDOrder) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<br><br>" + txtMinDir2 + "<br>");
            client.println(txtMinDir3 + "<br><hr>");


            // Language selection:
            // ###################
            client.println("<br><label for=\"switchLangWeb\"><h2>" + languageSelect + ":</h2></label>");
            client.println("<fieldset>");
            client.println("<div>");
            client.println("<input type='radio' id='idlang0' name='switchLangWeb' value='0'");
            if (switchLangWeb == 0) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='idlang0'>" + languageInt0 + "</label>");
            client.println("</div>");
            client.println("<div>");
            client.println("<input type='radio' id='idlang1' name='switchLangWeb' value='1'");
            if (switchLangWeb == 1) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='idlang1'>" + languageInt1 + "</label>");
            client.println("</div>");
            client.println("</fieldset>");
            client.println("<br><br><hr>");



            // DE special parameter VIERTEL VOR vs. DREIVIERTEL selection:
            // ###########################################################
            if (switchLangWeb == 0) {
              client.println("<br><label for=\"DEspecial1\"><h2>" + DEspecial1Text1 + ":</h2></label>");
              client.println("<fieldset>");
              client.println("<div>");

              client.println("<input type='radio' id='iddespecial0' name='DEspecial1' value='0'");
              if (DEspecial1 == 0) {
                client.print(" checked");
                client.print(">");
              } else {
                client.print(">");
              }
              client.println("<label for='iddespecial0'>" + DEspecial1Text2 + "</label>");
              client.println("</div>");
              client.println("<div>");

              client.println("<input type='radio' id='iddespecial1' name='DEspecial1' value='1'");
              if (DEspecial1 == 1) {
                client.print(" checked");
                client.print(">");
              } else {
                client.print(">");
              }
              client.println("<label for='iddespecial1'>" + DEspecial1Text3 + "</label>");
              client.println("</div>");
              client.println("</fieldset>");
              client.println("<br><br><hr>");
            }



            // PING IP-address:
            // ################
            client.println("<h2>" + txtPing0 + ":</h2>");
            client.println("<label for=\"PING_USEMONITOR\">" + txtPing1 + "</label>");
            client.print("<input type=\"checkbox\" id=\"PING_USEMONITOR\" name=\"PING_USEMONITOR\"");
            if (PING_USEMONITOR) {
              client.print(" checked");
              client.print("><br><br>");
            } else {
              client.print("><br><br>");
            }
            client.println("<label>" + txtPing2 + ":</label><br><br>");
            client.println("<label for=\"PING_IP_ADDR1_O1\">" + txtPing3 + ": </label>");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR1_O1\" name=\"PING_IP_ADDR1_O1\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR1_O1);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR1_O2\" name=\"PING_IP_ADDR1_O2\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR1_O2);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR1_O3\" name=\"PING_IP_ADDR1_O3\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR1_O3);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR1_O4\" name=\"PING_IP_ADDR1_O4\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR1_O4);
            client.println("\"><br>");
            client.println("<label for=\"PING_IP_ADDR2_O1\">" + txtPing4 + ": </label>");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR2_O1\" name=\"PING_IP_ADDR2_O1\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR2_O1);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR2_O2\" name=\"PING_IP_ADDR2_O2\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR2_O2);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR2_O3\" name=\"PING_IP_ADDR2_O3\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR2_O3);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR2_O4\" name=\"PING_IP_ADDR2_O4\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR2_O4);
            client.println("\"><br>");
            client.println("<label for=\"PING_IP_ADDR3_O1\">" + txtPing5 + ": </label>");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR3_O1\" name=\"PING_IP_ADDR3_O1\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR3_O1);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR3_O2\" name=\"PING_IP_ADDR3_O2\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR3_O2);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR3_O3\" name=\"PING_IP_ADDR3_O3\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR3_O3);
            client.println("\"> .");
            client.print("<input type=\"text\" id=\"PING_IP_ADDR3_O4\" name=\"PING_IP_ADDR3_O4\" size=\"3\" value=\"");
            client.print(PING_IP_ADDR3_O4);
            client.println("\"><br>");
            client.println("<br><label>" + txtPing6 + "</label><br>");
            client.println("<br><label for=\"PING_TIMEOUTNUM\">" + txtPing7 + ": </label>");
            client.print("<input type=\"text\" id=\"PING_TIMEOUTNUM\" name=\"PING_TIMEOUTNUM\" size=\"3\" value=\"");
            client.print(PING_TIMEOUTNUM);
            client.println("\">");
            client.println("<br><label>" + txtPing8 + "</label><br><br>");
            client.println("<label for=\"PING_DEBUG_MODE\">" + txtPing9 + " </label>");
            client.print("<input type=\"checkbox\" id=\"PING_DEBUG_MODE\" name=\"PING_DEBUG_MODE\"");
            if (PING_DEBUG_MODE) {
              client.print(" checked");
              client.print("><br>");
            } else {
              client.print("><br>");
            }
            client.print("<br><hr>");


            // Hostname:
            // #########
            client.println("<h2>" + txtHostName1 + ":</h2><br>");
            client.println("<label for=\"wchostnamenum\">" + txtHostName2 + ": " + wchostname + "</label>");
            client.println("<select id=\"wchostnamenum\" name=\"wchostnamenum\" >");
            client.println("<option selected=\"selected\">");
            client.println(wchostnamenum);
            client.println("</option><option>0</option><option>1</option><option>2</option><option>3</option><option>4</option><option>5</option><option>6</option><option>7</option><option>8</option><option>9</option></select><br><br><hr>");


            // REST functions:
            // ###############
            client.println("<h2>" + txtREST0 + ":</h2>");
            client.println("<label>" + txtREST1 + "</label><br><br>");
            client.println("<label for=\"useresturl\">" + txtREST2 + " </label>");
            client.print("<input type=\"checkbox\" id=\"useresturl\" name=\"useresturl\"");
            if (useresturl) {
              client.print(" checked");
              client.print("><br><br>");
              client.println("<label>" + txtREST3 + ": </label>");
              client.println("<br>");
              client.println("<label>" + txtREST4 + ": </label>");
              client.println("<a href=http://" + WiFi.localIP().toString() + ":" + server1port + "/ledsoff target='_blank'>http://" + WiFi.localIP().toString() + ":" + server1port + "/ledsoff</a><br>");
              client.println("<label>" + txtREST5 + ": </label>");
              client.println("<a href=http://" + WiFi.localIP().toString() + ":" + server1port + "/ledson target='_blank'>http://" + WiFi.localIP().toString() + ":" + server1port + "/ledson</a><br>");
              client.println("<label>" + txtREST6 + ": </label>");
              client.println("<a href=http://" + WiFi.localIP().toString() + ":" + server1port + "/ledstatus target='_blank'>http://" + WiFi.localIP().toString() + ":" + server1port + "/ledstatus</a><br>");
            } else {
              client.println("><br><br><label>" + txtRESTX + "</label><br>");
            }
            client.print("<br><hr>");


            // Update function:
            // ################
            client.println("<h2>" + txtUpdate0 + ":</h2>");
            client.println("<fieldset>");
            client.println("<div>");
            client.println("<input type='radio' id='idupdate0' name='useupdate' value='0'");
            if (useupdate == 0) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='idupdate0'>" + txtUpdateE1 + "</label>");
            client.println("</div>");
            client.println("<div>");
            client.println("<input type='radio' id='idupdate1' name='useupdate' value='1'");
            if (useupdate == 1) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='idupdate1'>" + txtUpdateE2 + "</label>");
            client.println("</div>");
            client.println("<div>");
            client.println("<input type='radio' id='idupdate2' name='useupdate' value='2'");
            if (useupdate == 2) {
              client.print(" checked");
              client.print(">");
            } else {
              client.print(">");
            }
            client.println("<label for='idupdate2'>" + txtUpdateE3 + "</label>");
            client.println("</div>");
            client.println("</fieldset>");

            // Update details section:
            if (useupdate == -1) useupdate = 1;  // Fix for setting from older versions
            if (useupdate == 0) {
              client.println("<br><br><label>" + txtUpdateX + "</label>");
            }
            if (useupdate == 1) {
              client.println("<br><br><label>" + txtUpdate2 + ":</label><br><br>");
              client.println("<a href=" + UpdatePath + " target='_blank'>" + UpdatePath + "</a><br><br>");
              client.println("<a href=" + UpdatePathIP + " target='_blank'>" + UpdatePathIP + "</a><br><br>");
              client.println("<label>" + txtUpdate3 + "<br>" + txtUpdate4 + "</label>");
              client.println("<br><br><label>" + txtUpdate5 + ":</label>");
              client.println("<a href='https://github.com/N1cls/Wordclock' target='_blank'>" + txtUpdate6 + "</a>");
            }
            if (useupdate == 2) {
              if (AvailableVersion == "-") readhttpfile();  // Read version from internet if function used for the first time...
              if (UpdateAvailable == 0) {
                client.println("<br><br><label>" + txtUpdate7 + ": " + AvailableVersion + "</label>");
              }
              if (UpdateAvailable == 1) {
                client.println("<br><br><label><b>" + txtUpdate8 + ": " + AvailableVersion + "</b></label><br><br>");
                if (useresturl == -1) {
                  client.println("<label>" + txtUpdate9 + ": <a href= http://" + WiFi.localIP().toString() + ":" + server1port + "/esphttpupdate target='_blank'>" + txtUpdate0 + "</a></label>");
                } else {
                  client.println("<label>" + txtRESTX + " - " + txtUpdateX + "</label>");
                }
              }
            }
            client.println("<br><br><hr>");


            // Reset WiFi configuration:
            // #########################
            client.println("<h2>" + txtWiFi0 + ":</h2><br>");
            if (useresturl) {
              client.println("<label>" + txtWiFi1 + "</label><br>");
              client.println("<br><a href= http://" + WiFi.localIP().toString() + ":" + server1port + "/clockwifireset target='_blank'>" + txtWiFi0 + "</a><br>");
              client.println("<br>! " + txtWiFi2 + " !<br><br><hr>");
            } else {
              client.println("<label>" + txtRESTX + "</label><br><hr>");
            }


            // WordClock restart:
            // ##################
            client.println("<h2>" + txtRestart0 + ":</h2><br>");
            if (useresturl) {
              client.println("<label>" + txtRestart1 + "</label><br>");
              client.println("<br><a href= http://" + WiFi.localIP().toString() + ":" + server1port + "/clockrestart target='_blank'>" + txtRestart0 + "</a><br>");
              client.println("<br>! " + txtRestart2 + " !<br><br><hr>");
            } else {
              client.println("<label>" + txtRESTX + "</label><br><hr>");
            }


            // Timezone and NTP:
            // #################
            client.println("<h2>" + txtTZNTP0 + ":</h2><br>");
            client.println("<label for=\"ntpserver\"></label>");
            client.print("<input type=\"text\" id=\"ntpserver\" name=\"ntpserver\" size=\"45\" value=\"");
            client.print(ntpServer);
            client.println("\"><br><br>");
            client.print("<input type=\"text\" id=\"timezone\" name=\"timezone\" size=\"45\" value=\"");
            client.print(timeZone);
            client.println("\"><br><br>");
            client.print("<br>" + txtTZNTP1 + ":<br><br>");
            client.print(DEFAULT_NTP_SERVER);
            client.println("<br><br>");
            client.print(DEFAULT_TIMEZONE);
            client.print("<br><br><br><a href=\"");
            client.println(TZ_WEB_SITE);
            client.println("\" target=\"_blank\">" + txtTZNTP2 + "</a><br><br><hr><br>");


            // Save settings button:
            // #####################
            client.println("<br><br><input type='submit' value='" + txtSaveSettings + "'>");
            client.print("<br><br><br><hr><br>");
            client.println("</form>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else {  // if you got a newline, then clear currentLine
            if (currentLine.startsWith("GET /setWC.php?")) {
              // Serial.print("Current request:  ");
              // Serial.println(currentLine);


              // Check for color settings:
              // #########################
              int pos = currentLine.indexOf("favcolor=%23");
              if (pos >= 0) {
                char* succ;
                String newColStr = currentLine.substring(pos + 12, pos + 18);
                String newRed = newColStr.substring(0, 2);
                redVal = strtol(newRed.begin(), &succ, 16);
                String newGreen = newColStr.substring(2, 4);
                greenVal = strtol(newGreen.begin(), &succ, 16);
                String newBlue = newColStr.substring(4, 6);
                blueVal = strtol(newBlue.begin(), &succ, 16);
              }


              // Check for blink time:
              // #####################
              if (currentLine.indexOf("&blinktime=on&") >= 0) {
                blinkTime = -1;
              } else {
                blinkTime = 0;
              }


              // Check for date display:
              // #######################
              if (currentLine.indexOf("&showdate=on&") >= 0) {
                showDate = -1;
              } else {
                showDate = 0;
              }


              // Check for power supply note:
              // ############################
              if (currentLine.indexOf("&powersupply=on&") >= 0) {
                powersupply = 1;
              } else {
                powersupply = 0;
              }


              // Check for DisplayOff switch:
              // ############################
              if (currentLine.indexOf("&displayoff=on&") >= 0) {
                displayoff = -1;
              } else {
                displayoff = 0;
              }


              // Check for useNightLEDs switch:
              // ##############################
              if (currentLine.indexOf("&useNightLEDs=on&") >= 0) {
                useNightLEDs = -1;
              } else {
                useNightLEDs = 0;
              }


              // Pick up Display OFF - Monday:
              // ############################
              pos = currentLine.indexOf("&displayonmaxMO=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                displayonmaxMO = maxStr.toInt();
              }
              pos = currentLine.indexOf("&displayonminMO=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                displayonminMO = minStr.toInt();
              }


              // Pick up Display OFF Max - Tuesday:
              // #################################
              pos = currentLine.indexOf("&displayonmaxTU=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                displayonmaxTU = maxStr.toInt();
              }
              pos = currentLine.indexOf("&displayonminTU=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                displayonminTU = minStr.toInt();
              }


              // Pick up Display OFF - Wednesday:
              // ################################
              pos = currentLine.indexOf("&displayonmaxWE=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                displayonmaxWE = maxStr.toInt();
              }
              pos = currentLine.indexOf("&displayonminWE=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                displayonminWE = minStr.toInt();
              }


              // Pick up Display OFF - Thursday:
              // ###############################
              pos = currentLine.indexOf("&displayonmaxTH=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                displayonmaxTH = maxStr.toInt();
              }
              pos = currentLine.indexOf("&displayonminTH=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                displayonminTH = minStr.toInt();
              }


              // Pick up Display OFF - Friday:
              // #############################
              pos = currentLine.indexOf("&displayonmaxFR=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                displayonmaxFR = maxStr.toInt();
              }
              pos = currentLine.indexOf("&displayonminFR=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                displayonminFR = minStr.toInt();
              }


              // Pick up Display OFF - Saturday:
              // ###############################
              pos = currentLine.indexOf("&displayonmaxSA=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                displayonmaxSA = maxStr.toInt();
              }
              pos = currentLine.indexOf("&displayonminSA=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                displayonminSA = minStr.toInt();
              }


              // Pick up Display OFF - Sunday:
              // #############################
              pos = currentLine.indexOf("&displayonmaxSU=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 16);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                displayonmaxSU = maxStr.toInt();
              }
              pos = currentLine.indexOf("&displayonminSU=");
              if (pos >= 0) {
                String minStr = currentLine.substring(pos + 16);
                pos = minStr.indexOf("&");
                if (pos > 0)
                  minStr = minStr.substring(0, pos);
                displayonminSU = minStr.toInt();
              }


              // Pick up WordClock HostName:
              // ###########################
              pos = currentLine.indexOf("&wchostnamenum=");
              if (pos >= 0) {
                String hostStr = currentLine.substring(pos + 15);
                pos = hostStr.indexOf("&");
                if (pos > 0)
                  hostStr = hostStr.substring(0, pos);
                wchostnamenum = hostStr.toInt();
              }


              // Check for UseUpdate switch:
              // ###########################
              pos = currentLine.indexOf("&useupdate=");
              if (pos >= 0) {
                String updateStr = currentLine.substring(pos + 11);
                pos = updateStr.indexOf("&");
                if (pos > 0)
                  updateStr = updateStr.substring(0, pos);
                useupdate = updateStr.toInt();
              }
              if (useupdate == 1) {
                MDNS.begin(wchostname + wchostnamenum);
                httpUpdater.setup(&httpServer);
                httpServer.begin();
                MDNS.addService("http", "tcp", 2022);
                MDNS.addService("http", "tcp", 80);
                UpdatePath = "http://" + String(wchostname + wchostnamenum) + ".local:2022/update";
                UpdatePathIP = "http://" + WiFi.localIP().toString() + ":2022/update";
              } else {
                httpUpdater.setup(&httpServer);
                httpServer.stop();
              }


              // Check for REST switch:
              // ######################
              if (currentLine.indexOf("&useresturl=on&") >= 0) {
                useresturl = -1;
              } else {
                useresturl = 0;
              }


              // Check for Use LED test switch:
              // ##############################
              if (currentLine.indexOf("&useledtest=on&") >= 0) {
                useledtest = -1;
              } else {
                useledtest = 0;
              }


              // Check for Use SET WLAN switch:
              // ##############################
              if (currentLine.indexOf("&usesetwlan=on&") >= 0) {
                usesetwlan = -1;
              } else {
                usesetwlan = 0;
              }


              // Check for Use SHOW IP switch:
              // #############################
              if (currentLine.indexOf("&useshowip=on&") >= 0) {
                useshowip = -1;
              } else {
                useshowip = 0;
              }


              // Check for RainBox switch:
              // #########################
              pos = currentLine.indexOf("&switchRainBow=");
              if (pos >= 0) {
                String rainbowStr = currentLine.substring(pos + 15);
                pos = rainbowStr.indexOf("&");
                if (pos > 0)
                  rainbowStr = rainbowStr.substring(0, pos);
                switchRainBow = rainbowStr.toInt();
              }


              // Check for language setting:
              // ###########################
              pos = currentLine.indexOf("&switchLangWeb=");
              if (pos >= 0) {
                String LangWebStr = currentLine.substring(pos + 15);
                pos = LangWebStr.indexOf("&");
                if (pos > 0)
                  LangWebStr = LangWebStr.substring(0, pos);
                switchLangWeb = LangWebStr.toInt();
                setLanguage(switchLangWeb);
              }


              // DE special parameter VIERTEL VOR vs. DREIVIERTEL selection:
              // ###########################################################
              pos = currentLine.indexOf("&DEspecial1=");
              if (pos >= 0) {
                String DEspecial = currentLine.substring(pos + 12);
                pos = DEspecial.indexOf("&");
                if (pos > 0)
                  DEspecial = DEspecial.substring(0, pos);
                DEspecial1 = DEspecial.toInt();
                // Serial.print("DEspecial1: ");
                // Serial.println(DEspecial1);
              }


              // Check for Minute LEDs order switch:
              // ###################################
              if (currentLine.indexOf("&switchLEDOrder=on&") >= 0) {
                switchLEDOrder = -1;
              } else {
                switchLEDOrder = 0;
              }


              // Check for DCW flag:
              // ###################
              if (currentLine.indexOf("DCW=ON") >= 0) {
                dcwFlag = -1;
              } else if (currentLine.indexOf("DCW=OFF") >= 0) {
                dcwFlag = 0;
              }


              // Get intensity DAY:
              // ##################
              pos = currentLine.indexOf("&intensity=");
              if (pos >= 0) {
                String intStr = currentLine.substring(pos + 11, pos + 14);
                pos = intStr.indexOf("&");
                if (pos > 0)
                  intStr = intStr.substring(0, pos);
                intensity = intStr.toInt();
              }


              // Get intensity NIGHT:
              // ####################
              pos = currentLine.indexOf("&intensityNight=");
              if (pos >= 0) {
                String intStr = currentLine.substring(pos + 16, pos + 19);
                pos = intStr.indexOf("&");
                if (pos > 0)
                  intStr = intStr.substring(0, pos);
                intensityNight = intStr.toInt();
              }


              // Use PING function:
              // ##################
              if (currentLine.indexOf("&PING_USEMONITOR=on&") >= 0) {
                PING_USEMONITOR = 1;
              } else {
                PING_USEMONITOR = 0;
              }


              // Use PING DEBUG function:
              // ########################
              if (currentLine.indexOf("&PING_DEBUG_MODE=on&") >= 0) {
                PING_DEBUG_MODE = 1;
              } else {
                PING_DEBUG_MODE = 0;
              }


              // 1st IP-address octet 1:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR1_O1=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR1_O1 = maxStr.toInt();
              }


              // 1st IP-address octet 2:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR1_O2=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR1_O2 = maxStr.toInt();
              }


              // 1st IP-address octet 3:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR1_O3=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR1_O3 = maxStr.toInt();
              }


              // 1st IP-address octet 4:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR1_O4=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR1_O4 = maxStr.toInt();
              }


              // 2nd IP-address octet 1:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR2_O1=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR2_O1 = maxStr.toInt();
              }


              // 2nd IP-address octet 2:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR2_O2=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR2_O2 = maxStr.toInt();
              }


              // 2nd IP-address octet 3:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR2_O3=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR2_O3 = maxStr.toInt();
              }


              // 2nd IP-address octet 4:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR2_O4=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR2_O4 = maxStr.toInt();
              }


              // 3rd IP-address octet 1:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR3_O1=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR3_O1 = maxStr.toInt();
              }


              // 3rd IP-address octet 2:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR3_O2=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR3_O2 = maxStr.toInt();
              }


              // 3rd IP-address octet 3:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR3_O3=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR3_O3 = maxStr.toInt();
              }


              // 3rd IP-address octet 4:
              // ###################
              pos = currentLine.indexOf("&PING_IP_ADDR3_O4=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 18);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_IP_ADDR3_O4 = maxStr.toInt();
              }


              // Max PING attempts:
              // ##################
              pos = currentLine.indexOf("&PING_TIMEOUTNUM=");
              if (pos >= 0) {
                String maxStr = currentLine.substring(pos + 17);
                pos = maxStr.indexOf("&");
                if (pos > 0)
                  maxStr = maxStr.substring(0, pos);
                PING_TIMEOUTNUM = maxStr.toInt();
              }


              // Get NTP server:
              // ###############
              pos = currentLine.indexOf("&ntpserver=");
              if (pos >= 0) {
                String ntpStr = currentLine.substring(pos + 11);
                pos = ntpStr.indexOf("&");  // "&" !!!
                if (pos > 0)
                  ntpStr = ntpStr.substring(0, pos);
                ntpServer = ntpStr;
              }


              // Pick up TimeZone definition:
              // ############################
              pos = currentLine.indexOf("timezone=");
              if (pos >= 0) {
                String tz = currentLine.substring(pos + 9);
                // check for unwanted suffix
                pos = tz.indexOf(" ");  // " " !!!
                if (pos > 0) {
                  tz = tz.substring(0, pos);
                }
                timeZone = urldecode(tz);
              }


              // Save data to EEPROM:
              // ####################
              writeEEPROM();    // save DATA to EEPROM
              configNTPTime();  // Reset NTP
            }
            currentLine = "";  // Clear the current command line
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    header = "";    // Clear the header variable
    client.stop();  // Close the connection
    //Serial.println("Web client disconnected.");
    //Serial.println("######################################################################################################");
  }
}


// ###########################################################################################################################################
// # Read current date & time from RTC:
// ###########################################################################################################################################
void rtcReadTime() {
  if (checkRTC() && useRTC == 1) {
    DateTime now = rtc.now();
    int oldHour = iHour;
    iYear = (int)(now.year());
    iMonth = (int)(now.month());
    iDay = (int)(now.day());
    iHour = (int)(now.hour());
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
void showIP() {
  IPAddress ip = WiFi.localIP();
  Serial.print("Displaying IP address ");
  Serial.println(ip);

  for (int x = 11; x > -75; x--) {
    dunkel();
    int offSet = x;
    // Loop over 4 bytes of IP Address
    for (int idx = 0; idx < 4; idx++) {
      uint8_t octet = ip[idx];
      printAt(octet / 100, offSet, 2);
      octet = octet % 100;
      offSet = offSet + 6;
      printAt(octet / 10, offSet, 2);
      offSet = offSet + 6;
      printAt(octet % 10, offSet, 2);
      offSet = offSet + 6;
      // add delimiter between octets
      if (idx < 3) {
        setLED(ledXY(offSet, 2), ledXY(offSet, 2), -1);  //sets point
        offSet++;
      }
    }
    pixels.show();
    delay(150);  // set speed of timeshift
  }
}


// ###########################################################################################################################################
// #  Switch off all LEDs:
// ###########################################################################################################################################
void dunkel() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));  // Switch off all LEDs
  }
}


// ###########################################################################################################################################
// # Switch on default Text "ES IST" / "IT IS": (DE or EN depending on the set web configuration language)
// ###########################################################################################################################################
void defaultText() {
  switch (switchLangWeb) {
    case 0:  // DE
      {
        pixels.setPixelColor(5, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(6, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(7, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(9, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(10, pixels.Color(redVal, greenVal, blueVal));  // set on default text
        break;
      }
    case 1:  // EN
      {
        // pixels.setPixelColor(5, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(6, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(7, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(9, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(10, pixels.Color(redVal, greenVal, blueVal));  // set on default text
        break;
      }
    default:  // DE
      {
        pixels.setPixelColor(5, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(6, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(7, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(9, pixels.Color(redVal, greenVal, blueVal));   // set on default text
        pixels.setPixelColor(10, pixels.Color(redVal, greenVal, blueVal));  // set on default text
        break;
      }
  }
}


// ###########################################################################################################################################
// # Convert x/y coordinates into LED number return -1 for invalid coordinate:
// ###########################################################################################################################################
int ledXY(int x, int y) {
  // Test for valid coordinates
  // If outside panel return -1
  if ((x < 0) || (x > 10) || (y < 0) || (y > 9))
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
void printAt(int ziffer, int x, int y) {
  switch (ziffer) {
    case 0:                                 //number 0
      setLEDLine(x + 1, x + 3, y + 6, -1);  //-1 is true, so switchOn
      for (int yd = 1; yd < 6; yd++) {
        setLED(ledXY(x, y + yd), ledXY(x, y + yd), -1);
        setLED(ledXY(x + 4, y + yd), ledXY(x + 4, y + yd), -1);
      }
      setLEDLine(x + 1, x + 3, y, -1);
      break;

    case 1:  //number 1
      setLED(ledXY(x + 3, y + 5), ledXY(x + 3, y + 5), -1);
      setLED(ledXY(x + 2, y + 4), ledXY(x + 2, y + 4), -1);
      for (int yd = 0; yd <= 6; yd++) {
        setLED(ledXY(x + 4, y + yd), ledXY(x + 4, y + yd), -1);
      }
      break;

    case 2:  //number 2
      for (int d = 1; d <= 4; d++) {
        setLED(ledXY(x + d, y + d), ledXY(x + d, y + d), -1);
      }
      setLEDLine(x, x + 4, y, -1);
      setLED(ledXY(x, y + 5), ledXY(x, y + 5), -1);
      setLED(ledXY(x + 4, y + 5), ledXY(x + 4, y + 5), -1);
      setLEDLine(x + 1, x + 3, y + 6, -1);
      break;

    case 3:  //number 3
      for (int yd = 1; yd <= 2; yd++) {
        setLED(ledXY(x + 4, y + yd + 3), ledXY(x + 4, y + yd + 3), -1);
        setLED(ledXY(x + 4, y + yd), ledXY(x + 4, y + yd), -1);
      }
      for (int yd = 0; yd < 7; yd = yd + 3) {
        setLEDLine(x + 1, x + 3, y + yd, -1);
      }
      setLED(ledXY(x, y + 1), ledXY(x, y + 1), -1);
      setLED(ledXY(x, y + 5), ledXY(x, y + 5), -1);
      break;

    case 4:  //number 4
      for (int d = 0; d <= 3; d++) {
        setLED(ledXY(x + d, y + d + 3), ledXY(x + d, y + d + 3), -1);
      }
      for (int yd = 0; yd <= 3; yd++) {
        setLED(ledXY(x + 3, y + yd), ledXY(x + 3, y + yd), -1);
      }
      setLEDLine(x, x + 4, y + 2, -1);
      break;

    case 5:  //number 5
      setLEDLine(x, x + 4, y + 6, -1);
      setLED(ledXY(x, y + 5), ledXY(x, y + 5), -1);
      setLED(ledXY(x, y + 4), ledXY(x, y + 4), -1);
      setLEDLine(x, x + 3, y + 3, -1);
      setLED(ledXY(x + 4, y + 2), ledXY(x + 4, y + 2), -1);
      setLED(ledXY(x + 4, y + 1), ledXY(x + 4, y + 1), -1);
      setLEDLine(x, x + 3, y, -1);
      break;

    case 6:  //number 6
      for (int d = 0; d <= 3; d++) {
        setLED(ledXY(x + d, y + d + 3), ledXY(x + d, y + d + 3), -1);
      }
      for (int yd = 0; yd < 4; yd = yd + 3) {
        setLEDLine(x + 1, x + 3, y + yd, -1);
      }
      setLED(ledXY(x, y + 1), ledXY(x, y + 1), -1);
      setLED(ledXY(x, y + 2), ledXY(x, y + 2), -1);
      setLED(ledXY(x + 4, y + 1), ledXY(x + 4, y + 1), -1);
      setLED(ledXY(x + 4, y + 2), ledXY(x + 4, y + 2), -1);
      break;

    case 7:  //number 7
      for (int yd = 0; yd <= 6; yd++) {
        setLED(ledXY(x + 3, y + yd), ledXY(x + 3, y + yd), -1);
      }
      setLEDLine(x + 1, x + 4, y + 3, -1);
      setLEDLine(x, x + 3, y + 6, -1);
      break;

    case 8:  //number 8
      for (int yd = 1; yd <= 2; yd++) {
        setLED(ledXY(x + 4, y + yd + 3), ledXY(x + 4, y + yd + 3), -1);
        setLED(ledXY(x + 4, y + yd), ledXY(x + 4, y + yd), -1);
        setLED(ledXY(x, y + yd + 3), ledXY(x, y + yd + 3), -1);
        setLED(ledXY(x, y + yd), ledXY(x, y + yd), -1);
      }
      for (int yd = 0; yd < 7; yd = yd + 3) {
        setLEDLine(x + 1, x + 3, y + yd, -1);
      }
      break;

    case 9:  //number 9
      for (int d = 0; d <= 3; d++) {
        setLED(ledXY(x + d + 1, y + d), ledXY(x + d + 1, y + d), -1);
      }
      for (int yd = 4; yd <= 5; yd++) {
        setLED(ledXY(x, y + yd), ledXY(x, y + yd), -1);
        setLED(ledXY(x + 4, y + yd), ledXY(x + 4, y + yd), -1);
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
    if (switchLEDOrder) {  // clockwise
      switch (i) {
        case 1: ledNr = 110; break;
        case 2: ledNr = 111; break;
        case 3: ledNr = 112; break;
        case 4: ledNr = 113; break;
      }
    } else {  // anti clockwise
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
void showCurrentDate() {
  for (int x = 11; x > -50; x--) {
    dunkel();
    printAt(iDay / 10, x, 2);
    printAt(iDay % 10, x + 6, 2);
    setLED(ledXY(x + 11, 2), ledXY(x + 11, 2), -1);  //sets first point
    printAt(iMonth / 10, x + 13, 2);
    printAt(iMonth % 10, x + 19, 2);
    if (iYear < 1000)
      iYear = iYear + 2000;
    setLED(ledXY(x + 24, 2), ledXY(x + 24, 2), -1);  //sets second point
    printAt(iYear / 1000, x + 26, 2);
    iYear = iYear % 1000;
    printAt(iYear / 100, x + 32, 2);
    iYear = iYear % 100;
    printAt(iYear / 10, x + 38, 2);
    printAt(iYear % 10, x + 44, 2);
    pixels.show();
    delay(150);  // set speed of timeshift
  }
}


// ###########################################################################################################################################
// # Display current time: (DE or EN depending on the set web configuration language)
// ###########################################################################################################################################
void showCurrentTime() {
  dunkel();       // switch off all LEDs
  defaultText();  // Switch on "ES IST" / "IT IS"

  // TEST TIMES:
  // iHour = 9;
  // iMinute = 15;

  // divide minute by 5 to get value for display control
  int minDiv = iMinute / 5;

  switch (switchLangWeb) {
    case 0:  // DE
      {
        // Fuenf: (Minuten)
        setLED(0, 3, ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)));
        // Viertel:
        if (DEspecial1 == 0) setLED(22, 28, ((minDiv == 3) || (minDiv == 9)));
        if (DEspecial1 == 1) setLED(22, 28, ((minDiv == 3)));
        // DREIVIERTEL:
        if (DEspecial1 == 1) setLED(22, 32, ((minDiv == 9)));
        // Zehn: (Minuten)
        setLED(11, 14, ((minDiv == 2) || (minDiv == 10)));
        // Zwanzig:
        setLED(15, 21, ((minDiv == 4) || (minDiv == 8)));
        // Nach:
        if (DEspecial1 == 0) setLED(40, 43, ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 7)));
        if (DEspecial1 == 1) {
          setLED(40, 43, ((minDiv == 1) || (minDiv == 2) || (minDiv == 4) || (minDiv == 7)));
          if (minDiv == 3) iHour = iHour + 1;
        }
        // Vor:
        if (DEspecial1 == 0) setLED(33, 35, ((minDiv == 5) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)));
        if (DEspecial1 == 1) setLED(33, 35, ((minDiv == 5) || (minDiv == 8) || (minDiv == 10) || (minDiv == 11)));
        // Halb:
        setLED(51, 54, ((minDiv == 5) || (minDiv == 6) || (minDiv == 7)));
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

        // Uhr:
        setLED(107, 109, (iMinute < 5));
        // Ein:
        setLEDHour(55, 57, (xHour == 1));
        // EinS: (S in EINS) (just used if not point 1 o'clock)
        setLEDHour(58, 58, ((xHour == 1) && (iMinute > 4)));
        // Zwei:
        setLEDHour(62, 65, (xHour == 2));
        // Drei:
        setLEDHour(73, 76, (xHour == 3));
        // Vier:
        setLEDHour(66, 69, (xHour == 4));
        // Fuenf:
        setLEDHour(44, 47, (xHour == 5));
        // Sechs:
        setLEDHour(77, 81, (xHour == 6));
        // Sieben:
        setLEDHour(93, 98, (xHour == 7));
        // Acht:
        setLEDHour(84, 87, (xHour == 8));
        // Neun:
        setLEDHour(102, 105, (xHour == 9));
        // Zehn: (Stunden)
        setLEDHour(99, 102, (xHour == 10));
        // Elf:
        setLEDHour(47, 49, (xHour == 11));
        // Zwoelf:
        setLEDHour(88, 92, (xHour == 12));
        break;
      }
    case 1:  // EN
      {
        // FIVE: (Minutes)            // x:05 + x:25 + x:35 + x:55
        setLED(23, 26, ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)));
        // QUARTER:                   // x:15 + X:45
        setLED(13, 19, ((minDiv == 3) || (minDiv == 9)));
        // TEN: (Minutes)             // x:10 + x:50
        setLED(38, 40, ((minDiv == 2) || (minDiv == 10)));
        // TWENTY:                    // x:20 + x:25 + x:35 + x:40
        setLED(27, 32, ((minDiv == 4) || (minDiv == 5) || (minDiv == 7) || (minDiv == 8)));
        // PAST:                      // x:05 + x:10 + x:15 + x:20 + x:25 + x:30
        setLED(51, 54, ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 5) || (minDiv == 6)));
        // TO:                        // x:35 + x:40 + x:45 + x:50 + x:55
        setLED(42, 43, ((minDiv == 7) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)));
        // HALF:                      // x:30
        setLED(33, 36, ((minDiv == 6)));
        // A:                         // x:15 + X:45
        setLED(3, 3, ((minDiv == 3) || (minDiv == 9)));

        // CORNER-LEDs: 1 per minute
        showMinutes(iMinute);

        //set hour from 1 to 12 (at noon, or midnight)
        int xHour = (iHour % 12);
        if (xHour == 0)
          xHour = 12;
        // at minute 35 hour needs to be counted up:
        if (iMinute >= 35) {
          if (xHour == 12)
            xHour = 1;
          else
            xHour++;
        }

        // O'CLOCK:
        setLED(104, 109, (iMinute < 5));
        // ONE:
        setLEDHour(55, 57, (xHour == 1));
        // TWO:
        setLEDHour(66, 68, (xHour == 2));
        // THREE:
        setLEDHour(61, 65, (xHour == 3));
        // FOUR:
        setLEDHour(73, 76, (xHour == 4));
        // FIVE:
        setLEDHour(69, 72, (xHour == 5));
        // SIX:
        setLEDHour(58, 60, (xHour == 6));
        // SEVEN:
        setLEDHour(94, 98, (xHour == 7));
        // EIGHT:
        setLEDHour(77, 81, (xHour == 8));
        // NINE:
        setLEDHour(44, 47, (xHour == 9));
        // TEN: (Hour)
        setLEDHour(99, 101, (xHour == 10));
        // ELEVEN:
        setLEDHour(82, 87, (xHour == 11));
        // TWELVE:
        setLEDHour(88, 93, (xHour == 12));
        break;
      }
  }
}


// ###########################################################################################################################################
// # Converts decimal to binary signs:
// ###########################################################################################################################################
byte decToBcd(byte val) {
  return ((val / 10 * 16) + (val % 10));
}


// ###########################################################################################################################################
// # Function to write / set the clock:
// ###########################################################################################################################################
void rtcWriteTime(int jahr, int monat, int tag, int stunde, int minute, int sekunde) {
  if (checkRTC() && useRTC == 1) {
    // Serial.println("Wire.write()...");
    Wire.beginTransmission(RTC_I2C_ADDRESS);
    Wire.write(0);  //count 0 activates RTC module
    Wire.write(decToBcd(sekunde));
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(stunde));
    Wire.write(decToBcd(0));  // weekdays not respected
    Wire.write(decToBcd(tag));
    Wire.write(decToBcd(monat));
    Wire.write(decToBcd(jahr - 2000));
    Wire.endTransmission();
  }
}


// ###########################################################################################################################################
// # Handle the time from the NTP server and write it to the RTC:
// ###########################################################################################################################################
void handleTime() {
  int timedebug = 0;
  // Check, whether we are connected to WLAN
  if ((WiFi.status() == WL_CONNECTED)) {
    time_t now;
    time(&now);  // read the current time
    struct tm ti;
    localtime_r(&now, &ti);
    uint16_t ye = ti.tm_year + 1900;
    uint8_t mo = ti.tm_mon + 1;
    uint8_t da = ti.tm_mday;
    int ho = ti.tm_hour;
    int mi = ti.tm_min;
    int sec = ti.tm_sec;
    if (checkRTC() && useRTC == 1) {
      int i = ti.tm_year + ti.tm_mon + ti.tm_mday + ti.tm_hour;
      if (i != lastRequest) {
        if (timedebug == 1) Serial.print("Set RTC to current time: ");
        lastRequest = i;
        // check for update
        if (timedebug == 1) Serial.print("WITH RTC: ");
        iYear = ye;
        iMonth = mo;
        iDay = da;
        iHour = ho;
        iMinute = mi;
        iSecond = sec;
        if (checkRTC() && useRTC == 1) rtcWriteTime(ye, mo, da, ho, mi, sec);
      }
    } else {
      // NTP TIME WITHOUT RTC:
      if (timedebug == 1) Serial.print("WITHOUT RTC: ");
      iYear = ye;
      iMonth = mo;
      iDay = da;
      iHour = ho;
      iMinute = mi;
      iSecond = sec;
    }
    if (timedebug == 1) {
      Serial.print(ho);
      Serial.print(':');
      Serial.print(mi);
      Serial.print(':');
      Serial.print(sec);
      Serial.print(" ");
      Serial.print(ye);
      Serial.print('-');
      Serial.print(mo);
      Serial.print('-');
      Serial.println(da);
    }
  }
  if (checkRTC() && useRTC == 1) rtcReadTime();
}


// ###########################################################################################################################################
// # Every Hour blink orange DCW - if switched on:
// ###########################################################################################################################################
void showDCW() {
  if ((dcwFlag) && (iMinute == 0) && (iSecond < 10)) {
    if ((iSecond % 2) == 0) {
      pixels.setPixelColor(59, pixels.Color(255, 128, 0));
      pixels.setPixelColor(60, pixels.Color(255, 128, 0));
      pixels.setPixelColor(61, pixels.Color(255, 128, 0));
    } else {
      pixels.setPixelColor(59, pixels.Color(0, 0, 0));
      pixels.setPixelColor(60, pixels.Color(0, 0, 0));
      pixels.setPixelColor(61, pixels.Color(0, 0, 0));
    }
  }
}


// ###########################################################################################################################################
// # Display the time:
// ###########################################################################################################################################
void ShowTheTime() {
  if (iMinute == 30) {
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
    pixels.setBrightness(intensity);  // Day brightness
    ShowTheTime();
  } else {
    if (useNightLEDs == -1) {
      pixels.setBrightness(intensityNight);  // Night brightness
      ShowTheTime();
    } else {
      dunkel();
    }
  }
}


// ###########################################################################################################################################
// # NTP time function:
// ###########################################################################################################################################
void configNTPTime() {
  //Serial.print("Set time zone to ");
  //Serial.println(timeZone);
  //Serial.print("Set time server to ");
  //Serial.println(ntpServer);
  configTime(timeZone.begin(), ntpServer.begin());
}


// ###########################################################################################################################################
// # Convert hex digit to int value:
// ###########################################################################################################################################
unsigned char h2int(char c) {
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
  if (!rtcStarted && useRTC == 1) {
    if (rtc.begin()) {
      rtcStarted = -1;
      // start RTC Communication via Wire.h library
      // Serial.println("Start RTC communication");
      Wire.begin();
    } else {
      useRTC = 0;
      Serial.println("Couldn't find RTC! --> Permanently disable the RTC usage after saving the configuration page once.");
    }
  }
  return rtcStarted;
}


// ###########################################################################################################################################
// # Decode %xx values in String - comming from URL / HTTP:
// ###########################################################################################################################################
String urldecode(String str) {
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
// # REST command function: ROOT
// ###########################################################################################################################################
void handleRoot() {
  WiFiClient client = server.available();
  server1->send(200, "text/plain", "WordClock REST web server active");
  Serial.print("WordClock REST web server active on port: ");
  Serial.println(server1port);
  client.stop();
}


// ###########################################################################################################################################
// # REST command function: LED set to ON
// ###########################################################################################################################################
void ledsON() {
  WiFiClient client = server.available();
  server1->send(200, "text/plain", "WordClock LEDs set to ON");
  Serial.println("WordClock LEDs set to ON");
  pixels.setBrightness(intensity);
  RESTmanLEDsON = true;
  LEDsON = true;
  pixels.show();
  client.stop();
}


// ###########################################################################################################################################
// # REST command function: LED set to OFF
// ###########################################################################################################################################
void ledsOFF() {
  WiFiClient client = server.available();
  server1->send(200, "text/plain", "WordClock LEDs set to OFF");
  Serial.println("WordClock LEDs set to OFF");
  pixels.setBrightness(0);
  pixels.show();
  RESTmanLEDsON = false;
  LEDsON = false;
  pixels.show();
  client.stop();
}


// ###########################################################################################################################################
// # REST command function: LED status
// ###########################################################################################################################################
void LedStatus() {
  WiFiClient client = server.available();
  server1->send(200, "text/plain", String(LEDsON));
  Serial.print("LED status: ");
  Serial.println(LEDsON);
  client.stop();
}


// ###########################################################################################################################################
// # Startup LED test function:
// ###########################################################################################################################################
// LED test --> no blank display if WiFi was not set yet:
void DisplayTest() {
  if (useledtest) {
    Serial.println("Display Test...");
    pixels.setBrightness(64);

    for (int i = 0; i < NUMPIXELS; i++) {
      setLED(i, i, 0);
      pixels.show();
    }

    for (int i = 0; i < NUMPIXELS; i++) {
      setLED(i, i, 1);
      pixels.show();
      delay(50);
      pixels.setPixelColor(i, 0, 0, 0);
    }

    pixels.setPixelColor(NUMPIXELS, 0, 0, 0);
    pixels.show();
  }
}


// ###########################################################################################################################################
// # Startup LED test function:
// ###########################################################################################################################################
void SetWLAN() {
  if (usesetwlan) {
    switch (switchLangWeb) {
      case 0:
        {  // DE:
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
          break;
        }
      case 1:
        {  // EN:
          Serial.println("Show WIFI...");
          setLED(31, 31, 1);    // W
          setLED(25, 25, 1);    // I
          setLED(76, 76, 1);    // F
          setLED(71, 71, 1);    // I
          setLED(110, 110, 1);  // Corner 1
          setLED(111, 111, 1);  // Corner 2
          setLED(112, 112, 1);  // Corner 3
          setLED(113, 113, 1);  // Corner 4
          break;
        }
    }
    pixels.show();
  }
}

// ###########################################################################################################################################
// # Restart the clock:
// ###########################################################################################################################################
void ClockRestart() {
  WiFiClient client = server.available();
  server1->send(200, "text/plain", "WORDCLOCK WILL RESTART IN 3 SECONDS...");
  delay(1000);
  client.stop();
  dunkel();
  Serial.println("Show RESET before board reset...");
  switch (switchLangWeb) {
    case 0:  // DE:
      {
        setLED(30, 31, 1);  // RE
        setLED(58, 58, 1);  // S
        setLED(64, 64, 1);  // E
        setLED(87, 87, 1);  // T
        break;
      }
    case 1:  // EN:
      {
        setLED(16, 16, 1);  // R
        setLED(30, 30, 1);  // E
        setLED(37, 37, 1);  // S
        setLED(39, 39, 1);  // E
        setLED(42, 42, 1);  // T
        break;
      }
  }
  pixels.show();
  Serial.println("##########################################");
  Serial.println("# WORDCLOCK WILL RESTART IN 3 SECONDS... #");
  Serial.println("##########################################");
  delay(3000);
  ESP.restart();
}


// ###########################################################################################################################################
// # Reset the WiFi configuration:
// ###########################################################################################################################################
void ClockWifiReset() {
  WiFiClient client = server.available();
  server1->send(200, "text/plain", "WIFI SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE WIFI AGAIN...");
  delay(1000);
  client.stop();
  dunkel();
  switch (switchLangWeb) {
    case 0:
      {  // DE:
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
        break;
      }
    case 1:
      {  // EN:
        Serial.println("Show WIFI...");
        setLED(31, 31, 1);    // W
        setLED(25, 25, 1);    // I
        setLED(76, 76, 1);    // F
        setLED(71, 71, 1);    // I
        setLED(110, 110, 1);  // Corner 1
        setLED(111, 111, 1);  // Corner 2
        setLED(112, 112, 1);  // Corner 3
        setLED(113, 113, 1);  // Corner 4
        break;
      }
      pixels.show();
  }
  WiFi.disconnect(true);
  delay(1500);
  WiFiManager wifiManager;
  delay(1500);
  wifiManager.resetSettings();
  delay(1500);
  Serial.println("####################################################################################################");
  Serial.println("# WIFI SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE WIFI AGAIN... #");
  Serial.println("####################################################################################################");
  delay(3000);
  ESP.restart();
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void setLED(int ledNrFrom, int ledNrTo, int switchOn) {
  if (switchOn) {
    if (switchRainBow == 1) {  // RainBow effect active
      if (ledNrFrom > ledNrTo) {
        setLED(ledNrTo, ledNrFrom, switchOn);
      } else {
        for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
          for (int i = ledNrFrom; i <= ledNrTo; i++) {
            if ((i >= 0) && (i < NUMPIXELS)) {
              int pixelHue = firstPixelHue + (i * 65536L / NUMPIXELS);
              pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
            }
          }
        }
      }
    } else {
      if (ledNrFrom > ledNrTo) {
        setLED(ledNrTo, ledNrFrom, switchOn);  //sets LED numbers in correct order (because of the date programming below)
      } else {
        for (int i = ledNrFrom; i <= ledNrTo; i++) {
          if ((i >= 0) && (i < NUMPIXELS))
            pixels.setPixelColor(i, pixels.Color(redVal, greenVal, blueVal));
        }
      }
    }
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void setLEDHour(int ledNrFrom, int ledNrTo, int switchOn) {
  // Every Hour blink orange
  if ((blinkTime) && (switchOn) && (iMinute == 0) && (iSecond < 10)) {
    if ((iSecond % 2) == 1) {
      for (int i = ledNrFrom; i <= ledNrTo; i++) {
        if ((i >= 0) && (i < NUMPIXELS))
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
// # PING function... Turn off the LED by presence status of IP-addresses (of your smart phone) monitored by a PING request 2 times perminute:
// ###########################################################################################################################################
void PingIP() {
  if (iSecond == 45 || iSecond == 15) {
    IPAddress IP1(PING_IP_ADDR1_O1, PING_IP_ADDR1_O2, PING_IP_ADDR1_O3, PING_IP_ADDR1_O4);
    IPAddress IP2(PING_IP_ADDR2_O1, PING_IP_ADDR2_O2, PING_IP_ADDR2_O3, PING_IP_ADDR2_O4);
    IPAddress IP3(PING_IP_ADDR3_O1, PING_IP_ADDR3_O2, PING_IP_ADDR3_O3, PING_IP_ADDR3_O4);

    // IP 1:
    // #####
    if (PING_IP_ADDR1_O1 != 0) {
      if (PING_DEBUG_MODE == 1) {
        Serial.print("IP gets pinged now: " + String(iHour) + ":" + String(iMinute) + ":" + String(iSecond) + " - IP1 ");
        Serial.print(String(PING_IP_ADDR1_O1) + "." + String(PING_IP_ADDR1_O2) + "." + String(PING_IP_ADDR1_O3) + "." + String(PING_IP_ADDR1_O4));
        Serial.print(" --> ");
      }
      if (Ping.ping(IP1)) {
        if (PING_DEBUG_MODE == 1) Serial.print("online - remaining attempts = ");
        PING_ATTEMPTSIP1 = PING_TIMEOUTNUM;  // Reset to configured value
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTSIP1);
        PingStatusIP1 = true;
      } else {
        if (PING_ATTEMPTSIP1 >= 1) PING_ATTEMPTSIP1 = PING_ATTEMPTSIP1 - 1;
        if (PING_DEBUG_MODE == 1) Serial.print("OFFLINE - remaining attempts = ");
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTSIP1);
      }
      if (PING_ATTEMPTSIP1 == 0) {
        PingStatusIP1 = false;
      }
    } else {
      PingStatusIP1 = false;
    }

    // IP 2:
    // #####
    if (PING_IP_ADDR2_O1 != 0) {
      if (PING_DEBUG_MODE == 1) {
        Serial.print("IP gets pinged now: " + String(iHour) + ":" + String(iMinute) + ":" + String(iSecond) + " - IP2 ");
        Serial.print(String(PING_IP_ADDR2_O1) + "." + String(PING_IP_ADDR2_O2) + "." + String(PING_IP_ADDR2_O3) + "." + String(PING_IP_ADDR2_O4));
        Serial.print(" --> ");
      }
      if (Ping.ping(IP2)) {
        if (PING_DEBUG_MODE == 1) Serial.print("online - remaining attempts = ");
        PING_ATTEMPTSIP2 = PING_TIMEOUTNUM;  // Reset to configured value
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTSIP2);
        PingStatusIP2 = true;
      } else {
        if (PING_ATTEMPTSIP2 >= 1) PING_ATTEMPTSIP2 = PING_ATTEMPTSIP2 - 1;
        if (PING_DEBUG_MODE == 1) Serial.print("OFFLINE - remaining attempts = ");
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTSIP2);
      }
      if (PING_ATTEMPTSIP2 == 0) {
        PingStatusIP2 = false;
      }
    } else {
      PingStatusIP2 = false;
    }

    // IP 3:
    // #####
    if (PING_IP_ADDR3_O1 != 0) {
      if (PING_DEBUG_MODE == 1) {
        Serial.print("IP gets pinged now: " + String(iHour) + ":" + String(iMinute) + ":" + String(iSecond) + " - IP3 ");
        Serial.print(String(PING_IP_ADDR3_O1) + "." + String(PING_IP_ADDR3_O2) + "." + String(PING_IP_ADDR3_O3) + "." + String(PING_IP_ADDR3_O4));
        Serial.print(" --> ");
      }
      if (Ping.ping(IP3)) {
        if (PING_DEBUG_MODE == 1) Serial.print("online - remaining attempts = ");
        PING_ATTEMPTSIP3 = PING_TIMEOUTNUM;  // Reset to configured value
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTSIP3);
        PingStatusIP3 = true;
      } else {
        if (PING_ATTEMPTSIP3 >= 1) PING_ATTEMPTSIP3 = PING_ATTEMPTSIP3 - 1;
        if (PING_DEBUG_MODE == 1) Serial.print("OFFLINE - remaining attempts = ");
        if (PING_DEBUG_MODE == 1) Serial.println(PING_ATTEMPTSIP3);
      }
      if (PING_ATTEMPTSIP3 == 0) {
        PingStatusIP3 = false;
      }
    } else {
      PingStatusIP3 = false;
    }

    // PING status check:
    if (PingStatusIP1 == true || PingStatusIP2 == true || PingStatusIP3 == true) {
      pixels.setBrightness(intensity);
      if (RESTmanLEDsON == true) LEDsON = true;
    }
    if (PingStatusIP1 == false && PingStatusIP2 == false && PingStatusIP3 == false) {
      if (PING_DEBUG_MODE == 1) Serial.println("All clients offline --> LEDs = OFF until one of the IP-addresses can be reached again...");
      pixels.setBrightness(0);
      pixels.show();
      if (RESTmanLEDsON == true) LEDsON = false;
    }
  }
}


// ###########################################################################################################################################
// # Wifi Manager setup and reconnect function that runs once at startup and during the loop function of the ESP:
// ###########################################################################################################################################
void WIFI_login() {
  Serial.print("Try to connect to WiFi: ");
  Serial.println(WiFi.SSID());
  bool WiFires;
  bool DebugWifiLEDs = false;
  WiFiManager wifiManager;
  configNTPTime();                                 // Set timezone
  wifiManager.setConfigPortalTimeout(AP_TIMEOUT);  // Max wait for 3 minutes
  WiFires = wifiManager.autoConnect(DEFAULT_AP_NAME);
  if (!WiFires) {
    Serial.print("Failed to connect to WiFi: ");
    Serial.println(WiFi.SSID());
    if (DebugWifiLEDs) WIFI_DebugWifiLEDs(pixels.Color(255, 0, 0));
  } else {
    Serial.print("Connected to WiFi: ");
    Serial.println(WiFi.SSID());
    if (DebugWifiLEDs) WIFI_DebugWifiLEDs(pixels.Color(0, 255, 0));
  }
}


// ###########################################################################################################################################
// # LED debug for Wifi Manager setup and reconnect function that runs once at startup and during the loop function of the ESP:
// ###########################################################################################################################################
void WIFI_DebugWifiLEDs(uint32_t color) {
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, (0, 0, 0));
  }
  pixels.show();
  delay(1000);
  int myArray[] = { 110, 111, 112, 113 };
  for (int element : myArray) {
    pixels.setPixelColor(element, color);
  }
  pixels.show();
  delay(3000);
}


// ###########################################################################################################################################
// # Direct update for the ESP:
// ###########################################################################################################################################
void esphttpupdate() {  // Basic ESP update function
  if (useupdate != 2) {
    server1->send(200, "text/plain", "The automatic update function is currently disabled...");
    Serial.println("The automatic update function is currently disabled...");
  } else {
    if (UpdateAvailable == 1) {
      WiFiClient client = server.available();
      server1->send(200, "text/plain", "Update Start... Check status on the display...");
      Serial.println("Update START");
      ESPhttpUpdate.onStart(update_started);
      ESPhttpUpdate.onEnd(update_finished);
      ESPhttpUpdate.onProgress(update_progress);
      ESPhttpUpdate.onError(update_error);
      t_httpUpdate_return ret = ESPhttpUpdate.update(client, "http://www.awsw.de/Arduino/WordClock/WordClockWithWeb.ino.nodemcu.bin");  // Update path on AWSW-de's server
      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;
        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;
        case HTTP_UPDATE_OK:
          Serial.println("HTTP_UPDATE_OK");
          break;
      }
      Serial.println("Update END");
      client.stop();
    } else {
      server1->send(200, "text/plain", "No update available...");
      Serial.println("No update available...");
    }
  }
}

void update_started() {  // Callback update start function
  Serial.println("CALLBACK:  HTTP update process started");
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, (0, 0, 0));
  }
  pixels.show();
  switchRainBow = 0;
  pixels.setBrightness(50);
  redVal = 0;
  greenVal = 255;
  blueVal = 255;
  // Arrow borders:
  setLED(3, 7, 1);
  setLED(14, 14, 1);
  setLED(18, 18, 1);
  setLED(29, 29, 1);
  setLED(25, 25, 1);
  setLED(36, 36, 1);
  setLED(40, 40, 1);
  setLED(51, 51, 1);
  setLED(47, 47, 1);
  setLED(56, 58, 1);
  setLED(62, 64, 1);
  setLED(68, 68, 1);
  setLED(74, 74, 1);
  setLED(80, 80, 1);
  setLED(84, 84, 1);
  setLED(94, 94, 1);
  setLED(92, 92, 1);
  setLED(104, 104, 1);
  pixels.show();
  delay(1500);
}

void update_finished() {  // Callback update success finish function
  Serial.println("CALLBACK:  HTTP update process finished");
  switchRainBow = 0;
  pixels.setBrightness(50);
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, (0, 0, 0));
  }
  pixels.show();
  update_msg_LEDs(0, 255, 0);
  delay(3000);
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, (0, 0, 0));
  }
  pixels.show();
  Serial.println("Show RESET before board reset...");
  switch (switchLangWeb) {
    case 0:  // DE:
      {
        setLED(30, 31, 1);  // RE
        setLED(58, 58, 1);  // S
        setLED(64, 64, 1);  // E
        setLED(87, 87, 1);  // T
        break;
      }
    case 1:  // EN:
      {
        setLED(16, 16, 1);  // R
        setLED(30, 30, 1);  // E
        setLED(37, 37, 1);  // S
        setLED(39, 39, 1);  // E
        setLED(42, 42, 1);  // T
        break;
      }
  }
  pixels.show();
  Serial.println("##########################################");
  Serial.println("# WORDCLOCK WILL RESTART IN 3 SECONDS... #");
  Serial.println("##########################################");
  delay(3000);
}

void update_error(int err) {  // Callback update error finish function
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
  switchRainBow = 0;
  pixels.setBrightness(50);
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, (0, 0, 0));
  }
  pixels.show();
  update_msg_LEDs(255, 0, 0);
  delay(3000);
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, (0, 0, 0));
  }
  pixels.show();
  Serial.println("Show RESET before board reset...");
  switch (switchLangWeb) {
    case 0:  // DE:
      {
        setLED(30, 31, 1);  // RE
        setLED(58, 58, 1);  // S
        setLED(64, 64, 1);  // E
        setLED(87, 87, 1);  // T
        break;
      }
    case 1:  // EN:
      {
        setLED(16, 16, 1);  // R
        setLED(30, 30, 1);  // E
        setLED(37, 37, 1);  // S
        setLED(39, 39, 1);  // E
        setLED(42, 42, 1);  // T
        break;
      }
  }
  pixels.show();
  Serial.println("##########################################");
  Serial.println("# WORDCLOCK WILL RESTART IN 3 SECONDS... #");
  Serial.println("##########################################");
  delay(3000);
}

void update_msg_LEDs(int redCol, int greenCol, int blueCol) {
  redVal = redCol;
  greenVal = greenCol;
  blueVal = blueCol;
  setLED(1, 2, 1);  // 1 (Arrow)
  setLED(4, 6, 1);
  setLED(8, 8, 1);
  setLED(10, 10, 1);
  setLED(21, 21, 1);  // 2
  setLED(19, 19, 1);
  setLED(17, 17, 1);
  setLED(15, 15, 1);
  setLED(13, 13, 1);
  setLED(11, 11, 1);
  setLED(22, 22, 1);  // 3
  setLED(24, 24, 1);
  setLED(26, 28, 1);
  setLED(30, 30, 1);
  setLED(32, 32, 1);
  setLED(33, 35, 1);  // 4
  setLED(37, 37, 1);
  setLED(41, 42, 1);
  setLED(72, 74, 1);  // O (7-10)
  setLED(81, 81, 1);
  setLED(94, 94, 1);
  setLED(79, 79, 1);
  setLED(96, 96, 1);
  setLED(101, 103, 1);
  setLED(70, 70, 1);  // K (7-10)
  setLED(83, 83, 1);
  setLED(92, 92, 1);
  setLED(105, 105, 1);
  setLED(68, 68, 1);
  setLED(84, 84, 1);
  setLED(91, 91, 1);
  setLED(107, 107, 1);
  pixels.show();
}

void update_progress(int cur, int total) {  // Callback update download progress function
  switchRainBow = 0;
  pixels.setBrightness(50);
  // Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
  int percent = (cur * 100) / total;
  redVal = 0;
  greenVal = 255;
  blueVal = 0;
  if (percent >= 10) {
    setLED(3, 7, 1);
  }
  if (percent >= 20) {
    setLED(14, 18, 1);
  }
  if (percent >= 30) {
    setLED(25, 29, 1);
  }
  if (percent >= 40) {
    setLED(36, 40, 1);
  }
  if (percent >= 50) {
    setLED(47, 51, 1);
  }
  if (percent >= 60) {
    setLED(56, 64, 1);
  }
  if (percent >= 70) {
    setLED(68, 74, 1);
  }
  if (percent >= 80) {
    setLED(80, 84, 1);
  }
  if (percent >= 90) {
    setLED(92, 94, 1);
  }
  if (percent >= 99) {
    setLED(104, 104, 1);
  }
  if (percent == 100) {
    delay(1500);
  }
  pixels.show();
  Serial.print("Automatic update - download file progress: ");
  Serial.print(percent);
  Serial.println("%");
}


// ###########################################################################################################################################
// # Check for available updates:
// ###########################################################################################################################################
void readhttpfile() {
  //Serial.print("AUTOMATIC UPDATE: Read current version number from http server: ");
  WiFiClient wifiClient;
  String errorMessage = "";
  String response = "";
  String GetData = "http://www.awsw.de/Arduino/WordClock/version.txt";  // Update path on AWSW-de's server
  HTTPClient http;
  http.begin(wifiClient, GetData);
  int httpCode = http.GET();
  if (httpCode > 0) {
    response = http.getString();
    //Serial.println(response);
    http.end();
    AvailableVersion = String(response);
    if (String(response) != String(WORD_CLOCK_VERSION)) {
      UpdateAvailable = true;
      //Serial.print("Software update available: ");
      //Serial.println(UpdateAvailable);
    }
    if (httpCode != 200) {
      errorMessage = "Error response (" + String(httpCode) + "): " + response;
      Serial.println(errorMessage);
      return;
    }
  }
}


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################