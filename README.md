# DECO

## Introduction

Welcome to the Github page for the social robot under 50 euros. This robot is the result of a student project by the name PINNO, personal innovation project. This robot is to be possibly used by the minor social robotics from Hogeschool Utrecht.

## What can the robot do?

DECO is a desk companion for people with a desk job. The robot reminds the user to stay healthy and productive without being pushy. The robot reminds people to take breaks and drink enough water. It can also inform the user of their current environment such as temperature. These are al possibilities within the robot but the goal is that this robot can easily be programmed by someone with limited knowledge of technology. THis is so the robot can be used by the previously mentioned minor from the HU.

## Printing the robot
Each part of the robot can be printed separated or if you have a bigger printer, be placed in one STL. Before you print you do have to add supports. This can be done easily with Ultimaker Cura.

## building the robot

The head:
The head consists of two parts: a roundish ball, the lower head and a dome for the face. 
1. Place the micro lidar and the oledscreen in the upper face and make sure that they are stuck using either a little bit of glue or some electricity tape.
2. place on of the servo motors in the g0 degree position, THsi can be done by connecting to the Arduino and running the return_to_start_pos command. Glue the neck horizontal to the servo motor making sure that there is a 90 degree angle between the motor and the neck.
3. place the servo with the neck part in the slot in the lower body and attach with some glue. 
4. Route all the cables for the components through the whole in the bottom of the lower body

The body:
1. Stick the servo through the bottom hole of the upper body. And turn the servo to 90 degrees. glue the vertical neck part to the servo with the attachment part diagonal to the servo. 
2. Attach the PIR sensor through the whole and attach with taper or glue. place the arduino in the bottom lower body and connect al the cables as shown in the Fritzing. 

Attachment
Connect the upper head to the lower head with some glue making sure the head looks up and down if the servo is moved. 
Slide the two neck pieces to each other attaching the robot together, make sure al the wires can get to the arduino with some space for movement. glue the lower and upper body together after connecting the usb cable into the arduino making sure the PIR sensor looks forward.

Voila! Robot. 
![DECO](https://github.com/MHovenkamp/DECO/blob/interface_development/images/DECO.jpg)



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
8. Soldering iron and necessary accessories.

total cost from amazon +- 63,18, You can probably get cheaper components from your local stores.  

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
This robot is compiled and flashed by [PlatformIO](https://platformio.org/). Install platformIO core to work with this project. Then run the folowing commands in a terminal in the directory. The directory will build the robot and flash to your arduino. PlatformIO will install all libraries and updates by itself. If something does not work try updating the libraries through platformIO trough the commando 
```
platformio update
```

To run: 
```
platformio run -t upload
```
If the right port cant be found change port to correct port with this command:
``` 
platformio run -t upload --upload-port <port>
```
You can find the correct port with:
```
pio device list
```
After flashing the robot will instantly start working. If the robot loses power and is then reconnected it will restart the program.

## Code layout
To write the preprogrammed code or to upload a file the code given has to be in a certain structure:

The setup wil run once and can be used to place som e setters before you start your program. Th eloop section will do exactly that: loop until the robot dies or power is removed.

```
SETUP:

// setup code

LOOP:

// code that will be looped

EOF:
// define end of code file

```

## Commands:

### Instant commando's
- ```shutdown```, The robot shuts down until movement is detected.
- ```interactive_mode```, The robot tries to follow an object in front of its face but looses interest if no object is found.
- ```break_reminder```, Show the break reminder on the screen for the set duration. 
- ```walk_reminder```, Show the walk reminder on the screen for the set duration.
- ```water_reminder```. Show the water reminder on the screen for the set duration.
- ```show_weather_station```,show the weatherstation on the screen for the set duration. NOTE: The temperatures, baro and humidity are measured inside the robot and can be inaccurate. 
- ```rng_movement```, move the robot once to a random position.
- ```return_to_start_pos```, Return the robot to the start position of 90 by 90 degrees. 
- ```move_head x```, move the head to x degrees. 40 <= x => 140
- ```move_neck x```, move the neck to x degrees. 0 <= x => 180.
- ```playSound x``` play start up sound. x:
    -   start_up
    -   shut_down
    -   notification
- ```WAIT x y```. x: Integer, y:
    -   define HOUR 
    -   MINUTE 
    -   SECOND 
    -   MIllI_SECOND 



### Getters
- ```getBreakTime```, Get the duration of the break time reminder.
- ```getWalkTime```. Get the duration of the walk time reminder.
- ```getWaterTime```, Get the duration of the water time reminder.
- ```getWeatherStationTime```, Get the duration of the weather station.
- ```getState```, get the current state of the robot:
    -   IDLE,
    -   REMINDER_BREAK
    -   REMINDER_WATER
    -   REMINDER_WALK
    -   WEATHER_STATION
    -   INTERACTIVE_MODE
    -   OFF

### Setters
Set if timed functions are active or not:
- ```weatherstation = x```:
    -   ACTIVE
    -   NON_ACTIVE
- ```walk_reminder = x```:
    -   ACTIVE
    -   NON_ACTIVE
- ```weatherstation = x```:
    -   ACTIVE
    -   NON_ACTIVE
- ```break_reminder = x```:
    -   ACTIVE
    -   NON_ACTIVE
Set the state of the robot
- ```STATE = x```:
    -   IDLE,
    -   REMINDER_BREAK
    -   REMINDER_WATER
    -   REMINDER_WALK
    -   WEATHER_STATION
    -   INTERACTIVE_MODE
    -   OFF
Set the time duration after which the robot will shut down if there is no movement.
- ```shut_down_after = x y```: x = integer, y = 
    -   define HOUR 
    -   MINUTE 
    -   SECOND 
    -   MIllI_SECOND 
set the duration and the period of a timed event.
a :
-   weatherstation
-   walk_reminder
-   water_reminder
-   break_reminder
- ```a DURATION = x y```: x = integer, y = 
    -   define HOUR 
    -   MINUTE 
    -   SECOND 
    -   MIllI_SECOND 
- ```a PERIOD = 10 MINUTE```: x = integer, y = 
    -   define HOUR 
    -   MINUTE 
    -   SECOND 
    -   MIllI_SECOND 
set how long the robot i allowed to follow an object in interactive mode.
- ```follow_object DURATION = x y```: x = integer, y = 
    -   define HOUR 
    -   MINUTE 
    -   SECOND 
    -   MIllI_SECOND 