# INF2004-Embedded-Micro-mouse

Develop a robot car that navigates a _predefined track_, _efficiently avoiding obstacles_ and _recognizing barcodes_ using infrared sensors. The integration of
a _PID controller_ enhances the car's control system, ensuring smooth and accurate motion during navigation and _barcode scanning_.

## Table of Contents

- [Team Members](#team-members)
- [Contributions](#contributions)
- [Getting Started](#getting-started)
- [Integration](#integration)
- [GPIO Pins](#gpio-pins)
- [Additional Resources](#additional-resources)
- [Block Diagram & Flowcharts](#block-diagram--flowchart)

## Team Members

- Ong Si Hui (2201945)
- Pang Zi Jian, Adrian (2200692)
- Nur Hakeem Bin Azman (2201052)
- Joshua Lim (2200687)
- Shaun Tay (2200555)

## Contributions
| `Member's Name`          | `Contributions`|
| -------------------------|:--------------:|
| Ong Si Hui                | Motor, Wheel Encoder, Integration, Documentation |
| Pang Zi Jian, Adrian      | Web Server, Mapping, Integration, Documentation  |
| Nur Hakeem Bin Azman      | Magnetometer, Integration, Documentation         |
| Joshua Lim                | IR Sensors, Integration, Documentation           |
| Shaun Tay                 | Ultrasonic Sensors, Integration, Documentation   |


## Getting Started

### Project Description
This engaging undergraduate project challenges students to create an intelligent autonomous 2-wheel robot car using the Raspberry Pi Pico microcontroller, equipped
with various sensors and a PID (Proportional-Integral-Derivative) controller. The primary goal is to develop a robot car that navigates a predefined track, efficiently
avoiding obstacles and recognizing barcodes using infrared sensors. The integration of a PID controller enhances the car's control system, ensuring smooth and accurate
motion during navigation and barcode scanning.
1. Robot Car Construction: Assemble and construct the 2-wheel robot car, ensuring mechanical stability and precision wheel control.
2. Sensor Integration: Integrate ultrasonic sensors for obstacle detection, infrared sensors for line following and barcode recognition, and a magnetometer for
orientation.
3. Navigation and Mapping: Implement navigation algorithms that guide the robot car along the track, while avoiding obstacles and optimizing for efficiency.
4. Barcode Recognition: Develop algorithms to enable the car to detect and interpret barcodes on the ground using infrared sensors.
5. PID Controller: Design and implement a PID controller to regulate the robot car's speed and steering, ensuring smooth and precise movement.
6. Efficiency and Speed Optimization: Optimize the navigation and PID control algorithms to achieve the shortest track completion time while accurately
recognizing barcodes.

<details>
<summary>Parts Needed/Sensors</summary>
2x Motor <br>
1x L298N Motor Driver Module <br>
2x Encoder <br>
1x Ultrasonic Sensor <br>
3x IR Sensor <br>
1x Cable Connector Splitter <br>
1x Magnetometer <br>
4x AA Batteries <br>
1x AA Battery Holder <br>
</details>

## Integration
![EmbeddedDiagram](https://github.com/skyish242/INF2004-Embedded-Micro-mouse/assets/46911283/e2605169-2ba2-4549-96ea-a6ee783fa92c)

## GPIO Pins
| `Pin Number`| `Usage`       | `Sensor`      |
| ------------|:-------------:|:-------------:|
| GPIO 0      | ENB           | L298N Module |
| GPIO 1      | ENA           | L298N Module |
| GPIO 4      | SDA           | Magnetometer |
| GPIO 5      | SCL           | Magnetometer |
| GPIO 10     | IN4           | L298N Module |
| GPIO 11     | IN3           | L298N Module |
| GPIO 12     | IN2           | L298N Module |
| GPIO 13     | IN1           | L298N Module |
| GPIO 18     | TRIG          | Ultrasonic |
| GPIO 19     | ECHO          | Ultrasonic |
| GPIO 21     | OUT           | Left Encoder |
| GPIO 22     | OUT           | Right Encoder |
| GPIO 26     | AO            | Right IR |
| GPIO 27     | AO            | Left IR |
| GPIO 28     | AO            |  Barcode IR |

## Additional Resources

<details>
<summary>Mapping Algorithm</summary>
  
### Pledge Algorithm
*****
This algorithm can work for finding an "exit" but when the robot first enters the maze through an opening it might detect the entrance as an exit.
[https://wiki.thymio.org/en:avoiding-an-obstacle-using-the-pledge-algorithm](https://wiki.thymio.org/en:avoiding-an-obstacle-using-the-pledge-algorithm)

### Flood Fill Algorithm
*****
**Theory** : __From end goal to start__
[https://www.youtube.com/watch?v=Zwh-QNlsurI](https://www.youtube.com/watch?v=Zwh-QNlsurI)
[https://www.youtube.com/watch?v=ktn3C7aXVR0&t=172s](https://www.youtube.com/watch?v=ktn3C7aXVR0&t=172s)

### Depth-First Maze Solver
*****
[https://www.youtube.com/watch?v=zalhUp4ms6c](https://www.youtube.com/watch?v=zalhUp4ms6c)

</details>

<details>
<summary>Line Tracking</summary>
 
  [https://www.youtube.com/watch?v=z9sm6G5alp8](https://www.youtube.com/watch?v=z9sm6G5alp8)
  
</details>


### GY511 LSM303DLHC
<details>
<summary>Documentation</summary>
https://cdn-shop.adafruit.com/datasheets/LSM303DLHC.PDF
</details>

## Block Diagram & Flowchart

### Block Diagram
https://drive.google.com/file/d/1rzfYkl3jkq1JCDhGWVK9S4dcBDvNVrC3/view?usp=sharing
![Block Diagram Micromouse](https://github.com/skyish242/INF2004-Embedded-Micro-mouse/assets/46911283/e8e6b7a7-16f2-4d11-ade5-0a2eab0d12fc)


### Flowchart of Individual Sensors
https://drive.google.com/file/d/1V1MQaxMbvh8Wvq6vHUziarrkZD12XFRi/view?usp=sharing
![IndividualSensors](https://github.com/skyish242/INF2004-Embedded-Micro-mouse/assets/46911283/ca255fcd-0e10-41af-bd99-c20a543c3418)

### Flowchart of Floodfill/Mapping Algorithm
https://drive.google.com/file/d/1M9YKE0fn1Jph__agKvQvMAuTRTzeHVLL/view?usp=sharing
![MappingAlgorithm](https://github.com/skyish242/INF2004-Embedded-Micro-mouse/assets/46911283/abd0f4be-ad72-4bf2-876c-6f64e7128b92)


