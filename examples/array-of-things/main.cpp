#include <mbed.h>
#include "OneWire.h"
#include "DS18B20.h"

// IDs for connected DS18B20s 
// Note that these IDs are bit-sequential with the LSB coming first 
char thing22[8] = {0x28, 0x87, 0x87, 0xF9, 0x0C, 0x00, 0x00, 0xB2};
char thing21[8] = {0x28, 0x22, 0xD6, 0xF9, 0x0C, 0x00, 0x00, 0x5D};
char thing51[8] = {0x28, 0xB0, 0x3B, 0xF9, 0x0C, 0x00, 0x00, 0xB4};
char thing69[8] = {0x28, 0x5D, 0xDA, 0xF9, 0x0C, 0x00, 0x00, 0xC0};

OneWire onewire(p9); // Initialize OneWire object

// Initialize DS18B20 objects and assign them their IDs
DS18B20 ds22(&onewire, thing22);
DS18B20 ds21(&onewire, thing21);
DS18B20 ds51(&onewire, thing51);
DS18B20 ds69(&onewire, thing69);


int main() {
    while(1){
        ThisThread::sleep_for(1s);
        ThisThread::sleep_for(100ms);

        ds22.convertTemp(); // Write temperature to the scratchpad 
        float temp = ds22.temperature('f');
        printf("The current temperature is: %.3f F\n", temp);
        float temp22 = ds22.temperature('f');
        ThisThread::sleep_for(100ms);

        ds21.convertTemp(); // Write temperature to the scratchpad 
        float temp21 = ds21.temperature('f');
        ThisThread::sleep_for(100ms);

        ds51.convertTemp(); // Write temperature to the scratchpad 
        float temp51 = ds51.temperature('f');
        ThisThread::sleep_for(100ms);

        ds69.convertTemp(); // Write temperature to the scratchpad 
        float temp69 = ds69.temperature('f');

        printf("Thing 22 reports: %.3f F\n", temp22);
        printf("Thing 21 reports: %.3f F\n", temp21);
        printf("Thing 51 reports: %.3f F\n", temp51);
        printf("Thing 69 reports: %.3f F\n", temp69);
        printf("\n");
    }
} 