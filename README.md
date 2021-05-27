# DECO

## Introduction

Welcome to the Github page for the social robot under 50 euros. This robot is the result of a student project by the name PINNO, personal innovation project. Thsi robot is to be possibly used by the minor social robotics from Hogeschool Utrecht.

## What can the robot do?

DECO is a desk companion for peoplt with a desk job. The robot reminds the user to stay healthy and productive without being pushy. The robot reminds people to take breaks and drink enoug water. It can also inform the user of their current environment such as temperature. These are al posibilities within th erobot but the goal is that this robot can easily be programmed by someone with limited knowledge of technology. THis is so the robot can be used by the previously mentioned minor from the HU.

## Printing the robot
Each part of the robot can be printed seperatly or if you have a bigger printer, be placed in one STL. Before you print you do have to add supports. This can be done easily with ultimaker Cura.

## Necessary components to build this robot.

1. 3D printer
2. 2 * SG90 or SG92R servo motors, These both work the same but the SG92R ia a little stronger. 
    - [Amazon](https://www.amazon.nl/AZDelivery-Micro-Helikopter-Vliegtuigen-Inclusief/dp/B07CZ42862/ref=sr_1_1?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=sg90&qid=1616418465&sr=8-1)
3. Arduino nano 33 BLE SENSE
    - [Arduino store](https://store.arduino.cc/arduino-nano-33-ble-sense)
4. PIR sensor HC-SR501
    - [Amazon](https://www.amazon.nl/AZDelivery-HC-SR501-Detection-compatibel-Inclusief/dp/B07V6BY66P/ref=sr_1_3?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=PIR&qid=1616418981&sr=8-3)
5. OLED 0.96 display
    - [Amazon](https://www.amazon.nl/Jolicobo-OLED-weergavemodule-kijkhoek-doe-het-zelf-projectdisplay/dp/B07RNP3CMP/ref=sr_1_6?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=4+pin+oled&qid=1616419749&sr=8-6)
6. Piezo buzzer
    - [Amazon](https://www.amazon.nl/GLOGLOW-elektronische-zoemer-actieve-geluid/dp/B07F5LB8K9/ref=sr_1_1?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=P5WC6FGJ5HAV&dchild=1&keywords=piezo+buzzer&qid=1616419841&sprefix=piezo+%2Caps%2C175&sr=8-1)
6. Cables
    - Male to female 
    - female to female
7. Arduino compatible USB cable.
    - [Arduino store](https://store.arduino.cc/usb-cable-type-a-male-to-micro-type-b-male)
8. Soldering iron and necessary accesoires.

total cost from mamazon +- 63,18, You can probably get cheaper components from your local stores.  

## Documentation
For documentation run 
```
doxygen Doxyfile
```

## Models

### Fritzing
![Fritzing Model](https://github.com/MHovenkamp/PINNO/blob/main/images/Hardware_model.png)

Al other models can be found by opening software_models.html.

## Building and running
This robot is compiled and flashed by [PlatformIO](https://platformio.org/). Install platformIO core to work with this project. Then run the folowing commands in the directory. The directory will build the robot and flash to your arduino.

```
platformio run -t upload
```
If the right port cant be found change port to correct port with this command:
``` 
platformio run -t upload --upload-port <port>
```
You can find the correc tport with:
```
pio device list
```
After flashing the robot will instanly start working. If the robot loses power and is then reconnected it will restart the program.
