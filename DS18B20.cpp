#include "DS18B20.h"
#define CONVERT_T 0x44

DS18B20::DS18B20(OneWire *onewire, char ROM[8]){
    for(int i = 0; i < 8; i++){
        _ROM[i] = ROM[i];
    }
    _onewire = onewire;
}

void DS18B20::set_family_code(char family_code){ 
    // Do nothing for now, maybe display error 
}

char DS18B20::read_family_code(){
    return _family_code; 
}

bool DS18B20::find_ROM(){
    bool result;
    result = _onewire->ROM_search();
    return result; 
}

unsigned long long DS18B20::return_ROM(){
    unsigned long long name = _onewire->whoAmI();
    return name; 
}


float DS18B20::temperature(char scale) {
    int reading; // Initial integer/bit reading 
    float answer; // Floating point return value
    if(_onewire->reset()){ // Ensure that reset command is sent without error 
        if(_ROM[0] != 0){ // If the device has an assigned ROM, the user can read temp data
            matchROM(); // Select the particular device 
            _onewire->writeByte(0xBE);  // Send out the read scratchpad command
            // Read the scratchpad byte by byte 
            for(int i=0;i<9;i++) {
                _RAM[i] = _onewire->readByte();
            }
            reading = (_RAM[1] << 8) + _RAM[0]; // Collect temperature bytes from the scratch pad 
            if (reading & 0x8000) { // Since the reading is in 2's compliment form, if the first bit is 1, the number is negative 
                reading = 0-((reading ^ 0xffff) + 1); // Convert from 2's compliment to signed integer
            }
            answer = reading + 0.0; // Convert to floating point and assign to the return variable
            answer = answer / 16.0f; // Scale the answer 
            // Check the units of return 
            if ((scale=='F') || (scale=='f')){
                // Convert to deg F
                answer = answer * 9.0f / 5.0f + 32.0f;
            }
        }else{ // If the device does not have a set ROM
            // maybe set off some error code??
            return 0; 
          
        }
    }else{ // If the reset command is not successfully sent 
        answer = 0; 
        // return some error code 
    }
    return answer;
}
 

int DS18B20::convertTemp(){
    bool answer = 0; // Boolean controls the amount of time given to the DS18B20 to collect & write temp data
    if(_onewire->reset()){ // Only proceed with communication if a reset is completed 
        matchROM();
        _onewire->writeByte(CONVERT_T);  // perform temperature conversion
        while(!answer){
            answer = _onewire->readBit();
        }
        /**
         * Explaining the science behind the while loop:
         * 
         * According to the DS18B20 documentation, if the DS18B20 is
         * powered by an external supply, the master can issue read
         * time slots after the Convert T command and the DS18B20
         * will respond by transmitting a 0 while the temperature
         * conversion is in progress and a 1 when the conversion is
         * done. In parasite power mode this notification technique
         * CANNOT be used since the bus is pulled high by the strong
         * pullup during the conversion.
         * 
         * Thus, we wait to move on from the function till the DS18B20
         * has had enough time to write the temp data to the scratchpad. 
         * This time is monitored by waiting for the DS18B20 to send a '1' 
         * to the master. 
         * **/
    }
    return answer;
}

void DS18B20::matchROM(){
    _onewire->writeByte(MATCH_ROM); // Send match command
    // Read out the ROM byte by byte 
    for(int i = 0; i < 8; i++){
        _onewire->writeByte(_ROM[i]);
    }
}