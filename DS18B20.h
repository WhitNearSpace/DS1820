#ifndef DS18B20_H
#define DS18B20_H

#include <mbed.h>
#include "OneWire.h"

#define DS18B20_FAMILY_CODE 0x28

class DS18B20{
    public:
        /**
         * Constructor 
         * **/
        DS18B20(OneWire *onewire, char ROM[8] = 0x00);

        /**
         * Overwrite the virtual void function in OneWire
         * 
         * Because this class comes with a set family code, there
         * should be no need for the user to access or set it 
         * **/
        void set_family_code(char family_code);

        char read_family_code();

        bool find_ROM();
        unsigned long long return_ROM();

        /**
         * Returns floating point value of current temperature in units decided by the user 
         * @param scale sets the units to Fahrenheit (if scale = 'F' or 'f') or Celcius in all other cases 
         * ***/
        float temperature(char scale);

        /***
         * Collect temperature data and write it to the scratchpad 
         * **/
        int convertTemp();

        /***
         *  Writes out the match ROM command and the specific ROM ID
         * 
         * NOTE: DOES NOT SEND OUT A RESET COMMAND 
         * **/
        void matchROM();


    private:
        char _ROM[8];
        char _family_code = DS18B20_FAMILY_CODE; 
        OneWire* _onewire; 
        char _RAM[9];

};

#endif