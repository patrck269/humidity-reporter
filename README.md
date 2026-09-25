# Humidity Reporter

Adafruit QT Py ESP32-C3 firmware for a DHT11 left in one place.

On boot it joins Wi-Fi at `192.168.5.<UNITNUM>`, reads humidity and temperature in Fahrenheit, and sends one UDP datagram:

```
Humidity:<h> Temperature:<f> Location:<LOCATION>
```

GPIO 3 stays on while it is awake. It then deep-sleeps for 240 seconds and boots again.

Set `ssid`, `password`, and `udpAddress` in `src/main.cpp` before flashing. `UNITNUM` and `LOCATION` name the sensor.

PlatformIO environment: `adafruit_qtpy_esp32c3`.
