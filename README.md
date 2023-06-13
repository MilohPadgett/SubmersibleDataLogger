# SubmersibleDataLogger
This is the documentation for a submersible data logger developed in association with the Cal Poly Center for Coastal Marine Sciences.
Created by: Miloh Padgett, Joshua Robinson, Yale Yun, Jon Abraham

## Part Names
The following names are used to refer to the various components of the device.
![Annotated Logger](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/66269046/95221804-980e-4200-a380-22e8f41dcf62)
![Annotated Logger Pt 2 (1)](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/66269046/e4978e3d-48d6-43db-b917-a5ae3ea91683)

## Quick Start Guide
### Configuring a Device
1. Make sure the switch at the bottom of the PCB is pushed towards the middle of the board. 
2. Place new batteries in the battery pack. Use 10x 3V Lithium AA Batteries.  
3. Insert SD card in the SD card holder(PCB bottom left). 
4. Velcro the battery pack near the bottom of the bracket. 
5. Connect the sensor cable to the 4-pin JST header at the top of the PCB. 
6. Connect the battery pack to the 2-pin JST header at the bottom of the PCB.
7. Check jumper locations on the top of the NUCLEO board.
8. Check cable connections on the bottom of the NUCLEO board.
9. Connect the ST-Link to the NUCLEO board using female-female Dupont wires.
10. Connect CN3 TX to PC5 and CN3 RX to PC4.
11. Connect the ST-Link(CN1 mini-USB port) to a computer using a mini-USB to USB-A cable.
12. Give the NUCLEO board power by moving the switch on the PCB to the outer edge. 

### Preparing the Enclosure
1. Use a rag or paper towel to dry off the outer housing and end plug.
2. Wipe any moisture and dirt off of the bored surface of the outer housing and end plug using lint-free wipes.
3. Check the O-rings for cracking, tears, and excessive twisting. If there is damage, remove both O-rings and replace with new Nitrile ASTM No. 150 O-Rings
(insert picture of removing and replacing O-rings)
4. Squeeze out a pea-sized amount of Silicone Grease onto your finger and distribute in small dots on the surface of the installed O-rings
(insert picture of amount of grease needed)
5. Use your finger to evenly spread the grease on the O-rings
6. Use a lint-free wipe to clean off the O-rings; the first coat allows the grease to pick up any small particulates before inserting the plug into the outer enclosure.
7. Repeat steps 4 and 5. Do not wipe off the grease the second time.
8. Insert the bracket and end plug into the outer housing, making sure to align the markings on the end plug and outer enclosure (to align the shoulder screw with the tapped hole). Apply pressure until the larger diameter of the end plug is in contact with the flat face of the outer housing.
(insert picture of aligning markings and fully installed)
9. Place the shoulder screw into the hole on the outer enclosure and use a 5/64" drive hex key to thread the screw into the end plug.
10. Insert the dummy plug OR properly potted 6-pin cable into the 6-pin female bulkhead connector on the datalogger. CAUTION: Only apply pressure to the pins when they are in line with the bulkhead connector, they are easily bent and damaged.

### Retreiving the Data
1. Before removing the shoulder screw and outer housing, use a rag or paper towel to dry off the datalogger
2. Use a 5/64" drive hex key to remove the shoulder screw
3. Pull the outer housing off of the end plug and internal bracket. This will require a lot of force; if it is too difficult to remove by hand then clamping in a vise or using allen keys to pry between the end plug and outer housing may help. 
4. Use a rag or paper towel to dry off the rest of the datalogger.
5. Remove the microSD card from the PCB.
6. Plug into your computer and retrieve the desired files. 

### Storing the Datalogger
1. Ensure that all components are dry.
2. Unplug the JST connector on the PCB that connects to the Battery Pack.
3. Remove all 10 batteries and store in a dry, insulated space. 
