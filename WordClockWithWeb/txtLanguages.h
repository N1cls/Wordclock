// ###########################################################################################################################################
// #
// # WordClock code for the thingiverse WordClock project: https://www.thingiverse.com/thing:4693081 - language file:
// #
// # Code by https://github.com/N1cls and https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0 https://github.com/N1cls/Wordclock/blob/master/LICENSE.md
// #
// # Compatible with WordClock version: V5.0
// #
// ###########################################################################################################################################


// ###########################################################################################################################################
// # Default texts in german language:
// ###########################################################################################################################################
// General texts:
String WordClockName = "WordClock";
String languageSelect = "Sprache";
String languageInt0 = "Deutsch";
String languageInt1 = "Englisch";
String txtSaveSettings = "Einstellungen speichern";
// LED settings:
String txtSettings = "Einstellungen";
String txtLEDsettings = "LED Einstellungen";
String txtLEDcolor = "Farbe";
String txtIntensityDay = "Helligkeit am Tag";
String txtIntensityNight = "Helligkeit bei Nacht";
String txtPowerSupplyNote1 = "Wichtig: Beide Werte begrenzt auf 128 von maximal 255. Achte darauf ein geeignetes Netzteil zu verwenden!";
String txtPowerSupplyNote2 = "Je nach LED Anzahl, selektierter Farbe und Helligkeit wird mindestens ein 5V/3A Netzteil empfohlen!";
String txtPowerSupplyNote3 = "Den Hinweis zum Netzteil akzeptiere und beachte ich. Ich möchte die Werte wieder auf maximal 255 einstellen können";
String txtPowerSupplyNote4 = "Den wichtigen Hinweis zum Netzteil nicht mehr anzeigen";
String txtFlashFullHour1 = "Volle Stunde blinken";
String txtFlashFullHour2 = "Stundenangabe soll zur vollen Stunde blinken?";
String txtShowDate1 = "Datumsanzeige als Lauftext";
String txtShowDate2 = "Alle 30 Sekunden anzeigen?";
String txtNightMode1 = "Display abschalten oder dunkler schalten?";
String txtNightMode2 = "Display komplett abschalten ...";
String txtNightMode3 = "... oder nur dunkler schalten auf Wert der Helligkeit bei Nacht?";
String txtNightModeOff = "Display aus ab";
String txtNightModeTo = "bis";
String txtNightModeClock = "Uhr";
String txtMO = "Montag";
String txtTU = "Dienstag";
String txtWE = "Mittwoch";
String txtTH = "Donnerstag";
String txtFR = "Freitag";
String txtSA = "Samstag";
String txtSU = "Sonntag";
// Content and startup:
String txtContentStartup = "Anzeigen und Startverhalten";
String txtUseLEDtest = "LED Start Test anzeigen?";
String txtUSEsetWLAN = "SET WLAN beim Start anzeigen?";
String txtShowIP = "IP-Addresse beim Start anzeigen?";
String txtRainbow1 = "Wähle den Regenbogen Farbeffekt Modus";
String txtRainbow2 = "Aus";
String txtRainbow3 = "Variante 1 (Worte verschieden bunt)";
String txtRainbow4 = "Variante 2 (Alle Worte zufällig bunt)";
String txtMinDir1 = "Minuten LEDs Ecken Reihenfolge im Uhrzeigersinn?";
String txtMinDir2 = "Wenn diese Option gesetzt wird, werden die Minuten-LEDs in den 4 Ecken";
String txtMinDir3 = "im Uhrzeigersinn angezeigt, ansonsten entgegen dem Uhrzeigersinn.";
// PING monitor IP-adresses:
String txtPing0 = "PING Monitor für IP-Adresse(n) -> LEDs abschalten wenn IP(s) länger offline";
String txtPing1 = "PING Monitor Funktion verwenden?";
String txtPing2 = "Bitte hier die zu überwachende(n) IP-Adresse(n) eintragen";
String txtPing3 = "1. IP-Adresse";
String txtPing4 = "2. IP-Adresse";
String txtPing5 = "3. IP-Adresse";
String txtPing6 = "Hinweis: Eine IP-Addresse mit dem Wert 0.0.0.0 wird in der Abfrage übersprungen.";
String txtPing7 = "Anzahl PING Versuche bis die LEDs abgeschaltet werden";
String txtPing8 = "Hinweis: Anzahl = 10 bedeutet einen 5 Minuten Timeout, da 2 PING Versuche pro Minute erfolgen.";
String txtPing9 = "DEBUG PING Monitor Funktion verwenden?";
// Hostname:
String txtHostName1 = "WordClock Hostname anpassen";
String txtHostName2 = "Hostname";
// REST functions:
String txtREST0 = "REST Funktionen";
String txtREST1 = "Über die folgenden Links können Funktionen der WordClock von Außen gesteuert werden.";
String txtREST2 = "REST Funktion verwenden?";
String txtREST3 = "Über einen der folgenden Links kann die WordClock manuell über den Browser ab und an geschaltet werden";
String txtREST4 = "LEDs ausschalten";
String txtREST5 = "LEDs einschalten";
String txtREST6 = "LED Status";
String txtREST7 = "Weitere Funktionen (experimentell)";
String txtREST8 = "LED Test einschalten";
String txtREST9 = "LED Test ausschalten";
String txtRESTX = "Die REST Funktion ist aktuell deaktiviert.";
// Update function:
String txtUpdate0 = "Update";
String txtUpdate1 = "Update Funktion verwenden?";
String txtUpdate2 = "Über einen der folgenden Links kann die WordClock über den Browser ohne Arduino IDE aktualisiert werden";
String txtUpdate3 = "Hinweis: Es wird eine in der Arduino IDE mit Strg+Alt+S zuvor erstellte .BIN Datei des Sketches benötigt,";
String txtUpdate4 = "die über die Option 'Update Firmware' hochgeladen werden kann.";
String txtUpdate5 = "Die notwendige Update-Datei kann hier heruntergeladen werden";
String txtUpdate6 = "Wordclock Repository auf GitHub";
String txtUpdateX = "Die Update Funktion ist aktuell deaktiviert.";
// WiFi:
String txtWiFi0 = "WLAN Einstellungen zurücksetzen";
String txtWiFi1 = "WLAN Einstellungen zurücksetzen und Uhr neu starten?";
String txtWiFi2 = "Wenn diese Option verwendet wird, werden die WLAN Einstellungen einmalig gelöscht";
// Restart
String txtRestart0 = "WordClock neustarten";
String txtRestart1 = "WordClock neustarten?";
String txtRestart2 = "Wenn diese Option verwendet wird, wird die Uhr einmalig neu gestartet";
// Time zone and NTP server:
String txtTZNTP0 = "Zeitzone & NTP-Server";
String txtTZNTP1 = "Standardwerte";
String txtTZNTP2 = "Erklärung zur Einstellung der Zeitzone";


