# Ruthless RFID

<p align="center">
  <img src="https://github.com/TheZ0/Ruthless-RFID/assets/142558812/21e70e36-53e2-437b-ba30-4b78fc8ffa6c" />
</p>
# OverView
ISO/IEC 14443 compliant homemade RFID reader and writer for MIFARE Ultralight cards. Capable of writing stored data to new cards, storing read card data into flash memory as well as cloning from one card to another.

# Hardware Details
* RF Transceiver - NXP Semicondutors MFRC522 (QFN32)
* Microcontroller - ST Microelectronics STM32F401CBU6 (QFN48)
* OLED Display - Adafruit 5228
* Flash memory - Winbond W25N01GVZEIG (1 GBit) (8WSON)

Check out [Ruthless RFID Altium](https://github.com/TheZ0/Ruthless-RFID/tree/master/Ruthless%20RFID%20Altium) for PCB design and layout.

# Firmware Details
Firmware for the Ruthless RFID was created using the STM32CubeIDE. The Ruthless RFID is running FreeRTOS to separate each functionality shown on the home screen into it's own task as well as some background tasks such as listening for USB input.
## Drivers
* RF Transceiver - [MFRC.c](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Src/MFRC.c) , [MFRC.h](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Inc/MFRC.h)
* OLED Display - [OLED.c](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Src/OLED.c) , [OLED.h](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Inc/OLED.h)
* Flash memory - [W25N01GVZEIG.c](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Src/W25N01GVZEIG.c) , [W25N01GVZEIG.h](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Inc/W25N01GVZEIG.h)

## RuthlessFS
The Ruthless RFID uses a basic file system RuthlessFS to store and organise data read from a physical card or uploaded via USB by user. Each file occupies a 128KiB block (smallest erasable block size) and contains data such as PICC type, UID and raw contents. Checkout [ruthlessfs.c](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Src/ruthlessfs.c) , [ruthlessfs.h](https://github.com/TheZ0/Ruthless-RFID/blob/master/Firmware/Ruthless%20RFID/Core/Inc/ruthlessfs.h) for more info.

## Terminal
Ruthless RFID has a basic command line interface which the user can interact with using a terminal emulator such as PuTTy or similar. This interface supports common commands such as `ls`, `rm` and `cat` as well as two custom commands `pg` (program) for user to input their own file and `mod` for user to modify an existing file (See below).
![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/f85bfdae-1538-4a8e-ad04-c7e1751e3575)


![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/6d5c7efb-cadd-4794-9223-a820d66ced53)
