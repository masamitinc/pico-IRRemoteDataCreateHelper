#include <Arduino.h>

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

void setup() {
    Serial.begin(115200);

    delay(5000);	// For Raspberry Pi Pico bootup

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));

#if 0
// Samsung TV Power On
    #define POWER         0xE0E040BF            //from IRrecvDemo, Ken Shirriff
    #define SAMSUNG_BITS_this  32 
    IrSender.sendSAMSUNG(POWER, SAMSUNG_BITS_this);

// LG TV Power On
    #define LG_TV_POWER_ON 0x20DF10EF
    IrSender.sendNEC(LG_TV_POWER_ON, 32);
    IrSender.sendSony(0xA90, 12);

// TCL TV Power On
    #define TCL_ADDR 0xC7EA
    #define TCL_PWR_CMD1 0x17
    #define TCL_PWR_CMD2 0x97
    //Serial.println("TCL_PWR_CMD1");
    //IrSender.sendNEC2(TCL_ADDR, TCL_PWR_CMD1, 0);
    Serial.println("TCL_PWR_CMD2 - gap=37350us");
    IrSender.sendNEC2(TCL_ADDR, TCL_PWR_CMD2, 0);

    //Hisense TV Power On
    //IrSender.sendNEC(0x40, 0x12,1);
    IrSender.sendNEC(0xFDB04F, 32); //HISENSE Power toggle 

    // Sharp TV Power On
    IrSender.sendSharp(0x128, 0x16, 1);
#endif

    //Clear raw buffer
    for (int i = 0; i < RAW_BUFFER_LENGTH; i++)
    {
        IrSender.rawlen = 0;
        IrSender.rawbuf[i] = 0;
    }

////////// Example to send RC6 protocol data (e.g. for Philips TV)
    extern uint8_t sLastSendToggleValue;	//Default = 1
    sLastSendToggleValue = 0;   //1: Normal, 0: Toggle
    IrSender.sendRC6(0x0, 0xC, 1, 1);    //Philips TV Power

#define IR_TIME_DIVIDER (444)

////////// Example to send RC6 protocol data (e.g. for Philips TV)

#if 1

    Serial.print("Num= ");
    Serial.println(IrSender.rawlen);
    for (int i = 0; i <= IrSender.rawlen; i++)
    {
        Serial.print("\t");
        Serial.print(IrSender.rawbuf[i] / IR_TIME_DIVIDER);
        Serial.print(",\t// ");
        Serial.print(IrSender.rawbuf[i]);
        Serial.println((IrSender.rawOn[i] == 1) ? " on" : " off");
        if (i==0) Serial.println("// -=-=-=-=-=- CUT HERE -=-=-=-=-=-");
    }
#endif
}

void loop() {
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (IrReceiver.decode()) {

        /*
         * Print a summary of received data
         */
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print extended info
            IrReceiver.printIRResultRawFormatted(&Serial, true);

            IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
        } else {
            IrReceiver.resume(); // Early enable receiving of the next IR frame

            IrReceiver.printIRResultShort(&Serial);
            IrReceiver.printIRSendUsage(&Serial);
        }
        Serial.println();

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
            Serial.println(F("Repeat received. Here you can repeat the same action as before."));
        } else {
            if (IrReceiver.decodedIRData.command == 0x10) {
                Serial.println(F("Received command 0x10."));
                // do something
            } else if (IrReceiver.decodedIRData.command == 0x11) {
                Serial.println(F("Received command 0x11."));
                // do something else
            }
        }
    }
}