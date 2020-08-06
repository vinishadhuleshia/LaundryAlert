# LaundryAlert

Uses an ESP8266 Wifi module running on Arduino IDE to monitor the status of the washing machine with a vibration sensor and alert the user by email or text when the washer load is complete.

Used a vibration sensor (SW-420) connected to an ESP8266 Wifi module with three female to female jumper cables using the 3.3 Volt pin on the ESP8266 board.

Collects the vibration sensor data every 1/4th of a second and increments a vibration count to track the status of the washing machine. When the count reaches 300, the washer status changes to 'running'.

Every second that the vibration sensor does not detect a vibration, the count is decremented. Once the count reaches 0, after a minimum of 300 seconds, the status changes to 'done' if it was previously 'running'. It will then send an email to the recipient or a phone number to act as a text.

You can personalize the message sent or add more options to randomize between.
