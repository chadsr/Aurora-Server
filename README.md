# Aurora Server (ESP32)
[![Build](https://github.com/Chadsr/Aurora-Server/actions/workflows/build.yml/badge.svg)](https://github.com/Chadsr/Aurora-Server/actions/workflows/build.yml)
### A wireless, ESP32 controlled, addressable RGB LED strip server (UDP streaming &amp; JSON API)

## Setup
### 1.
```
git clone git@github.com:Chadsr/Aurora-Server.git
cd Aurora-Server/
```

### 2. Configure
#### 2.1 Setup network credentials
Edit `src/main.h` with your wireless network AP (2.4Ghz) SSID and password.

#### 2.2 Setup LED quantity and data pin
Edit `LED_PIN` and `NUM_LEDS` in `src/leds.h`, to reflect your setup.

## Compilation & Upload

### 1. Using PlatformIO
Connect your device via usb and run:
```
platformio run --target upload
```

## API Endpoints

| Endpoint       | Type   | Description                                                                   | Example Return Value |
| -------------- | ------ | ----------------------------------------------------------------------------- | -------------------- |
| `/leds`        | `GET`  | Returns a count of the available induvidual LEDs                              | `{"count": 720}`     |
| `/leds`        | `POST` | Accepts a list of RGB colour assignments for sequences of LEDs <sup>[1]</sup> | `{"success": true}`  |
| `/leds/toggle` | `POST` | Toggles all configures LEDs on/off                                            | `{"success": true}`  |

### Example Request: <sup>[1]</sup>
```
{
  "clearPrevious": true, 
    "sections": [
      {
            "r": 50,
            "g": 255,
            "b": 0,
            "startPosition": 1,
            "endPosition": 240
        },
        {
            "r": 100,
            "g": 100,
            "b": 100,
            "startPosition": 241,
            "endPosition": 480
        },
              {
            "r": 0,
            "g": 255,
            "b": 0,
            "startPosition": 481,
            "endPosition": 720
        }
    ]
}
```
### clearPrevious
A boolean value specifying whether previous LED assignments should be reset (turned off) or if the request should be applied directly.
### sections
A list of colour sequences
#### r, g, b
Unsigned integer colour values representing an RGB colour.
#### startPosition
The LED index to start applying the given colour to.
#### endPosition
The LED index to finish applying the given colour to.

## UDP Streaming
*UDP streaming is still experimental and has not been tested. Development of a client application can be found in [this repository](https://github.com/Chadsr/Aurora-Client).*
