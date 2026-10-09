#include "mbed.h"
#include "Photoresistor.h"
#include "Grove_LCD_RGB_Backlight.h"
#include "rainbow_bg.h"


Grove_LCD_RGB_Backlight lcd(D14, D15); // Default I2C pins for the Grove LCD RGB Backlight
AnalogIn rotarySensor(A0); // Analog pin for the photoresistor
DigitalIn button(D2); // Digital pin for the button
DigitalOut led(D4); // Digital pin for the LED

enum class AppState{
    MODE_NORMAL,
    MODE_SET_LOWER,
    MODE_SET_UPPER
};

AppState currentState = MODE_NORMAL;

float lowerThreshold = 40.0f; // Default lower threshold for light intensity
float upperThreshold = 60.0f; // Default upper threshold for light intensity
bool ledState = false; // State of the LED (on/off)

int button_hold_counter = 0; // Counter for button hold duration
bool button_was_pressed = false; // Flag to track if the button was pressed


int main()
{
    char buffer[16]; // Buffer for LCD display

    lcd.clear();    // Initialize the LCD display

    while (true){
        bool button_is_pressed = button.read(); // Read the button state
        bool button_clicked = false; // Flag to indicate if the button was clicked
        bool button_long_pressed = false; // Flag to indicate if the button was long pressed

        if (button_is_pressed){
            button_hold_counter++; // Increment the hold counter if the button is pressed
            button_was_pressed = true; // Set the flag indicating the button was pressed

            if (button_hold_counter >= 250 && currentState == AppState::MODE_NORMAL) { // Check for long press in normal mode
                button_long_pressed = true; // Set the long press flag
                button_hold_counter = 0; // Reset the hold counter
            }
        }
        else{
            if (button_was_pressed && button_hold_counter > 0 && button_hold_counter < 250) { // Check for a short press
                button_clicked = true; // Set the click flag
            }
            button_hold_counter = 0; // Reset the hold counter
            button_was_pressed = false; // Reset the button pressed flag
        }

        if (currentState == AppState::MODE_NORMAL){
            if (button_long_pressed){
                currentState = AppState::MODE_SET_LOWER;
                lcd.clear();
                while(button.read() == 1) {ThisThread::sleep_for(10ms);} // Wait for button release
                continue;
            }

            float lux = calculateLux(); // Calculate the current light intensity in lux
            float percentage = (lux / 355.0f) * 100.0f; // Convert lux to percentage (assuming 355 lux is the maximum)
            
            if (percentage < 0.0f) percentage = 0.0f; // Clamp percentage to 0%
            if (percentage > 100.0f) percentage = 100.0f;   // Clamp percentage to 100%

            if (percentage < lowerThreshold){
                ledState = true; // Turn on the LED if below lower threshold
            }
            else if (percentage > upperThreshold){
                ledState = false; // Turn off the LED if above upper threshold
            }

            city_led.write(ledState); // Update the LED state

            lcd.locate(0, 0);   // Move cursor to the first row
            sprintf(buffer, "Light: %.2f%%", percentage); // Format the light percentage
            lcd.print(buffer);  // Display the light percentage on the LCD

            lcd.locate(0, 1);   // Move cursor to the second row
            sprintf(buffer, "Lux: %.2f", lux); // Format the lux value
            lcd.print(buffer);  // Display the lux value on the LCD
            
        }
        else if (currentState == AppState::MODE_SET_LOWER){
            float current_rotary_value = rotarySensor.read() * 100.0f; // Read the rotary sensor value and convert to percentage

            lcd.locate(0, 0);   // Move cursor to the first row
            lcd.print("Set Lower Thresh:"); // Display the mode on the LCD
            lcd.locate(0, 1);   // Move cursor to the second row
            sprintf(buffer, "%.2f%%", current_rotary_value); // Format the current rotary
            lcd.print(buffer);  // Display the current rotary value on the LCD

            if (button_clicked){
                lowerThreshold = current_rotary_value;
                currentState = AppState::MODE_SET_UPPER; // Move to the next mode
                lcd.clear(); // Clear the LCD for the next mode
            }
        }
        else if (currentState == AppState::MODE_SET_UPPER){
            float current_rotary_value = rotarySensor.read() * 100.0f; // Read the rotary sensor value and convert to percentage

            lcd.locate(0, 0);   // Move cursor to the first row
            lcd.print("Set Upper Thresh:"); // Display the mode on the LCD
            lcd.locate(0, 1);   // Move cursor to the second row
            sprintf(buffer, "%.2f%%", current_rotary_value); // Format the current rotary
            lcd.print(buffer);  // Display the current rotary value on the LCD

            if (button_clicked){
                upperThreshold = current_rotary_value;
                currentState = AppState::MODE_NORMAL; // Return to normal mode
                lcd.clear(); // Clear the LCD for normal operation
            }
        }

        updateRainbowNonBlocking(lcd, 1.0f, 20ms); // Update the LCD backlight with a rainbow effect
        ThisThread::sleep_for(20ms); // Sleep for 20 milliseconds to control the loop frequency
    }

}

