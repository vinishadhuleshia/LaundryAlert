# LaundryAlert

Uses an ESP8266 Wifi module running on Arduino IDE to monitor the status of the washing machine with a vibration sensor and alert the user by email or text when the washer load is complete.

Uses a vibration sensor (SW-420) connected to an ESP8266 Wifi module with three female to female jumper cables using the 3.3 Volt pin on the ESP8266 board.

Collects the vibration sensor value every 1/4th of a second and increments a vibration count to monitor the status of the washing machine. When the count reaches 300, the washer status changes to 'running'. 

Every second that the vibration sensor does not detect a vibration (the washer is still not shaking), the count is decremented. Once the count reaches 0, after a minimum of 300 seconds, the status changes to 'done' if it was previously 'running'. 

A message will then be sent as an email to the recipient's email or phone number (to act as a text).

The user can customize between any messages and/or add more and change the vibration count max of 300. Changing the vibration count max will result in a quicker notification once the washer is complete but may inacurately portray the status of the washer.
