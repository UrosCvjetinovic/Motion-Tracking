# Motion-Tracking
Motion tracking using gyroscope and accelerometar.

By using the accelerometer and gyroscope from the BMI088 sensor, the capability to track the motion of an object was achived. Possible orientation states of the board have been defined and transiton between them when motion is detected with the gyroscope.

## Project requirements:
**Integrated Computer Systems 2, project 7b**

Write a program that enables synchronous reading of a 3D accelerometer and 3D gyroscope using the X-NUCLEO-IKS01A2 sensor board. The acquired data needs to be transmitted via a serial connection to a computer and graphically displayed on an OLED display.

 # Report 
 ## How to set up
 ### Hardware
 - Board: X-NUCLEO-IKS01A2
 - Sensor: [BMI088](https://www.bosch-sensortec.com/products/motion-sensors/imus/bmi088/) (used microe click with sensor integrated [13DOF-click](https://www.mikroe.com/13dof-click) )
 ### Software
 - Clone repo:
```
git clone --recurse-submodules https://github.com/UrosCvjetinovic/Motion-Tracking.git
```
 - Adapt stm32 driver to use the given board:
```
   Uncomment define of STM32L476xx in file Drivers/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/include/stm324xx.h:Line78
```
 
 ## Implementation
 
 ### Used internal modules
 
 #### Timers
 In this project 1 timer is used TIM17 for debauncing. 
 
 ##### TIM17
 To debounce two buttons (SW1 and SW2), it is necessary to check their status after the initial press and activate the desired procedure (in this case, changing the displayed screen on the OLED display - NextScreen()) only once, and the waiting time is 90ms.
 
 #### I2C
 For communication with the OLED display and BMI088, I2C 2 is used, operating in standard mode at 100kHz. The implementation of the driver for this display is included as part of the stm32-ssd1306 submodule.
 
 ### Logic
 
 #### Orientation states
 The orientation states can be seen in the first column of the figure 'TransitionStates'. Transitions between states occur when the angular velocity values read from the BMI088 gyroscope of axes exceeds a certain threshold.
 
 
 ![TransitionStates](https://github.com/UrosCvjetinovic/Motion-Tracking/blob/master/img/TransitionStates.png)
 
 #### Display manager
 The display manager's role is to keep track of which screen to display and, if a button is pressed, cycle through each of the screens. Reading data from the sensors is only done for the necessary information related to the current screen.
 
 - *Idle Screen:*
    While this screen is active, no readings are taken, and the sensors are powered off.
	
 ![IdleScreen](https://github.com/UrosCvjetinovic/Motion-Tracking/blob/master/img/IdleScreen.jpeg)
 
 - *Connection Status Screen:*
    While this screen is active, the sensors are suspended, and checks are performed to verify if the sensors are connected.
	
 ![ConnectionStatusScreen](https://github.com/UrosCvjetinovic/Motion-Tracking/blob/master/img/ConnectionStatusScreen.jpeg)
 
 - *Raw Data Screen:*
    While this screen is active, the sensors are active, and measurements from the sensors are displayed on the screen.
	
 ![RawDataScreen](https://github.com/UrosCvjetinovic/Motion-Tracking/blob/master/img/RawDataScreen.jpeg)
 
 - *Orientation Screen:* 
    While this screen is active, the sensors are active, and orientation calculated from the sensors measurements are displayed graphically.
	
 ![OrientationScreen1](https://github.com/UrosCvjetinovic/Motion-Tracking/blob/master/img/OrientationScreen1.jpeg)
 
 ![OrientationScreen2](https://github.com/UrosCvjetinovic/Motion-Tracking/blob/master/img/OrientationScreen2.jpeg)
 
 ## Testing
 
 TODO: add images
 
 ### Comments
 - Transitions are made to only change by rotating on one of the axes. Possible improvement can be to have states when two axis exceed threshold at the same time
 
