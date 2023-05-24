#include "BluetoothSerial.h"
#include "robotka.h"
#include <thread>
bool BTworks = true;
#include "joystick.h"  // na lorris je to COM5 - musi byt pripojeny analyzer i terminal

#define DELAY 50
#define DELAY_BLINK 250
#define LED_COUNT 8

/* 
1) zapnout robotku -> rozsviti se modra led 
2) stisknout up -> rozsviti se zluta led 
3) zapnu v Lorris sezeni Joystick2, v nem v analyzeru i terminalu COM5
4) vyzkousim jezdeni a jsem pripraven 
*/

void NightRider() {
    int cervena_Nig = 255;
    int zelena_Nig = 0;
    int modra_Nig = 0;

    while(true){
        if(rkButtonUp(true)) {
            break;
        }
        for(int cislo_led = 0; cislo_led < 8; cislo_led++){
            // nastav barvu na modrou pomocí RGB 
            rkSmartLedsRGB(cislo_led, cervena_Nig, zelena_Nig, modra_Nig);
            if( cislo_led > 0){ // pokud nejsi na prvni LED
            rkSmartLedsRGB(cislo_led-1, 0, 0, 0); // vypni predchozi LED
            }
            // pockej 400 ms
            delay(150);
        }
    
    }
}

void print() {
    while(true) {
        Serial.println(a);
        SerialBT.println(a);
        a++;
        delay(1000);
    }
}

void setup() {

    rkConfig cfg;

    cfg.owner = "vasek"; // Ujistěte se, že v aplikace RBController máte nastavené stejné
    cfg.name = "mojerobotka";
    cfg.motor_max_power_pct = 100; // limit výkonu motorů na xx %

    cfg.motor_enable_failsafe = false;
    cfg.rbcontroller_app_enable = false; // nepoužívám mobilní aplikaci (lze ji vypnout - kód se zrychlí, ale nelze ji odstranit z kódu -> kód se nezmenší)
    rkSetup(cfg);

    rkLedBlue(true); // cekani na stisk

    //NightRider();

    int cervena = 255;
    int zelena = 0;
    int modra = 0;

    for(int cislo_led = 0; cislo_led < 8; cislo_led++){
            rkSmartLedsRGB(cislo_led, cervena, zelena, modra); // nastav barvu na cervenou 
        }

    while(true) {
        SerialBT.println("waiting for push UP");
        Serial.println("waiting for push UP");
        if(rkButtonUp(true)) {
            break;
        }
        delay(100);
    }
    rkLedBlue(false);
    rkLedYellow(true);

    cervena = 0;
    zelena = 255;
    modra = 0;

    for(int cislo_led = 0; cislo_led < 8; cislo_led++){
            rkSmartLedsRGB(cislo_led, cervena, zelena, modra); // nastav barvu na cervenou 
        }

    Serial.begin(115200);  // COM port 25 pocitac Burda
    if (!SerialBT.begin("TCA-BRobotka")) //Bluetooth device name; zapnutí BT musí být až za rkSetup(cfg); jinak to nebude fungovat a bude to tvořit reset ESP32
    {
        Serial.println("!!! Bluetooth initialization failed!");
    } else {
        SerialBT.println("!!! Bluetooth work!\n");
        Serial.println("!!! Bluetooth work!\n");
        rkLedBlue(true);
        delay(300);
        rkLedBlue(false);
    }

    std::thread t1(print);

    // delay(300);
    fmt::print("{}'s Robotka '{}' with {} mV started!\n", cfg.owner, cfg.name, rkBatteryVoltageMv());
    //rkLedYellow(true); // robot je připraven


    while(true) {
        if ( read_joystick() )
            {
                float axis_1 = (abs(axis[2]) < 10) ? 0 : -axis[2] /128.0; // prohozeni os 0 a 3 pro lepsi ovladani
                //axis_0 = axis_0*axis_0*axis_0;
                float axis_2 = (abs(axis[0]) < 10) ? 0 : -axis[0] /128.0; 
                // axis_1 = axis_1*axis_1*axis_1;
                int levy_m = -(axis_1 + (axis_2 *0.5)) * speed_coef;  // hodnota pro levy motor
                int pravy_m = (axis_1 - (axis_2 *0.5)) * speed_coef; // hodnota pro pravy motor 
                if(levy_m > 100) 
                    levy_m = 100;
                if(pravy_m > 100) 
                    pravy_m = 100;
                if (BTworks) {
                    SerialBT.print(levy_m); SerialBT.print(" "); SerialBT.println(pravy_m);
                }
                else {
                    fmt::print("levy: {}, pravy: {} \n ", levy_m, pravy_m );
                }
                if ((levy_m < 0) and (pravy_m < 0)) {
                    std::swap(levy_m, pravy_m);
                }
                    
                rkMotorsSetSpeed(levy_m, pravy_m);
            }
        delay(1);
    }

}

