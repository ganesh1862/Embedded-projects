MSPM0G3507 Dual Application Bootloader
--------------------------------------

This is a simple UART-controlled bootloader for the MSPM0G3507 microcontroller.

The goal is straightforward: decide which application to boot (APP1 or APP2) based on a flag stored in flash memory. 

The bootloader checks this flag on startup and jumps to the corresponding app. 

You can also trigger a switch via UART by sending any data ending with 0xAA.

Core Features:
--------------
- Boots into APP1 or APP2 depending on a flash-stored flag (0x01 or 0x10).
- Receives UART trigger to toggle between apps.
- Validates the app’s vector table before jumping (SP in SRAM, PC in Flash).
- Green LED toggles while the bootloader is active (so you know it’s alive).
- UART prints messages for debugging.

Memory Map:
-----------
| Region     | Address     | Purpose                              |
|------------|-------------|--------------------------------------|
| Bootloader | 0x00000000  | Main bootloader code                 |
| APP1       | 0x00003C00  | Application slot 1                   |
| APP2       | 0x0000FC00  | Application slot 2                   |
| Config     | 0x0001BC00  | Flash-stored app flag (0x01 / 0x10)  |

How Switching Works:
--------------------
- On boot, the bootloader reads a config byte from flash:
    - 0x10 → Boot APP1 and write 0x01 back to config address
    - 0x01 → Boot APP2 and write 0x10 back to config address

UART Trigger:
-------------
- The bootloader waits for a UART command ending with 0xAA
- When it sees this, it flips the boot flag and jumps accordingly
- Any payload is fine, just make sure to end with 0xAA

Boot Decision Logic:
---------------------
1. Bootloader starts, toggles LED, prints status over UART.
2. Waits for UART trigger or just boots based on last config.
3. Reads config value at 0x1BC00.
4. If it says APP1 (0x10):
   - Rewrites flash to set next boot to APP2 (0x01)
   - Validates APP1 vector table
   - Jumps to APP1
5. Else if APP2 (0x01):
   - Rewrites flash to set next boot to APP1 (0x10)
   - Validates APP2 vector table
   - Jumps to APP2


This bootloader is lightweight, flexible, and a good base for more advanced OTA or secure boot systems.
