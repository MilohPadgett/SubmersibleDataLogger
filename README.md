
This is the documentation for a submersible data logger developed in association with the Cal Poly Center for Coastal Marine Sciences.
Created by: Miloh Padgett, Joshua Robinson, Yale Yun, Jon Abraham

<img width="527" alt="exploded view for pasting" src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763584/aeaca087-7689-42aa-aedb-910fcd89017d">


## Part Names
The following names are used to refer to the various components of the device.
![Annotated Logger](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/66269046/95221804-980e-4200-a380-22e8f41dcf62)
![Annotated Logger Pt 2 (1)](https://github.com/MilohPadgett/SubmersibleDataLogger/assets/66269046/e4978e3d-48d6-43db-b917-a5ae3ea91683)

## Quick Start Guide
### Configuring a Device
#### Hardware Setup
1. Make sure the switch at the bottom of the PCB is pushed towards the middle of the board. 
2. Disconnect the battery pack from the 2-pin JST header at the bottom of the PCB and add new batteries. Use 10x 3V Lithium AA Batteries.  
3. Insert SD card in the SD card holder. 
4. Secure the battery pack near the bottom of the internal bracket. 
5. Check that the sensor cable is connected to the 4-pin JST header at the top of the PCB. 
6. Reconnect the battery pack to the 2-pin JST header.
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/fd66ff42-4c03-42e4-8842-09b519287379" width="400">

7. Check jumper locations on the top of the NUCLEO board.
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/d7b20bbb-f6e2-482a-9373-1e6521d29a1d" width="600">
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/85c962e0-a4a5-4f83-8a10-1d1ebcdf18b6" width="381">

8. Check cable connections on the bottom of the NUCLEO board.

Note: Image on the right shows a top down view of the connections. Cables should be attached to the bottom of the board.

<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/02d208a3-633c-43ee-9a60-3c697cd5ea6c" width="580">
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/45d1dfcb-a8fc-45f6-a357-e779802db509" width="401">

9. Connect the ST-Link to the NUCLEO board using female-female Dupont wires.
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/3e069f00-94a0-49cd-b0ba-054c5c2f24b5" width="500">

10. Connect CN3 TX to PC5 and CN3 RX to PC4.
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/0b2118a7-8c04-46c2-b1b7-f28a7499768d" width="500">

11. Connect the ST-Link(CN1 mini-USB port) to a computer using a mini-USB to USB-A cable.
12. Give the NUCLEO board power by moving the switch on the PCB to the outer edge (away from the blue component).
<img src="https://github.com/MilohPadgett/SubmersibleDataLogger/assets/135763514/7dc14263-69ff-46ab-ace1-7c017d92c853" width="500">

#### Software Setup
1. Plug the STM32 into your PC using the USB cable.
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
14. At this point, you may unplug the USB cable, and proceed with closing the data logger.


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
