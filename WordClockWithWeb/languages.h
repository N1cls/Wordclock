// ###########################################################################################################################################
// #
// # WordClock code for the thingiverse WordClock project: https://www.thingiverse.com/thing:4693081 - language file:
// #
// # Code by https://github.com/N1cls and https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0 https://github.com/N1cls/Wordclock/blob/master/LICENSE.md
// #
// # Compatible with WordClock version: V5.8
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
String WordClockName, languageSelect, languageInt0, languageInt1, languageInt2, txtSaveSettings;
// LED settings:
String txtSettings, txtLEDsettings, txtLEDcolor, txtIntensityDay, txtIntensityNight, txtPowerSupplyNote1, txtPowerSupplyNote2, txtPowerSupplyNote3, txtPowerSupplyNote4;
String txtFlashFullHour1, txtFlashFullHour2, txtShowDate1, txtShowDate2, txtNightMode1, txtNightMode2, txtNightMode3, txtNightModeOff, txtNightModeTo, txtNightModeClock;
String txtShowTemp1, txtShowTemp2, txtShowHumidity1, txtShowHumidity2, txtIntervalLabel, txtMinutesSuffix, txtSensorFound, txtSensorNotFound, txtShowNow;
String txtMO, txtTU, txtWE, txtTH, txtFR, txtSA, txtSU;
// Content and startup:
String txtContentStartup, txtUseLEDtest, txtUseBootText, txtBootText1, txtBootText2, txtBootTextHint, txtUSEsetWLAN, txtShowIP, txtRainbow1, txtRainbow2, txtRainbow3, txtRainbow4, txtMinDir1, txtMinDir2, txtMinDir3;
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


