#include "BluetoothSerial.h"

BluetoothSerial SerialBT;  

int8_t axis[7] = {5,6,7,8,9,10,11};
byte btn[8] = {0,0,0,0,0,0,0,0};
byte btn_last[8] = {0,0,0,0,0,0,0,0};
int speed_coef = 100; // nasobeni hodnoty, co leze z joysticku

unsigned long last_millis = 0;
bool rotating = false; // otáčí se?
int a = 10;

bool read_joystick()
{
    if (BTworks) {
        if ( SerialBT.available() == 0 )
            return false;

        int test = SerialBT.read();
        if (test == 0x80)
        {
            int axis_count = SerialBT.read();
            for (int x = 0; x < axis_count; x++)
            {
                while(SerialBT.available() < 1)
                {
                    // DO NOTHING - WAITING FOR PACKET
                    delay(1);
                }

                int8_t tmp = SerialBT.read();
                axis[x] = tmp;
                SerialBT.print(x);
                SerialBT.print(": ");
                SerialBT.print(axis[x], DEC);
                SerialBT.print(" ");

            }
            return true;
        }
        else if  ( test == 0x81 )
        {
            while(SerialBT.available() < 1) {
                // DO NOTHING - WAITING FOR PACKET
                delay(1);
            }
            byte a = SerialBT.read();
            while(SerialBT.available() < 1) {
                // DO NOTHING - WAITING FOR PACKET
                delay(1);
            }
            btn_last[a] = btn[a];
            btn[a] = SerialBT.read();
            Serial.print(a, DEC); Serial.print(": "); Serial.print(btn[a], DEC); Serial.print("last: "); Serial.print(btn_last[a], DEC);
            return true;
        }
        return false;
    }

    else {
        if ( Serial.available() == 0 )
            return false;

        int test = Serial.read();
        if (test == 0x80)
        {
            int axis_count = Serial.read();
            for (int x = 0; x < axis_count; x++)
            {
                while(Serial.available() < 1)
                {
                    // DO NOTHING - WAITING FOR PACKET
                    delay(1);
                }

                int8_t tmp = Serial.read();
                axis[x] = tmp;
                Serial.print(x);  
                Serial.print(": ");
                Serial.print(axis[x], DEC);
                Serial.print(" ");
            }
            return true;
        }
        else if  ( test == 0x81 )
        {
            while(Serial.available() < 1) {
                // DO NOTHING - WAITING FOR PACKET
                delay(1);
            }
            byte a = Serial.read();
            while(Serial.available() < 1) {
                // DO NOTHING - WAITING FOR PACKET
                delay(1);
            }
            btn_last[a] = btn[a];
            btn[a] = Serial.read();
            Serial.print(a, DEC); Serial.print(": "); Serial.print(btn[a], DEC); Serial.print("last: "); Serial.print(btn_last[a], DEC);
            return true;
        }
        return false;
    }
}


