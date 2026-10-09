# Smart City Public Lights Controller

This repository contains the implementation for a smart public lighting control system. The project is designed to simulate a city's streetlights, automatically turning an LED on or off based on ambient light conditions, while utilizing hysteresis to prevent flickering.

This project was developed based on the requirements detailed in the `[AME2627] - OPTIONAL EXERCISE_2.pdf` document for the **Embedded and Mobile Applications** course.

## 📋 Features

- **Ambient Light Sensing:** Calculates ambient illuminance (in lux) using a photoresistor.
- **Hysteresis Control:** Implements dual thresholds (upper and lower) to prevent undesired oscillations (flickering) caused by measurement noise.
- **Adjustable Thresholds:** Uses a rotary angle sensor to manually adjust the threshold values.
- **Threshold Selection:** Uses a hardware button to toggle whether the rotary sensor is currently modifying the upper or lower hysteresis threshold.
- **System Feedback:** Uses an I2C LCD RGB display to show current lux values and threshold limits, and an LED to simulate the city lights.

## 🛠️ Hardware Requirements

- **Microcontroller:** Nucleo board + Shield
- **Sensors:**
  - Photoresistor sensor (LDR)
  - Rotary angle sensor (Potentiometer)
  - Push button
- **Actuators & Displays:**
  - Standard LED (to simulate city lights)
  - Grove LCD RGB Backlight (connected via I2C to pins D14/SDA and D15/SCL)
- **Miscellaneous:** Jumper wires

## 🧮 Technical Specifications

### Photoresistor & Lux Calculation
The system reads the analog output from the photoresistor (LDR) and calculates the illuminance in lux. According to the `Photoresistor Specifications.pdf`, the system uses a $3.3\text{V}$ reference voltage ($Vref$).

The formula used to calculate the lux is:
$$lux=\frac{((Vref*luxRel)*Vout)-luxRel}{Rl}$$

**Constants used:**
- $Rl = 10\text{K}\Omega$
- $luxRel = 500$
- $Vref = 3.3\text{V}$ (recommended)

*Reference Lux Values:*
- Pitch Dark: $0.4-5$ lux
- Dark Indoors: $51-95$ lux
- Reading: $326-355$ lux

### Hysteresis Implementation
To ensure system stability, the city lights (LED) do not rely on a single switching point. Instead, the application defines a **hysteresis band**:
- The lights turn **ON** when the ambient light falls below the lower threshold.
- The lights turn **OFF** when the ambient light rises above the upper threshold.
- While the light level fluctuates between the two thresholds (inside the hysteresis band), the lights maintain their current state.

### LCD Display (I2C)
The project utilizes a 2x16 LCD display with an adjustable RGB backlight. The core API functions utilized for the display include:
- `Grove_LCD_RGB_Backlight(PinName sda, PinName scl)`: Constructor.
- `setRGB(char r, char g, char b)`: Configures the screen's backlight color.
- `print(char *str)`: Prints text/values to the screen.
- `locate(char col, char row)`: Moves the cursor to specific coordinates.
- `clear()`: Clears the display.

## 🚀 Setup and Usage

1. Connect the Grove base shield to your Nucleo board.
2. Connect the Photoresistor to an Analog input pin.
3. Connect the Rotary angle sensor to an Analog input pin.
4. Connect the Push button to a Digital input pin.
5. Connect the LED to a Digital output pin.
6. Connect the Grove LCD RGB Backlight to the I2C port (D14/SDA, D15/SCL).
7. Compile and flash the binary to the Nucleo board.
8. Monitor the lux values and thresholds on the LCD (or debug console). Press the button to switch which threshold (upper/lower) you are configuring with the rotary angle sensor. 

## 📝 Course Information
**Course:** Embedded and Mobile Applications  
**Professor:** David Gamez 
**Original Specification:** `[AME2627] - OPTIONAL EXERCISE_2.pdf`