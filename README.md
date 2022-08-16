# WordClock

This is the <b>WordClock</b> software to the thingiverse project: <a href="https://www.thingiverse.com/thing:4693081" target="_blank">Wordclock Wortuhr</a>

Feel free to add functions, ideas and everything else, which helps this project to grow! 

Current version: 4.0

<hr>

<img src="9CA8B21F-5ED3-4F7D-95CA-FCF97703D408.jpeg">
Time here is 07:43 or 19:43 o'clock

<hr>

<b>Hardware:</b>
- Node MCU V3 - ESP8266 board
- RTC (DS3231) board 
- 114 LEDs of an WS2812B led stripe 

<hr>

<b>Available functions:</b>
- Check the settings.h file for the latest functions
- WiFi Manager integration to avoid setting your WiFi credentials manually in the code
- The from your WiFi router received IP-address can be shown as a scrolling text to inform you about the value
- The configuraton can be set with an integrated web portal
- The LED color can be comfigured to a static value
- The LED intensity can be set for a day time and optional for a night mode usage
- To every new hour the hour text can flash to notify about the new hour reached
- Optional you can chose to show the date as scrolling text every minute after 30 seconds
- A time span can be selected to turn off the LEDs fully or reduce their intensity for night time usage
- Updates of the software can be done with the web portal as well to avoid connecting the decice to Arduino IDE again. The needed .bin file is included
- During startup you can choose if the following functions should be 
-- 


<hr>

<b>Version history:</b>
- 1.0 to 3.4: 	Initial releases shared on thingiverse
- 3.5 to 3.9.3:	Minor function changes and code cleanup
- 4.0: 		Added an option to periodically ping a IP-address of f.e. your smart phone. 
		If the IP-address does not answer for a period of time the LEDs turn off. 
		This will allow to reduce power consumption of the WordClock when you are away.
		The ping request is done 2x every minute. So if the IP-address can be reached
		again, the LEDs will turn on after a few seconds
