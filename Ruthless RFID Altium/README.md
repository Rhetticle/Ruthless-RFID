# Overview
The PCB and schematic for the Ruthless RFID were made in Altium Designer.

# Schematic Notes
![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/c2c0b8fc-0632-4636-b973-8d82d2e63900)


For designs which do not use the coil design used in this repository it is recommended that close attention is paid to the RF section of the MFRC522. Variations in coil design will have significant impact on the matching network C20 - C27. The filtering section (L1,C21) and (L2,C26) do not depend on coil design but should be chosen to have a resonant frequency of 13.56MHz). See [AN11564](https://www.nxp.com/docs/en/application-note/AN11564.pdf) from NXP for more details on the matching network.

# PCB Notes
![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/15cc10d7-203e-4f68-a9c4-bb0f1bdf2e0d)
![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/860eb617-37b9-4931-bea5-412b15a24067)



The PCB is two layers with 3V3 plane on top and GND plane below. 

Important Layout Points
* Keep matching capacitors as close coil as possible to coil
* Ensure filter inductors L1,L2 are placed as close to MFRC522 as possible
* Keep return GND traces from matching and filtering components as short as possible
* Ensure no copper fills are above or below coil

# Manufacturing Details
Gerber files for the PCB can be found in Project Outputs/RFID.zip. 
