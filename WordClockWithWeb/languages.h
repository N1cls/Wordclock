// ###########################################################################################################################################
// #
// # WordClock code for the thingiverse WordClock project: https://www.thingiverse.com/thing:4693081 - language file:
// #
// # Code by https://github.com/N1cls and https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0 https://github.com/N1cls/Wordclock/blob/master/LICENSE.md
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
// # Default texts in german language:
// ###########################################################################################################################################
// General texts:
String WordClockName, languageSelect, languageInt0, languageInt1, txtSaveSettings;
// LED settings:
String txtSettings, txtLEDsettings, txtLEDcolor, txtIntensityDay, txtIntensityNight, txtPowerSupplyNote1, txtPowerSupplyNote2, txtPowerSupplyNote3, txtPowerSupplyNote4;
String txtFlashFullHour1, txtFlashFullHour2, txtShowDate1, txtShowDate2, txtNightMode1, txtNightMode2, txtNightMode3, txtNightModeOff, txtNightModeTo, txtNightModeClock;
String txtMO, txtTU, txtWE, txtTH, txtFR, txtSA, txtSU;
// Content and startup:
String txtContentStartup, txtUseLEDtest, txtUSEsetWLAN, txtShowIP, txtRainbow1, txtRainbow2, txtRainbow3, txtRainbow4, txtMinDir1, txtMinDir2, txtMinDir3;
// PING monitor IP-adresses:
String txtPing0, txtPing1, txtPing2, txtPing3, txtPing4, txtPing5, txtPing6, txtPing7, txtPing8, txtPing9;
// Hostname:
String txtHostName1, txtHostName2;
// REST functions:
String txtREST0, txtREST1, txtREST2, txtREST3, txtREST4, txtREST5, txtREST6, txtREST7, txtRESTX;
// Update function:
String txtUpdate0, txtUpdateE1, txtUpdateE2, txtUpdateE3, txtUpdate2, txtUpdate3, txtUpdate4, txtUpdate5, txtUpdate6, txtUpdate7, txtUpdate8, txtUpdate9, txtUpdateX;
// WiFi:
String txtWiFi0, txtWiFi1, txtWiFi2;
// Restart
String txtRestart0, txtRestart1, txtRestart2;
// Time zone and NTP server:
String txtTZNTP0, txtTZNTP1, txtTZNTP2;
// DE special parameter VIERTEL VOR vs. DREIVIERTEL selection:
String DEspecial1Text1, DEspecial1Text2, DEspecial1Text3;


