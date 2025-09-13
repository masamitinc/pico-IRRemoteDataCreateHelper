## Instruction

* Description:
  - Show IR on/off timing after using IrSender.sendXXX()
 
* Environment
  - Raspberry Pi Pico W
  - VSCode with Pltform.io extension

### Steps

1. Find the Send command and parameter
    - Ex.) `IrSender.sendRC6(0x0, 0xC, 1, 1);    //Philips TV Power`

2. `#define IR_TIME_DIVIDER (1)`  
 for the first time, and will find the proper one.

3.  Upload and Monitor
    ```
    Num= 39
            65535,  // 65535 off
            // -=-=-=-=-=- CUT HERE -=-=-=-=-=-
            2664,   // 2664 on
            888,    // 888 off
            444,    // 444 on
            888,    // 888 off
            444,    // 444 on
            444,    // 444 off
            444,    // 444 on
            ...
    ```

4. Find "IR_TIME_DIVIDER" (Guess from GCD ... Greatest Common Divisor)
    - Ex) 444 in this case

5. `#define IR_TIME_DIVIDER (444)`
6. Upload and Monitor
    ```
    Num= 39
            147,    // 65535 off
    // -=-=-=-=-=- CUT HERE -=-=-=-=-=-
            6,      // 2664 on
            2,      // 888 off
            1,      // 444 on
            2,      // 888 off
            1,      // 444 on
            1,      // 444 off
            1,      // 444 on
            1,      // 444 off
            ...
    ```


7. Use it for TX IR data
    ```c
    const S_TVREMOTE_IR_DATA sTvRemote_PhilipsPower =
    {
        sizeof(sTvRemote_PhilipsPower_data) / sizeof(sTvRemote_PhilipsPower_data[0]),	// len
        444,	// factor
        3,		// repeat
        93,		// sleep_ms_between_repeats
        &sTvRemote_PhilipsPower_data[0],
        ///
        sizeof(sTvRemote_PhilipsPower_data_toggle) / sizeof(sTvRemote_PhilipsPower_data_toggle[0]),	// len_toggle
        &sTvRemote_PhilipsPower_data_toggle[0]
    };
    ```


## Pin connections

1. Raspberry Pi Pico / Pico W
    - IR_RECEIVE_PIN  15
    - IR_SEND_PIN     16
    - https://pico.pinout.xyz/
    - https://picow.pinout.xyz/

2. Arduino Uno R3
    - IR_RECEIVE_PIN  2
    - IR_SEND_PIN     3
    - https://docs.arduino.cc/resources/pinouts/A000066-full-pinout.pdf

