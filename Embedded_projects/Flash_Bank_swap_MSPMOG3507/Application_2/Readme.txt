MSPM0G3507 - APP2 Firmware
--------------------------

Overview:
---------
This firmware represents **Application 2 (APP2)** in a dual-app bootloader system on the MSPM0G3507 microcontroller.

- Blink a red LED to show it's running
- Periodically send debug messages over UART
- Wait for a UART command ending in 0xAA
- On trigger, validate and jump to the bootloader (at 0x00000000)
- If valid, it jumps to the bootloader
- If not, it prints a debug error

UART Settings:
- Baudrate 115200
- Trigger command ends with 0xAA

This setup is useful for:
- Testing dual-app bootloaders
- Firmware update switching
- Field-upgradable systems


