# BilgeMonitor-Firmware
Bilge Controller Software for my Sailboat
Has provisions for manual pumping, automatically pumping if the float switch is tripped, alarming if the high water float switch is tripped, and it also cyclically pumps every couple hours to keep the bilge bone dry.

all timed functions use interrupts and no delays. very short delays are only used for debounce on the switches.

Project uses an arduino and a relay sheild from seeed studio and a RULE Mate 2000GPH bilge pump

Developed in Atmel Studio but I am using the arduino core so I can do things like Serial.print. 
Wiring diagram and connection info is written in the comments in main.cpp

Plan to add logging capabillities and alerts based on any changes in bilge frequency in the future.
