# BilgeMonitor-Firmware
Bilge Controller Software for my Sailboat
Has provisions for manual pumping, automatically pumping if the float switch is tripped, alarming if the high water float switch is tripped, and it also cyclically pumps every couple hours to keep the bilge bone dry.

all timed functions use interrupts and no delays. very short delays are only used for debounce on the switches.

Project uses an arduino and a relay sheild from seeed studio and a RULE Mate 2000GPH bilge pump
<br>
Relays = https://bit.ly/2wsoskD
<br>
Arduino = https://bit.ly/2rk4y6m
<br>
Bilge pump = https://bit.ly/2wsffsG  (pump has an integrated float switch and controller, but it sucks...)

Developed in Atmel Studio but includes the arduino core core so I can do things like Serial.print and if anyone want's to modify it for their use it'll be easy to add arduino syntax like digitalwrite and such.

Wiring diagram and connection info is written in the comments in main.cpp

Plan to add logging capabillities and alerts based on any changes in bilge frequency in the future.
