# Overview
The PCB and schematic for the Ruthless RFID were made in Altium Designer.

# Schematic Notes
![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/cffecdbb-6982-453f-9af9-9f20f65f9c16)

For designs which do not use the coil design used in this repository it is recommended that close attention is paid to the RF section of the MFRC522. Variations in coil design will have significant impact on the matching network C20 - C27. The filtering section (L1,C21) and (L2,C26) do not depend on coil design but should be chosen to have a resonant frequency of 13.56MHz). See [AN11564](https://www.nxp.com/docs/en/application-note/AN11564.pdf) from NXP for more details on the matching network.

# PCB Notes
![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/a523d39a-db99-4711-a31f-62c3c83c30a3)
![image](https://github.com/TheZ0/Ruthless-RFID/assets/142558812/18614355-a8df-41ef-87cf-a012d329495c)

The PCB is two layers with 3V3 plane on top and GND plane below. 

Important Layout Points
* Keep matching capacitors as close coil as possible to coil
* Ensure filter inductors L1,L2 are placed as close to MFRC522 as possible
* Keep return GND traces from matching and filtering components as short as possible
* Ensure no copper fills are above or below coil

# Manufacturing Details
Gerber files for the PCB can be found in Project Outputs/RFID.zip. 