void setLanguage(int lang) {
  // ###########################################################################################################################################
  // # Translations for: DE
  // ###########################################################################################################################################
  if (lang == 0) {         // DEUTSCH
    // Allgemeine Texte:
    WordClockName = "WordClock";
    languageSelect = "Sprache für das WordClock Layout und die Web Konfiguration";
    languageInt0 = "Deutsch";
    languageInt1 = "Englisch";
    languageInt2 = "Spanisch";
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
    txtShowDate2 = "Datum anzeigen?";
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

    txtShowTemp1 = "Temperaturanzeige als Lauftext (GY-21 / HTU21D Sensor)";
    txtShowTemp2 = "Temperatur anzeigen?";
    txtShowHumidity1 = "Luftfeuchtigkeitsanzeige als Lauftext (GY-21 / HTU21D Sensor)";
    txtShowHumidity2 = "Luftfeuchtigkeit anzeigen?";
    txtIntervalLabel = "Anzeigen alle";
    txtMinutesSuffix = "Minute(n)";
    txtShowNow = "Jetzt anzeigen";
    txtSensorFound = "GY-21 / HTU21D Sensor gefunden.";
    txtSensorNotFound = "Kein GY-21 / HTU21D Sensor gefunden. Die Temperatur-/Feuchtigkeitsanzeige wird deaktiviert.";

    // Anzeigen und Startverhalten:
    txtContentStartup = "Anzeigen und Startverhalten";
    txtUseLEDtest = "LED Start Test anzeigen?";
    txtUseBootText = "Eigene Lauftexte beim Start anzeigen?";
    txtBootText1 = "Lauftext 1 (z.B. Firmenname): ";
    txtBootText2 = "Lauftext 2 (z.B. Name): ";
    txtBootTextHint = "Nur Großbuchstaben A-Z und Leerzeichen, max. 20 Zeichen. Leer lassen, um einen Text zu überspringen.";
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
  if (lang == 1) {         // ENGLISH
    // General texts:
    WordClockName = "WordClock";
    languageSelect = "Language for the WordClock layout and web configuration";
    languageInt0 = "German";
    languageInt1 = "English";
    languageInt2 = "Spanish";
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
    txtShowDate2 = "Show date?";
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

    txtShowTemp1 = "Show temperature as scrolling text (GY-21 / HTU21D sensor)";
    txtShowTemp2 = "Show temperature?";
    txtShowHumidity1 = "Show humidity as scrolling text (GY-21 / HTU21D sensor)";
    txtShowHumidity2 = "Show humidity?";
    txtIntervalLabel = "Display every";
    txtMinutesSuffix = "minute(s)";
    txtShowNow = "Show now";
    txtSensorFound = "GY-21 / HTU21D sensor found.";
    txtSensorNotFound = "No GY-21 / HTU21D sensor found. Temperature/humidity display will be disabled.";

    // Content and startup:
    txtContentStartup = "Content and startup";
    txtUseLEDtest = "Run LED test on startup?";
    txtUseBootText = "Show custom scrolling texts on startup?";
    txtBootText1 = "Boot text 1 (e.g. company name): ";
    txtBootText2 = "Boot text 2 (e.g. name): ";
    txtBootTextHint = "Only uppercase letters A-Z and spaces, max. 20 characters. Leave empty to skip a text.";
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


  // ###########################################################################################################################################
  // # Translations for: ES
  // ###########################################################################################################################################
  if (lang == 2) {         // ESPAÑOL
    // Textos generales:
    WordClockName = "WordClock";
    languageSelect = "Idioma para el diseño de WordClock y la configuración web";
    languageInt0 = "Alemán";
    languageInt1 = "Inglés";
    languageInt2 = "Español";
    txtSaveSettings = "Guardar configuración";

    // Configuración de los LED:
    txtSettings = "Configuración";
    txtLEDsettings = "Configuración de los LED";
    txtLEDcolor = "Color";
    txtIntensityDay = "Intensidad durante el día";
    txtIntensityNight = "Intensidad durante la noche";
    txtPowerSupplyNote1 = "Importante: Ambos valores están limitados a 128 de un máximo de 255. ¡Asegúrate de usar una fuente de alimentación adecuada!";
    txtPowerSupplyNote2 = "Según la cantidad de LEDs, el color seleccionado y la intensidad, se recomienda una fuente de alimentación de 5V/3A!";
    txtPowerSupplyNote3 = "Acepto y tengo en cuenta la nota sobre la fuente de alimentación. Quiero poder volver a establecer los valores hasta un máximo de 255";
    txtPowerSupplyNote4 = "No volver a mostrar la nota importante sobre la fuente de alimentación";
    txtFlashFullHour1 = "Parpadeo en hora en punto";
    txtFlashFullHour2 = "¿Parpadear el valor de la hora cada hora en punto?";
    txtShowDate1 = "Mostrar fecha como texto desplazante";
    txtShowDate2 = "¿Mostrar fecha?";
    txtNightMode1 = "¿Apagar o atenuar la pantalla?";
    txtNightMode2 = "Apagar la pantalla por completo...";
    txtNightMode3 = "...o solo atenuarla al valor de intensidad nocturna?";
    txtNightModeOff = "Apagar pantalla desde";
    txtNightModeTo = "hasta";
    txtNightModeClock = "en punto";
    txtMO = "Lunes";
    txtTU = "Martes";
    txtWE = "Miércoles";
    txtTH = "Jueves";
    txtFR = "Viernes";
    txtSA = "Sábado";
    txtSU = "Domingo";

    txtShowTemp1 = "Mostrar temperatura como texto desplazante (sensor GY-21 / HTU21D)";
    txtShowTemp2 = "¿Mostrar temperatura?";
    txtShowHumidity1 = "Mostrar humedad como texto desplazante (sensor GY-21 / HTU21D)";
    txtShowHumidity2 = "¿Mostrar humedad?";
    txtIntervalLabel = "Mostrar cada";
    txtMinutesSuffix = "minuto(s)";
    txtShowNow = "Mostrar ahora";
    txtSensorFound = "Sensor GY-21 / HTU21D encontrado.";
    txtSensorNotFound = "No se encontró ningún sensor GY-21 / HTU21D. La visualización de temperatura/humedad se desactivará.";

    // Contenido y comportamiento de inicio:
    txtContentStartup = "Contenido y comportamiento de inicio";
    txtUseLEDtest = "¿Ejecutar prueba de LED al iniciar?";
    txtUseBootText = "¿Mostrar textos personalizados al iniciar?";
    txtBootText1 = "Texto 1 (p. ej. nombre de la empresa): ";
    txtBootText2 = "Texto 2 (p. ej. nombre): ";
    txtBootTextHint = "Solo letras mayúsculas A-Z y espacios, máx. 20 caracteres. Deja vacío para omitir un texto.";
    txtUSEsetWLAN = "¿Mostrar texto WIFI al iniciar?";
    txtShowIP = "¿Mostrar dirección IP al iniciar?";
    txtRainbow1 = "Elige el modo de efecto de color arcoíris";
    txtRainbow2 = "Apagado";
    txtRainbow3 = "Variante 1 (palabras en diferentes colores)";
    txtRainbow4 = "Variante 2 (todas las palabras coloreadas al azar)";
    txtMinDir1 = "¿Orden de las esquinas de los LEDs de minutos en sentido horario?";
    txtMinDir2 = "Si se activa esta opción, los LEDs de minutos en las 4 esquinas se mostrarán";
    txtMinDir3 = "en sentido horario; de lo contrario, en sentido antihorario.";

    // Monitor PING de direcciones IP:
    txtPing0 = "Monitor PING para dirección(es) IP -> Apagar LEDs si la(s) IP(s) están fuera de línea durante un tiempo";
    txtPing1 = "¿Usar la función de monitor PING?";
    txtPing2 = "Introduce aquí la(s) dirección(es) IP a monitorizar";
    txtPing3 = "1ª dirección IP";
    txtPing4 = "2ª dirección IP";
    txtPing5 = "3ª dirección IP";
    txtPing6 = "Nota: Una dirección IP con el valor 0.0.0.0 se omitirá en la consulta.";
    txtPing7 = "Número de intentos de PING hasta que se apaguen los LEDs";
    txtPing8 = "Nota: Un valor de 10 significa un tiempo de espera de 5 minutos, ya que se realizan 2 intentos de PING por minuto.";
    txtPing9 = "¿Usar la función de monitor PING de DEPURACIÓN?";

    // Nombre de host:
    txtHostName1 = "Personalizar el nombre de host de WordClock";
    txtHostName2 = "Nombre de host";

    // Funciones REST:
    txtREST0 = "Funciones REST";
    txtREST1 = "Las funciones de WordClock se pueden controlar externamente mediante los siguientes enlaces.";
    txtREST2 = "¿Usar la función REST?";
    txtREST3 = "WordClock se puede encender y apagar manualmente desde el navegador mediante uno de los siguientes enlaces";
    txtREST4 = "Apagar LEDs";
    txtREST5 = "Encender LEDs";
    txtREST6 = "Estado de los LED";
    txtRESTX = "La función REST está actualmente desactivada.";

    // Función de actualización:
    txtUpdate0 = "Actualización";
    txtUpdateE1 = "No usar la función de actualización";
    txtUpdateE2 = "Usar función de actualización local";
    txtUpdateE3 = "Usar función de actualización automática vía Internet";
    txtUpdate2 = "Usando uno de los siguientes enlaces, WordClock se puede actualizar desde el navegador sin el IDE de Arduino";
    txtUpdate3 = "Nota: Se requiere un archivo .BIN del sketch creado previamente en el IDE de Arduino con Ctrl+Alt+S,";
    txtUpdate4 = "que se puede cargar mediante la opción 'Update Firmware'.";
    txtUpdate5 = "El archivo de actualización necesario se puede descargar aquí";
    txtUpdate6 = "Repositorio de WordClock en GitHub";
    txtUpdate7 = "La versión instalada es la misma que la versión disponible";
    txtUpdate8 = "Actualización disponible a la versión";
    txtUpdate9 = "Usa el siguiente enlace para iniciar la actualización";
    txtUpdateX = "La función de actualización está actualmente desactivada.";

    // WiFi:
    txtWiFi0 = "Restablecer configuración WiFi";
    txtWiFi1 = "¿Restablecer configuración WiFi y reiniciar el reloj?";
    txtWiFi2 = "Si se usa esta opción, se eliminará la configuración WiFi";

    // Reinicio:
    txtRestart0 = "Reiniciar WordClock";
    txtRestart1 = "¿Reiniciar WordClock?";
    txtRestart2 = "Si se usa esta opción, el reloj se reiniciará";

    // Zona horaria y servidor NTP:
    txtTZNTP0 = "Zona horaria y servidor NTP";
    txtTZNTP1 = "Valores predeterminados";
    txtTZNTP2 = "Explicación sobre la configuración de la zona horaria";
  }


}
// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################
