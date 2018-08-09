# Generic Libraries for Illini RoboMaster Embedded Control Systems

This repository is a library dependency for Illini RoboMaster Embedded Control Systems.
We separate our library structure into two layers: BSP layer and Libraries layer.
These two layers are listed in an ascending order of abstraction level. They are generically
built to adapt to most of the common hardware designs, control logics, and even STM32 MCUs. Such
generalization capabilities of this library allow us to compress all kinds of different control logics
for different robots into this ONE control library. So you may find it helpful to look straight into
our source code for some generic functionalities. Below are a more detailed description of our layers.

## [BSP](https://github.com/illini-robomaster/iRM_Embedded_Libraries/tree/master/BSP) - Board Supporting Package
BSP layer handles direct interactions with hardware drivers and HAL libraries. It currently supports control for ADC, UART, GPIO, CAN, Generic IMU, Key Press Debouncing, Laser, LED, Debug Printing (through a user defined UART port), and PWM. In this layers, everything is built to be generic so that it may be reused by any higher level control logic libraries.

## [Libraries](https://github.com/illini-robomaster/iRM_Embedded_Libraries/tree/master/Libraries)
Libraries layer is built upon the BSP layer. In this layer, we develop control logic for a higher level abstraction of hardware such as motor control, pid calculation, fusion kalman filter for sensor denoising etc. Some modules such as gimbal / haptor control may be developed to specifically adapt to our hardware settings, but most of them are built to achieve generic functionalities within control areas.


## [Tests](https://github.com/illini-robomaster/iRM_Embedded_Libraries/tree/master/Tests)
Tests layer is independent of the main program. It is only used when the RUNTEST flag is set to ON when compiling the project. We create these Tests, either unit tests or runtime functionality tests, to insure the modules we developed are working properly before putting them into the main program. So it contains all levels of tests ranging from BSP layers to Libraries layers. However, it is currently poorly documented, so you might need some time to go through the source code to find out how we tests our own modules.
