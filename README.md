
This is the documentation for a submersible data logger developed in association with the Cal Poly Center for Coastal Marine Sciences.
Created by: Miloh Padgett, Joshua Robinson, Yale Yun, Jon Abraham

<img width="527" alt="exploded view for pasting" src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763584/aeaca087-7689-42aa-aedb-910fcd89017d">

## Bulkhead Connector's Outputs
![LoggerPinout](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/940dee5c-c237-4c81-9d3d-c5ae6e471af8)

## Part Names
The following names are used to refer to the various components of the device.
![Annotated Logger](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/66269046/95221804-980e-4200-a380-22e8f41dcf62)

Reference image for the NUCLEO board.

![NUCLEOreference](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/e33c9bdc-e902-4284-94de-c4abfe10d4e0)

Arrow on the 4 pin and 2 pin Dupont connectors that will be used to orient the cable when connecting the PCB to the NUCLEO board.

![Dupontarrow](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/dc4b309f-4018-4d4e-ad8e-a0d28fd34077)

## Quick Start Guide
### Configuring a Device
#### Hardware Setup
1. Make sure the switch at the bottom of the PCB is pushed towards the middle of the board. 
2. Disconnect the battery pack from the 2-pin JST header at the bottom of the PCB and add new batteries. Use 10x 3V Lithium AA Batteries.  
3. Insert SD card in the SD card holder. 
4. Secure the battery pack near the bottom of the internal bracket. 
5. Check that the sensor cable is connected to the 4-pin JST header at the top of the PCB. 
6. Reconnect the battery pack to the 2-pin JST header.
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/97e0ac89-2c3f-4c96-9450-5a9353c0e688" width="400">

7. Check that jumpers are placed at the locations on the front side of the NUCLEO board. The jumpers should be on the highlited pin locations in the left image. The jumpers at the top left and right corner of the board (CN11 & CN12) are spares and can be used to replace the jumpers highlighted with yellow boxes in the image on the right. 
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/d7b20bbb-f6e2-482a-9373-1e6521d29a1d" width="600">
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/85c962e0-a4a5-4f83-8a10-1d1ebcdf18b6" width="381">

8. Looking at the back side of the NUCLEO board make sure the cables are plug in and match the image on the left. If cables are unplugged refer to 8a through 8d. 

Note: Image on the right shows a top down view of the connections. Cables should be attached to the bottom of the board. Arrows in the image on the left match the arrows on the Dupont connector, which are used to make sure the cable is plug in with the right orientation. 

<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/02d208a3-633c-43ee-9a60-3c697cd5ea6c" width="580">
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/45d1dfcb-a8fc-45f6-a357-e779802db509" width="391">

8a. The cable labeled UART has a 4 pin and a 1 pin connector. Look at the back side of the NUCLEO board while keeping the same orientation of the top and bottom edges as in the reference image above. Attacth the 4 pin connector so that the socket with the arrow connects to the pins on the left that is 9 up from the bottom left most pin. Connect the 1 pin connector to the pin that is 3 up and 1 to the right of the bottom left most pin. 
![IMG_1066](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/1a42ab68-90b1-4f05-b52a-ee396e794ce4)

8b. The cable labeled SD 4+1 has a 4 pin and a 1 pin connctor. With the same orientation as before, attach the 4 pin connector so that the socket with the arrow connects to the pin that is 1 below and 1 to the right of the top left most pin. Connect the 1 pin connector to the pin that is 5 below and 1 to the right of the top left most pin. 
![IMG_1065](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/c91e9097-2661-466b-945b-83469c555806)

8c. The cable labeled SD 2 should be attached so that the socket with the arrow connects to the pin that is 1 below the top right most pin. 
![IMG_1064](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/5f4a0942-3adf-4ae4-acaf-fdf1271b83dc)

8d. The Cable labeled MCU PWR has a 4 pin connector. Attach the connector so that the socket with the arrow connects to the pin that is 7 above and 1 to the left of the bottom right most pin. 
![IMG_1063](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/f5287e13-8505-4a75-89d3-5d143aa2a172)


9. Connect CN4 to CN7 using the diagram below. This connects the ST-Link to the Microcontroller, which allows the user to communicate to the microcontroller with their computer. These must be unplugged before closing the datalogger.
![GetImage (5)](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/d42c1a67-6579-4f23-9945-6ed6190e18ae)


10. Connect CN3 TX to the pin third from the top on the outter edge of CN10 (CN10 pin 33) and CN3 RX to thrid pin from the bottom on the outter edge of CN10 (CN10 pin 21). These must be unplugged before closing the datalogger.
![GetImage (6)](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/9949428b-ee7f-43f5-865d-1d6cebd767a1)


