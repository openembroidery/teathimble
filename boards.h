// Examples of configurations for other boards 
// The pins names should be AIOx (Analog Input Output) or DIOx (Digital Input Output), with x a number.
// For those names check proper arduio_xxx.h files. AVRlib names will work also anyway...
// TODO: all other boards configurations needs to be checked and fixed
#ifdef MOTHERBOARD
    #if MOTHERBOARD==1
    // ramps 1.4
    #if !(defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__))
        #error Oops! Make sure you have 'Arduino 1280 / Mega 2560' selected from the 'Tools -> Boards' menu.
    #endif
    #define X_STEP_PIN               DIO54
    //PF0     // 54
    #define X_DIR_PIN                DIO55
    //PF1    // 55
    //#define X_MIN_PIN                DIO3
    //PE5    // 3
    //#define X_MAX_PIN                DIO2
    //PE4    // 2
    #define X_ENABLE_PIN             DIO38
    //PD7    // 38
    #define X_INVERT_DIR
    #define X_INVERT_MIN
    #define X_INVERT_MAX
    #define X_INVERT_ENABLE
    
    #define Y_STEP_PIN               DIO60
    //PL3    // 60
    #define Y_DIR_PIN                DIO61
    //PL1    // 61
    #define Y_MIN_PIN                DIO14
    //PJ1    // 14
    //#define Y_MAX_PIN                DIO15
    //PJ0    // 15
    #define Y_ENABLE_PIN             DIO56
    //PF2    // 56
    #define Y_INVERT_DIR
    #define Y_INVERT_MIN
    #define Y_INVERT_MAX
    #define Y_INVERT_ENABLE
    
    /*#define Z_STEP_PIN             DIO46
     * //PL3     * // 46
     #define Z_DIR_PIN               DIO48
     //PL1     // 48
     #define Z_MIN_PIN               DIO18
     //PD3     // 18
     #define Z_MAX_PIN               DIO19
     //PD2     // 19
     #define Z_ENABLE_PIN            DIO62
     //PK0     // 62
     #define Z_INVERT_DIR
     #define Z_INVERT_MIN
     #define Z_INVERT_MAX
     #define Z_INVERT_ENABLE
     
     #define E_STEP_PIN              DIO26
     //PA4     // 26
     #define E_DIR_PIN               DIO28
     //PA6     // 28
     #define E_ENABLE_PIN            DIO24
     //PA2     // 24
     #define E_INVERT_DIR
     #define E_INVERT_ENABLE
     
     // pwms: 10, 8, led 13, analogin: 13,14
     */
    #endif
    #if MOTHERBOARD==2
    // cnc shield v3
    #ifndef __AVR_ATmega328P__
        #error Oops! Make sure you have 'Arduino Duemilanove w/ ATMega328' selected from the 'Tools -> Boards' menu.
    #endif
    #define X_STEP_PIN               DIO2
    //PD2 // 2
    #define X_DIR_PIN                DIO5
    //PD5 // 5
    
    //#define X_MIN_PIN                DIO9
    //PB1 9 
    //#define X_MAX_PIN                
    #define X_ENABLE_PIN             DIO8
    //PB0   // 8
    #define X_INVERT_DIR
    #define X_INVERT_MIN
    #define X_INVERT_MAX
    #define X_INVERT_ENABLE
    
    #define Y_STEP_PIN               DIO3
    //PD3   //3
    #define Y_DIR_PIN                DIO6
    //PD6  //6
    #define Y_MIN_PIN                DIO10
    //PB2 //10
    //#define Y_MAX_PIN                
    #define Y_ENABLE_PIN             DIO8
    //PB0  //8
    #define Y_INVERT_DIR
    #define Y_INVERT_MIN
    #define Y_INVERT_MAX
    #define Y_INVERT_ENABLE
    
    /*#define Z_STEP_PIN              DIO4
     //PD4 // 4
     #define Z_DIR_PIN                DIO7
     //PD7   //7
     #define Z_MIN_PIN                DIO11 
     // PB3  //11
     #define Z_MAX_PIN                
     #define Z_ENABLE_PIN             DIO8
     //PB0
     #define Z_INVERT_DIR
     #define Z_INVERT_MIN
     #define Z_INVERT_MAX
     #define Z_INVERT_ENABLE
     
     #define E_STEP_PIN               DIO12
     //PB4  //12
     #define E_DIR_PIN                PB6
     //PB6  //13
     #define E_ENABLE_PIN             
     #define E_INVERT_DIR
     #define E_INVERT_ENABLE
     */
    #endif
#endif
