# WordClock

This is the <b>WordClock</b> software to the thingiverse project: <a href="https://www.thingiverse.com/thing:4693081" target="_blank">Wordclock Wortuhr</a>

Feel free to add functions, ideas and everything else, which helps this project to grow! 

<hr>

<h3><b>Current version: 4.9         </b></h3>

<hr>

<img src="./Images/WordClock.jpeg">
Time here is 07:43 or 19:43 o'clock

<hr>

<h3><b>Required Hardware:</b></h3>

- Node MCU V3 - ESP8266 board
- RTC (DS3231) board 
- 114 LEDs of an WS2812B led stripe 
- Powersupply 5V / >=3A

<hr>

<h3><b>Available functions:</b></h3>

- Note: Check the settings.h file for the latest functions
- WiFi Manager integration to avoid setting your WiFi credentials manually in the code
- The from your WiFi router received IP-address can be shown as a scrolling text to inform you about the current value
- During startup in default a LED test function is shown to check your wiring. All LEDs should light up in green color and turn off again
- During startup a text "SET WLAN" is shown until the WiFi settings were done for the first time or afterwards until the device received a new IP-address
- The configuraton can be set with an integrated web portal by f.e. http://WordClock0 or http://192.168.178.50 (depending on your settings)
- The LED color can be configured to a static value
- The LED intensity can be set for a day time and optional for a night mode usage
- To every new hour the hour text can flash orange to notify about the new reached hour
- Optional you can choose to show the date as scrolling text every minute + 30 seconds
- A time span can be selected to turn off the LEDs fully or reduce their intensity for night time usage for each day seperatelly
- Updates of the software can be done with the web portal as well to avoid connecting the device to Arduino IDE again. The needed .bin file is included
- During startup you can choose if the following functions should be displayed: IP-address as scrolling text, the SET WLAN text and/or the LED test function
- The LED color can be shown as randomized 'rainbow' color changed every new minute or the separate words can be shown in a static rainbow color
- You can choose if the minute LEDs in the corners should be rotated in clockwise or anti clockwise direction
- The in the internal WifiManager stored WiFi settings can be set to default on request to switch to a new WiFi router afterwards
- In case the WiFi is no longer available the device will reset the WiFi settings automatically after 240s to avoid flashing with Arduino IDE again
- You can configure the hostname of the device that will be tried to tranfer to your routers DNS configuration to reach the web portal via DNS name too
- A restart request can be forced with the web portal as well
- The timezone and NTP-Server configuration can be configured as well to fit to your demands
- Optional you can set up to 3 IP-addresses (of f.e. your smart phones) connected on your network to be monitored via PING requests 2x per minute. After a set amount of failed PING attempts the WordClock LEDs will turn off to reduce power consumption when you are not at home

<hr>

<h3>Web configuration interface:</h3>

<img src="./Images/IMG_1791.jpg">
<img src="./Images/IMG_1792.jpg">
<img src="./Images/IMG_1793.jpg">

<hr>

<h3><b>Version history:</b></h3>

- 1.0 to 3.4: 	Initial releases shared on thingiverse
- 3.5 to 3.9.3:	Minor function changes and code cleanup
- 4.0: 		Added an option to periodically ping a IP-address of f.e. your smart phone. 
		If the IP-address does not answer for a period of time the LEDs turn off. 
		This will allow to reduce power consumption of the WordClock when you are away.
		The ping request is done 2x every minute. So if the IP-address can be reached
		again, the LEDs will turn on after a few seconds
- 4.1: 		Minor text changes
- 4.2: 		Integrated 2 REST urls to manually turn the LEDs on or off from an external control like f.e. your smart home controller
- 4.3: 		Minor text changes. Added 2 more REST urls for an experimental twinkle mode LED test
- 4.4: 		Changed the startup LED test function to reduce power consumption to fit 5V/3A power supplies, changed the maximum setting for the brightness sliders to 128 from 255. A note for the power supply check was added. The RESET and WiFI RESET function were recreated to remove an error
- 4.5: 		Added an option to apply the power supply message and to raise the maximum LED intensity again to 255. Code cleanup. Changed the HTML page settings to show german umlauts (äöüß) now correctly.
- 4.6: 		Enhanced the PING option for now up to 3 IP-addresses that can be monitored. Code cleanup. Added another url to be able to querry the status of the LEDs.
- 4.7: 		Code cleanup. Added a small How-To to add the WordClock REST urls to HomeBridge to be able to control the WordClock from HomeKit and via Siri.
- 4.8:          Added a WiFi reconnect function in case the WiFi connection gets aborted during longer usage. Added a live update of the intensity value text fields next to the 2 sliders. Smaller changes to some functions.
- 4.9:          Added a radio button selector to switch between 2 rainbow color modes. Added a URL to get to the Wordclock repository page on GitHub.

<hr>

<h3><b>How-To's:</b></h3>

<hr>

<b>Add WordClock to Homebridge and control it with HomeKit and via Siri in your smart home:</b>

<b>Note:</b>
- The here meantioned REST url functions will work similar f.e. in HomeAssistant or other solutions too.

<b>Preconditions:</b>
- Your smart home alreday runs "HomeBridge" (https://homebridge.io) and the here shown "Homebridge UI" plugin (https://github.com/oznu/homebridge-config-ui-x) to add the configuration via web interface.

<b>Setup:</b>
- In HomeBridge add the following plugin "Homebridge Http Switch" (https://github.com/Supereg/homebridge-http-switch) 

- After the installation the "Homebridge Http Switch" plugin will be shown:
<img src="./Images/IMG_1647.PNG">

- Configure its "Settings" and add the WordClock as a new "Accessory": 
<img src="./Images/IMG_1648.PNG">

- Add the following code to the accessory after changing the value "WordClockIP" to your WordClock IP-address as well as the "name" value:

<pre><code>
{
    "accessory": "HTTP-SWITCH",
    "name": "WordClock 2",
    "switchType": "stateful",
    "onUrl": {
        "url": "http://WordClockIP:55555/ledson",
        "method": "GET"
    },
    "offUrl": {
        "url": "http://WordClockIP:55555/ledsoff",
        "method": "GET"
    },
    "statusUrl": {
        "url": "http://WordClockIP:55555/ledstatus",
        "method": "GET"
    }
}

</code></pre>

- Save the changes and use the HomeBridge UI control to restart HomeBridge.

- After the HomeBridge restart your WordClock can be controlled to turn the LEDs OFF and ON in HomeBridge, HomeKit, automation scenarios and via Siri:
<img src="./Images/IMG_1649.PNG">

<hr>
