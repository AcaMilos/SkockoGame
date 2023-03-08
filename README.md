# SkockoGame-STM32L4_with_OLED

## About platform
Whole game is implemented on STM32L476RG microcontroller, with some aditional peripherals, like SSD1306 OLED display, 2x switch (buttons), 2x potentiometer, 2x 7-segment display.


## About Game
Game is about choosing the right combination of 6 characters.
You have 6 characters and 4 positions. On each position could be any character (they could repeat). 

<img width="400" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/GameSkocko.jpg?raw=true">

## System at work

<img width="200" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/platform_at_beginning_V1.0_small.jpg?raw=true">

<img width="200" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/platform_at_choosing_comb_V1.0_small.jpg?raw=true">

<img width="200" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/platform_result_V1.0_small.jpg?raw=true">

<img width="200" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/platform_final_result_V1.0_small.jpg?raw=true">


## About Hardware
On next photo is Block Schematic of System.

<img width="800" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/BlockSystemV1.0.PNG?raw=true">


## About software
That's bare metal system (without any RTOS). Whole project relies on STM32 HAL functions. All user software was written in C++, so generaly project is mix of C++, C, and assembly (a bit).

There on next photo you can see software architecture of this system.

<img width="1200" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/SW_Architecture_Main_program_V1.0.jpg?raw=true">

There on next photo you can see software architecture of ButtonPressedEventCallback function, which could not fit on above photo.

<img width="800" alt="SkockoGame" src="https://github.com/AcaMilos/SkockoGame-STM32L4_with_OLED/blob/master/Photos/SW_Architecture_ButtonPressedEventCallbackV1.0.PNG?raw=true">