11. Connect the NUCLEO board (CN1 mini-USB port) to a computer using a right angle mini-USB to USB-A cable.
12. Give the NUCLEO board power by moving the switch on the PCB to the outer edge (away from the blue component).
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/7dc14263-69ff-46ab-ace1-7c017d92c853" width="500">

#### Software Setup
1. Ensure the ST-link is connected, and the board is powered on.
2. Download the GUI: <a id="raw-url" href="https://raw.githubusercontent.com/MilohPadgett/SubmersibleDataLogger/master/GUI.exe">Download FILE</a>
3. Run the GUI executable file
4. Click the ‘Select COM Port’ dropdown menu and choose the port that the STM32 is plugged into. If no ports appear, try pressing the ‘Refresh COM’ button and ensuring that the STM is properly connected. A light should be on in the top corner of the STM.
5. If the GUI stops responding or crashes, please ensure the wiring is configured properly and the power is turned on.
6. Once the data logger is connected, the GUI will notify you and you can select the ECO from the ‘Select Sensor’ dropdown menu.
7. A new window will appear that will prompt for time between samples, duration of samples, and start delay. You must press the set buttons on the left side to lock in your values. The changes will be reflected on the right. 
8. You can choose to press the ‘Calculate Lifetime’ button, and an expected lifetime in days will appear in the bottom right corner.
9. After you are done setting your desired configurations, press the exit button in the left hand corner.
10. You will be returned to the original page, where you can press deploy to upload your settings to the data logger.
11. A small red LED on the Blue SD card reader module will turn on. This signified that the logger received the configuration and is setting up. Do NOT continue until this light turns off again. 
12. Once the Red LED turns off, the data logger will start its autonomous cycle. 
13. If you have chosen to set a start delay, the logger will enter this delay, otherwise it will start collecting data and writing it to the SD card.
14. At this point, you may unplug the USB cable, unplug the ST-link connecter cables, and proceed with closing the data logger.


### Preparing the Enclosure
1. Use a rag or paper towel to dry off the outer housing and end plug.
2. Wipe any moisture and dirt off of the bored surface of the outer housing and end plug using lint-free wipes. ![Cleaning Surfaces](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763584/d7c1d028-3ae6-4cd0-8e5c-78a830bd8ff9)
3. Check the O-rings for cracking, tears, and excessive twisting. If there is damage, remove both O-rings and replace with new Nitrile ASTM No. 150 O-Rings
![O-Ring Removal_Replacement](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763584/6460451c-9859-448b-9323-10605eade3a3)
4. Squeeze out a pea-sized amount of Silicone Grease onto your finger and distribute in small dots on the surface of the installed O-rings
![Grease](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763584/e39cbb22-a78f-461a-8302-e6f2dcf8e26e)
![Greasing O-rings](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763584/9c1f8c1d-1eb3-4243-ae99-df2761c3a2e1)
6. Use your finger to evenly spread the grease on the O-rings
7. Use a lint-free wipe to clean off the O-rings; the first coat allows the grease to pick up any small particulates before inserting the plug into the outer enclosure.
8. Repeat steps 4 and 5. Do not wipe off the grease the second time.
9. Insert the bracket and end plug into the outer housing, making sure to align the markings on the end plug and outer enclosure (to align the shoulder screw with the tapped hole). Apply pressure until the larger diameter of the end plug is in contact with the flat face of the outer housing.
![Shoulder Screw](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763584/457de7c9-c903-421a-9464-05824a9ba66d)
10. Place the shoulder screw into the hole on the outer enclosure and use a 5/64" drive hex key to thread the screw into the end plug.
11. Insert the dummy plug OR properly potted 6-pin cable into the 6-pin female bulkhead connector on the datalogger. CAUTION: Only apply pressure to the pins when they are in line with the bulkhead connector, they are easily bent and damaged.

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

### Potential Issues/Future Recommendations
•	Redesign of the internal bracket for ease of assembly and robustness (i.e. impact resistance to dropping).

•	Add software features to give sensors shutdown commands rather than just cutting power. 

•	Expand GUI to have more configuration capabilities for more sensors. 

•	Exploration of a flanged elastic seal design for ease of disassembly.

•	Port expansion to connect with more than one sensor per unit.

•	Wireless data transmission functionality.

•	Long term fully integrated deployment test (1 month in ocean environment).

•	Program and verify compatibility with other common sensors (Sea-Bird 37 SIP CTD, Aanderaa Oxygen Optode).

•	Selection of a more robust development board or custom PCB to simplify power optimization (instead of scoring STM and replacing onboard voltage regulator).

