#include "Button.h"
#include "Arduino.h"

int ButtonManager::mNumButtons = 0;
Button ButtonManager::mButtons[NUM_BUTTONS];

Button * ButtonManager::registerButton(int pin, int bounceTime, void (*onPress)(), void (*onRelease)())
{
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
    attachInterrupt(pin, onInterrupt, CHANGE);
    
    Button * newButton = &mButtons[mNumButtons];
    newButton->pin = pin;
    newButton->lastChange = 4294967295;
    newButton->state = HIGH;
    newButton->onPress = onPress;
    newButton->onRelease = onRelease;
    ++mNumButtons;
    return newButton;
}

// pos, turn, oldVal
void ButtonManager::onInterrupt()
{
    //Serial.println("onInterrupt");
    for (short i = 0; i < mNumButtons; ++i)
    {
        Button * button = &mButtons[i];
        // did this button change?
        if (button->state == digitalRead(button->pin))
            continue;
        // check state change
        unsigned long time = millis();
        if (time - button->lastChange > button->bounceTime)
        {
            button->lastChange = time;
            if (button->state == HIGH)
            {
                button->state = LOW;
                if (button->onRelease != 0)
                {
                    button->onRelease();
                }
            }
            else
            {
                button->state = HIGH;
                if (button->onPress != 0)
                {
                    button->onPress();
                }
            }
        }
    }
}