void setLanguage(int lang) {
  // ###########################################################################################################################################
  // # Translations for: DE
  // ###########################################################################################################################################
  if (lang == 0) {         // DEUTSCH
    // Allgemeine Texte:
    WordClockName = "WortUhr";
    languageSelect = "Sprache";
    languageInt0 = "Deutsch";
    languageInt1 = "Englisch";
    txtSaveSettings = "Einstellungen speichern";

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
    txtShowDate2 = "Alle 30 Sekunden anzeigen?";
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
    txtREST7 = "Weitere Funktionen (experimentell)";
    txtREST8 = "LED Test einschalten";
    txtREST9 = "LED Test ausschalten";
    txtRESTX = "Die REST Funktion ist aktuell deaktiviert.";

    // Update Funktion:
    txtUpdate0 = "Update";
    txtUpdate1 = "Update Funktion verwenden?";
    txtUpdate2 = "Über einen der folgenden Links kann die WordClock über den Browser ohne Arduino IDE aktualisiert werden";
    txtUpdate3 = "Hinweis: Es wird eine in der Arduino IDE mit Strg+Alt+S zuvor erstellte .BIN Datei des Sketches benötigt,";
    txtUpdate4 = "die über die Option 'Update Firmware' hochgeladen werden kann.";
    txtUpdate5 = "Die notwendige Update-Datei kann hier heruntergeladen werden";
    txtUpdate6 = "Wordclock Repository auf GitHub";
    txtUpdateX = "Die Update Funktion ist aktuell deaktiviert.";

    // WLAN:
    txtWiFi0 = "WLAN Einstellungen zurücksetzen";
    txtWiFi1 = "WLAN Einstellungen zurücksetzen und Uhr neu starten?";
    txtWiFi2 = "Wenn diese Option verwendet wird, werden die WLAN Einstellungen einmalig gelöscht";

    // Neustart:
    txtRestart0 = "WordClock neustarten";
    txtRestart1 = "WordClock neustarten?";
    txtRestart2 = "Wenn diese Option verwendet wird, wird die Uhr einmalig neu gestartet";

    // Zeitzone and NTP-Server:
    txtTZNTP0 = "Zeitzone & NTP-Server";
    txtTZNTP1 = "Standardwerte";
    txtTZNTP2 = "Erklärung zur Einstellung der Zeitzone";
  }


  // ###########################################################################################################################################
  // # Translations for: EN
  // ###########################################################################################################################################
  if (lang == 1) {         // ENGLISH
    // General texts:
    WordClockName = "WordClock";
    languageSelect = "Language";
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
    txtShowDate2 = "Display the date every 30 seconds?";
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
    txtUSEsetWLAN = "Show SET WLAN text on startup?";
    txtShowIP = "Show IP-address on startup?";
    txtRainbow1 = "Choose the rainbow color effect mode";
    txtRainbow2 = "Off";
    txtRainbow3 = "Variant 1 (words in different colours)";
    txtRainbow4 = "Variant 2 (all words randomly colored)";
    txtMinDir1 = "Minutes LEDs corners order clockwise?";
    txtMinDir2 = "If this option is set, the minute leds will be in the 4 corners";
    txtMinDir3 = "displayed clockwise, otherwise counterclockwise.";

    // PING monitor IP-adresses:
    txtPing0 = "PING monitor for IP address(es) -> Turn off LEDs if IP(s) are offline for a longer period of time";
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
    txtREST7 = "Other functions (experimental)";
    txtREST8 = "Switch on the LED test";
    txtREST9 = "Switch off the test LED";
    txtRESTX = "The REST function is currently disabled.";

    // Update function:
    txtUpdate0 = "Update";
    txtUpdate1 = "Use update function?";
    txtUpdate2 = "Using one of the links below, the WordClock can be updated via the browser without the Arduino IDE";
    txtUpdate3 = "Note: A .BIN file of the sketch previously created in the Arduino IDE with Ctrl+Alt+S is required,";
    txtUpdate4 = "which can be uploaded via the 'Update Firmware' option.";
    txtUpdate5 = "The necessary update file can be downloaded here";
    txtUpdate6 = "WordClock repository on GitHub";
    txtUpdateX = "The update function is currently disabled.";

    // WiFi:
    txtWiFi0 = "Reset WiFi settings";
    txtWiFi1 = "Reset wifi settings and restart watch?";
    txtWiFi2 = "If this option is used, the WLAN settings will be deleted once";

    // Restart:
    txtRestart0 = "Restart WordClock";
    txtRestart1 = "Restart WordClock?";
    txtRestart2 = "If this option is used, the clock will be restarted once";

    // Time zone and NTP server:
    txtTZNTP0 = "Time zone & NTP server";
    txtTZNTP1 = "Default values";
    txtTZNTP2 = "Explanation of setting the time zone";
  }


}
// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################