void setLanguage(int lang) {
  // ###########################################################################################################################################
  // # Translations for: DE
  // ###########################################################################################################################################
  if (lang == 0) {  // DEUTSCH
    // Allgemeine Texte:
    WordClockName = "WordClock";
    languageSelect = "Sprache für das WordClock Layout und die Web Konfiguration";
    languageInt0 = "Deutsch";
    languageInt1 = "Englisch";
    txtSaveSettings = "Einstellungen speichern";
    DEspecial1Text1 = "Schreibweise für x:15 und x:45";
    DEspecial1Text2 = "VIERTEL VOR / VIERTEL NACH";
    DEspecial1Text3 = "DREIVIERTEL / VIERTEL";

    // LED Einstellungen:
    txtSettings = "Einstellungen";
    txtLEDsettings = "LED Einstellungen";
    txtLEDcolor = "Farbe";
    txtIntensityDay = "Helligkeit am Tag";
    txtIntensityNight = "Helligkeit bei Nacht";
    txtPowerSupplyNote1 = "Wichtig: Beide Werte begrenzt auf 128 von maximal 255. Achte darauf ein geeignetes Netzteil zu verwenden!";
    txtPowerSupplyNote2 = "Je nach LED Anzahl, selektierter Farbe und Helligkeit wird mindestens ein 5V/3A Netzteil empfohlen!";
    txtPowerSupplyNote3 = "Den Hinweis zum Netzteil akzeptiere und beachte ich. Ich möchte die Werte wieder auf maximal 255 einstellen können";
    txtPowerSupplyNote4 = "Den wichtigen Hinweis zum Netzteil nicht mehr anzeigen";
    txtFlashFullHour1 = "Volle Stunde blinken";
    txtFlashFullHour2 = "Stundenangabe soll zur vollen Stunde blinken?";
    txtShowDate1 = "Datumsanzeige als Lauftext";
    txtShowDate2 = "Alle 30 Minuten anzeigen?";
    txtNightMode1 = "Display abschalten oder dunkler schalten?";
    txtNightMode2 = "Display komplett abschalten ...";
    txtNightMode3 = "... oder nur dunkler schalten auf Wert der Helligkeit bei Nacht?";
    txtNightModeOff = "Display aus ab";
    txtNightModeTo = "bis";
    txtNightModeClock = "Uhr";
    txtMO = "Montag";
    txtTU = "Dienstag";
    txtWE = "Mittwoch";
    txtTH = "Donnerstag";
    txtFR = "Freitag";
    txtSA = "Samstag";
    txtSU = "Sonntag";

    // Anzeigen und Startverhalten:
    txtContentStartup = "Anzeigen und Startverhalten";
    txtUseLEDtest = "LED Start Test anzeigen?";
    txtUSEsetWLAN = "SET WLAN beim Start anzeigen?";
    txtShowIP = "IP-Addresse beim Start anzeigen?";
    txtRainbow1 = "Wähle den Regenbogen Farbeffekt Modus";
    txtRainbow2 = "Aus";
    txtRainbow3 = "Variante 1 (Worte verschieden bunt)";
    txtRainbow4 = "Variante 2 (Alle Worte zufällig bunt)";
    txtMinDir1 = "Minuten LEDs Ecken Reihenfolge im Uhrzeigersinn?";
    txtMinDir2 = "Wenn diese Option gesetzt wird, werden die Minuten-LEDs in den 4 Ecken";
    txtMinDir3 = "im Uhrzeigersinn angezeigt, ansonsten entgegen dem Uhrzeigersinn.";

    // PING Monitor IP-Adressen:
    txtPing0 = "PING Monitor für IP-Adresse(n) -> LEDs abschalten wenn IP(s) länger offline";
    txtPing1 = "PING Monitor Funktion verwenden?";
    txtPing2 = "Bitte hier die zu überwachende(n) IP-Adresse(n) eintragen";
    txtPing3 = "1. IP-Adresse";
    txtPing4 = "2. IP-Adresse";
    txtPing5 = "3. IP-Adresse";
    txtPing6 = "Hinweis: Eine IP-Addresse mit dem Wert 0.0.0.0 wird in der Abfrage übersprungen.";
    txtPing7 = "Anzahl PING Versuche bis die LEDs abgeschaltet werden";
    txtPing8 = "Hinweis: Anzahl = 10 bedeutet einen 5 Minuten Timeout, da 2 PING Versuche pro Minute erfolgen.";
    txtPing9 = "DEBUG PING Monitor Funktion verwenden?";

    // Hostname:
    txtHostName1 = "WordClock Hostname anpassen";
    txtHostName2 = "Hostname";

    // REST Funktionen:
    txtREST0 = "REST Funktionen";
    txtREST1 = "Über die folgenden Links können Funktionen der WordClock von Außen gesteuert werden.";
    txtREST2 = "REST Funktion verwenden?";
    txtREST3 = "Über einen der folgenden Links kann die WordClock manuell über den Browser ab und an geschaltet werden";
    txtREST4 = "LEDs ausschalten";
    txtREST5 = "LEDs einschalten";
    txtREST6 = "LED Status";
    txtRESTX = "Die REST Funktion ist aktuell deaktiviert.";

    // Update Funktion:
    txtUpdate0 = "Update";
    txtUpdateE1 = "Update Funktion nicht verwenden";
    txtUpdateE2 = "Lokale Update Funktion verwenden";
    txtUpdateE3 = "Automatische Update Funktion via Internet verwenden";
    txtUpdate2 = "Über einen der folgenden Links kann die WordClock über den Browser ohne Arduino IDE aktualisiert werden";
    txtUpdate3 = "Hinweis: Es wird eine in der Arduino IDE mit Strg+Alt+S zuvor erstellte .BIN Datei des Sketches benötigt,";
    txtUpdate4 = "die über die Option 'Update Firmware' hochgeladen werden kann.";
    txtUpdate5 = "Die notwendige Update-Datei kann hier heruntergeladen werden";
    txtUpdate6 = "Wordclock Repository auf GitHub";
    txtUpdate7 = "Die installierte Version entspricht der aktuell verfügbaren Version";
    txtUpdate8 = "Ein Update ist verfügbar auf Version";
    txtUpdate9 = "Verwende den folgenden Link um das Update zu starten";
    txtUpdateX = "Die Update Funktion ist aktuell deaktiviert.";

    // WLAN:
    txtWiFi0 = "WLAN Einstellungen zurücksetzen";
    txtWiFi1 = "WLAN Einstellungen zurücksetzen und Uhr neu starten?";
    txtWiFi2 = "Wenn diese Option verwendet wird, werden die WLAN Einstellungen gelöscht";

    // Neustart:
    txtRestart0 = "WordClock neustarten";
    txtRestart1 = "WordClock neustarten?";
    txtRestart2 = "Wenn diese Option verwendet wird, wird die Uhr neu gestartet";

    // Zeitzone and NTP-Server:
    txtTZNTP0 = "Zeitzone & NTP-Server";
    txtTZNTP1 = "Standardwerte";
    txtTZNTP2 = "Erklärung zur Einstellung der Zeitzone";
  }


  // ###########################################################################################################################################
  // # Translations for: EN
  // ###########################################################################################################################################
  if (lang == 1) {  // ENGLISH
    // General texts:
    WordClockName = "WordClock";
    languageSelect = "Language for the WordClock layout and web configuration";
    languageInt0 = "German";
    languageInt1 = "English";
    txtSaveSettings = "Save settings";

    // LED settings:
    txtSettings = "settings";
    txtLEDsettings = "LED settings";
    txtLEDcolor = "Color";
    txtIntensityDay = "Intensity in day mode";
    txtIntensityNight = "Intensity in night mode";
    txtPowerSupplyNote1 = "Important: Both values limited to 128 of maximum 255. Take care to use a suitable power supply!";
    txtPowerSupplyNote2 = "Depending on the amount of LEDs, selected color and intensity a 5V/3A power supply is recommended!";
    txtPowerSupplyNote3 = "I accept and observe the note on the power supply unit. I would like to be able to set the values ​​back to a maximum of 255";
    txtPowerSupplyNote4 = "Do not show the important note about the power supply again";
    txtFlashFullHour1 = "Flash full hour";
    txtFlashFullHour2 = "Flash the hour value every new hour?";
    txtShowDate1 = "Show date as scolling text";
    txtShowDate2 = "Display the date every 30 minutes?";
    txtNightMode1 = "Switch off or darken the display?";
    txtNightMode2 = "Switch off the display completely ...";
    txtNightMode3 = "... or only switch it darker to the value of the intensity in night mode?";
    txtNightModeOff = "Turn display off from";
    txtNightModeTo = "to";
    txtNightModeClock = "o'clock";
    txtMO = "Monday";
    txtTU = "Tuesday";
    txtWE = "Wednesday";
    txtTH = "Thursday";
    txtFR = "Friday";
    txtSA = "Saturday";
    txtSU = "Sunday";

    // Content and startup:
    txtContentStartup = "Content and startup";
    txtUseLEDtest = "Run LED test on startup?";
    txtUSEsetWLAN = "Show WIFI text on startup?";
    txtShowIP = "Show IP-address on startup?";
    txtRainbow1 = "Choose the rainbow color effect mode";
    txtRainbow2 = "Off";
    txtRainbow3 = "Variant 1 (words in different colours)";
    txtRainbow4 = "Variant 2 (all words randomly colored)";
    txtMinDir1 = "Minutes LEDs corners order clockwise?";
    txtMinDir2 = "If this option is set, the minute leds will be in the 4 corners";
    txtMinDir3 = "displayed clockwise, otherwise counterclockwise.";

    // PING monitor IP-adresses:
    txtPing0 = "PING monitor for IP address(es) -> Turn off LEDs if IP(s) are offline for a period of time";
    txtPing1 = "Use PING monitor function?";
    txtPing2 = "Please enter the IP address(es) to be monitored here";
    txtPing3 = "1. IP address";
    txtPing4 = "2. IP address";
    txtPing5 = "3. IP address";
    txtPing6 = "Note: An IP address with the value 0.0.0.0 will be skipped in the query.";
    txtPing7 = "Number of PING attempts until the LEDs are switched off";
    txtPing8 = "Note: Count = 10 means a 5 minute timeout as there are 2 PING attempts per minute.";
    txtPing9 = "Use DEBUG PING monitor function?";

    // Hostname:
    txtHostName1 = "Customize WordClock hostname";
    txtHostName2 = "Hostname";

    // REST functions:
    txtREST0 = "REST functions";
    txtREST1 = "WordClock functions can be controlled externally via the following links.";
    txtREST2 = "Use REST function?";
    txtREST3 = "The WordClock can be switched on and off manually via the browser via one of the following links";
    txtREST4 = "Turn off LEDs";
    txtREST5 = "Turn on LEDs";
    txtREST6 = "LED state";
    txtRESTX = "The REST function is currently disabled.";

    // Update function:
    txtUpdate0 = "Update";
    txtUpdateE1 = "Do not use the update function";
    txtUpdateE2 = "Use local update function";
    txtUpdateE3 = "Use automatic update function via internet";
    txtUpdate2 = "Using one of the links below, the WordClock can be updated via the browser without the Arduino IDE";
    txtUpdate3 = "Note: A .BIN file of the sketch previously created in the Arduino IDE with Ctrl+Alt+S is required,";
    txtUpdate4 = "which can be uploaded via the 'Update Firmware' option.";
    txtUpdate5 = "The necessary update file can be downloaded here";
    txtUpdate6 = "WordClock repository on GitHub";
    txtUpdate7 = "The installed version is the same as the available version";
    txtUpdate8 = "Update available to version";
    txtUpdate9 = "Use the following link to start the update";
    txtUpdateX = "The update function is currently disabled.";

    // WiFi:
    txtWiFi0 = "Reset WiFi settings";
    txtWiFi1 = "Reset wifi settings and restart watch?";
    txtWiFi2 = "If this option is used, the WiFi settings will be deleted";

    // Restart:
    txtRestart0 = "Restart WordClock";
    txtRestart1 = "Restart WordClock?";
    txtRestart2 = "If this option is used, the clock will be restarted";

    // Time zone and NTP server:
    txtTZNTP0 = "Time zone & NTP server";
    txtTZNTP1 = "Default values";
    txtTZNTP2 = "Explanation of setting the time zone";
  }
}
// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################
