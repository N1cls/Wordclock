// ###########################################################################################################################################
// #
// # WordClock code for the thingiverse WordClock project: https://www.thingiverse.com/thing:4693081 - settings file:
// #
// # Code by https://github.com/N1cls and https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0 https://github.com/N1cls/Wordclock/blob/master/LICENSE.md
// #
// # Please set the default settings here only:
// #
// # Compatible with WordClock version: V5.9.4
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
// # Hardware settings:
// ###########################################################################################################################################
int RTC_I2C_ADDRESS = 0x68;  // I2C adress of the RTC  DS3231 (Chip on ZS-042 Board)
#define AP_TIMEOUT 240       // Timeout in seconds for AP / WLAN config
#define PIN D6               // Arduino-Pin connected to the NeoPixels
#define NUMPIXELS 114        // How many NeoPixels are attached to the Arduino?


// ###########################################################################################################################################
// # LED defaults:
// ###########################################################################################################################################
int redVal = 0;           // Default color RED
int greenVal = 255;       // Default color GREEN
int blueVal = 0;          // Default color BLUE
int intensity = 42;       // LED intensity (0..255) in day mode   - Important note: Check power consumption and used power supply capabilities!
int intensityNight = 21;  // LED intensity (0..255) in night mode - Important note: Check power consumption and used power supply capabilities!


// ###########################################################################################################################################
// # Various default settings:
// ###########################################################################################################################################
int switchLangWeb = 0;    // Language default setting (0 = german, 1 = english, 2...x = for future use)
int wifireset = 0;        // WiFi Reset switch (not stored in EEPROM)
int clockreset = 0;       // WordClock Reset switch (not stored in EEPROM)
int useupdate = 1;        // Use the internal web update server
int useledtest = 1;       // Show start animation and display test at boot
int usesetwlan = 1;       // Show start animation and display test at boot
int useshowip = 1;        // Show the current ip at boot
int switchRainBow = 0;    // Show the display in rainbow mode (default color is then ignored)
int switchLEDOrder = 1;   // Show the minute LEDs in the 4 corners in clockwise order if set to 1
int showDate = 0;         // Show date when minute = 30 ? - Note: Any value <> 0 shows the date when minute = 30
int blinkTime = 0;        // Flag for blinking hour time name every hour
int dcwFlag = 0;          // Flag for highlighting DCW every Hour
int useresturl = 1;       // Use the REST function web server
int server1port = 55555;  // REST function web server port
int powersupply = 0;      // Flag for the power supply note text and to raise the limit for the intensity from 128 to 255
int useRTC = 1;           // Flag to disable the RTC board (No configuration needed. Set automatically after entering and saving the web config once)
int DEspecial1 = 0;       // DE special function only: Change between "VIERTEL VOR" = "0" and "DREIVIERTEL" = "1"


// ###########################################################################################################################################
// # Night mode option defaults:
// ###########################################################################################################################################
int displayoff = 0;       // Use the night mode function to set the LEDs to off
int useNightLEDs = 0;     // Show LEDs in night mode with reduced intensity
int displayonmaxMO = 22;  // Display off from 22:00 o'clock Monday
int displayonminMO = 6;   // Display on after 06:59 o'clock Monday
int displayonmaxTU = 22;  // Display off from 22:00 o'clock Tuesday
int displayonminTU = 6;   // Display on after 06:59 o'clock Tuesday
int displayonmaxWE = 22;  // Display off from 22:00 o'clock Wednesday
int displayonminWE = 6;   // Display on after 06:59 o'clock Wednesday
int displayonmaxTH = 22;  // Display off from 22:00 o'clock Thursday
int displayonminTH = 6;   // Display on after 06:59 o'clock Thursday
int displayonmaxFR = 23;  // Display off from 23:00 o'clock Friday
int displayonminFR = 6;   // Display on after 06:59 o'clock Friday
int displayonmaxSA = 23;  // Display off from 23:00 o'clock Saturday
int displayonminSA = 7;   // Display on after 06:59 o'clock Saturday
int displayonmaxSU = 22;  // Display off from 22:00 o'clock Sunday
int displayonminSU = 6;   // Display on after 06:59 o'clock Sunday


// ###########################################################################################################################################
// # Variables declaration:
// ###########################################################################################################################################
#define DEFAULT_AP_NAME "WordClock"  // WiFi access point name of the ESP8266
String wchostname = "WordClock";     // Hostname to be set in your router
int wchostnamenum = 0;               // Hostname + Number used as suffix to the host name
ESP8266WebServer httpServer(2022);   // Update server port


// ###########################################################################################################################################
// # Turn off the LED by presence status of an IP-address (of your smart phone) monitored by a PING request 2 times perminute:
// #
// # Notes:
// # - You might need to set the IP-address of the to monitor device to static in your router configuration
// # - If the IP-address is set to 0.0.0.0 it will be ignored and skipped during the PING requests
// ###########################################################################################################################################
int PING_IP_ADDR1_O1 = 192;  // 1st IP-address of the to monitor device - ip octet 1
int PING_IP_ADDR1_O2 = 168;  // 1st IP-address of the to monitor device - ip octet 2
int PING_IP_ADDR1_O3 = 178;  // 1st IP-address of the to monitor device - ip octet 3
int PING_IP_ADDR1_O4 = 56;   // 1st IP-address of the to monitor device - ip octet 4
int PING_IP_ADDR2_O1 = 0;    // 2nd IP-address of the to monitor device - ip octet 1
int PING_IP_ADDR2_O2 = 0;    // 2nd IP-address of the to monitor device - ip octet 2
int PING_IP_ADDR2_O3 = 0;    // 2nd IP-address of the to monitor device - ip octet 3
int PING_IP_ADDR2_O4 = 0;    // 2nd IP-address of the to monitor device - ip octet 4
int PING_IP_ADDR3_O1 = 0;    // 3rd IP-address of the to monitor device - ip octet 1
int PING_IP_ADDR3_O2 = 0;    // 3rd IP-address of the to monitor device - ip octet 2
int PING_IP_ADDR3_O3 = 0;    // 3rd IP-address of the to monitor device - ip octet 3
int PING_IP_ADDR3_O4 = 0;    // 3rd IP-address of the to monitor device - ip octet 4
int PING_TIMEOUTNUM = 10;    // After which amount of timed out PING request should the LED be turned off? Value 10 = 5 minutes
int PING_DEBUG_MODE = 0;     // Use the serial output monitor of Arduino IDE to see the current online status
int PING_USEMONITOR = 0;     // Switch flag to turn the PING function on (1) or off (0)


// ###########################################################################################################################################
// # NTP time server settings:
// ###########################################################################################################################################
#define DEFAULT_NTP_SERVER "de.pool.ntp.org"
#define DEFAULT_TIMEZONE "CET-1CEST,M3.5.0/02,M10.5.0/03"
#define TZ_WEB_SITE "http://www.hs-help.net/hshelp/gira/v4_7/de/proj_tz.html"


// ###########################################################################################################################################
// # Internal web server settings:
// ###########################################################################################################################################
WiFiServer server(80);  // Set web server port number to 80


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################
