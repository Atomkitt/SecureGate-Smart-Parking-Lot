SecureGate: Smart Parking Lot

Problem Statement

At SIIT, the main parking lot is often full, resulting in the driver having to circle repeatedly to try and find available space or wait for someone to leave. This can be time-consuming and possibly lead to eg. late attendance for the students. It would be preferable if the driver already knows that space is full, so that they can immediately park in another area.
Meanwhile, other parking lots such as in malls usually display an inaccurate count of available parking spaces due to some sensors not registering a vehicle already occupying the space. This makes the information unreliable and obsolete. 
Therefore, by instead counting the total number of vehicles inside the parking area using our system and comparing it to the maximum capacity, we can limit the number of vehicles and prevent overcrowding for efficient and convenient parking.

Components used & How the system works

Components used:

Microcontroller: Arduino Uno
Input Sensors: HC-SR04 (Ultrasonic), Keypad, DIP Switch
Output Devices: Micro Servo SG90, 7-Segment Display, Buzzer, LEDs
Others: Resistors 330 Ohms, Breadboard, Jumper wires

How the system works:

Hardware Interfacing

The system utilizes sensors and actuators connected via a breadboard:
Detection & Input: The HC-SR04 Ultrasonic Sensor acts as the eyes of the system, measuring the distance of vehicles. A Keypad: Provides a multi-function manual for administrators. It allows manual gate control (Entry/Exit) and direct adjustment without moving the gate, while a DIP Switch is for hardware sensitivity adjustment by switching between detection 10cm - 20cm ranges.
Actuation & Feedback: A Micro Servo SG90 operates the gate using Pulse Width Modulation (PWM). Visual feedback is provided by a 7-Segment Display (displaying remaining spots) and dual-color LEDs, while a Low-Level Trigger Buzzer provides audible alerts during gate movement.

Software Logic :designed to be "Robust against noise" through two advanced techniques

Noise Reduction: Instead of relying on a single distance reading, the system performs three measured elements and selects the Median Value. This filters out "spikes" or "echo noise" , ensuring the gate doesn't open or close accidentally.
Finite State Machine: The system operates through four distinct states to ensure reliable transitions:
IDLE: Monitoring distance and waiting for a vehicle.
OPENING: Activating the servo and buzzer while switching the LED to green.
OPEN: Holding the gate up and waiting for the vehicle to pass (confirmed by distance increasing beyond the threshold).
CLOSING: Returning the servo to idle state and decrementing the parking counter.


Operational Workflow

Sensing: The system reads the distance and checks the DIP Switch position to set the threshold.
Validation: If a vehicle is detected within the threshold AND parking spaces are more than 0, the FSM transitions to OPENING.
Transaction: Once the car passes, the 7-Segment Display updates the count (e.g., 9 down to 8).
Manual Override: The keypad control panel with the following functions:
Button '1': Manual Entry (Opens gate, decrements counter by 1).
Button '4': Manual Exit (Opens gate, increments counter by 1).
Button '2' / '5': Increments/Decrements the display value by 1 without moving the gate.
Button '0': System Reset (Resets counter to 9 and provides audio feedback).

Result / Conclusion

Results

Detection Accuracy: The HC-SR04 sensor accurately detected objects within the 10cm and 20cm thresholds with a minimal error margin of 0.5cm.
Noise Reliability: The gate remained stable even when rapid movements or signal echoes occurred.
System Feedback: The 7-Segment Display updated in real-time as cars passed. LEDs and the Buzzer provided clear, synchronized status alerts for "Entry" and "Wait" states.
Manual Override: The Keypad responded instantly, proving the system’s ability to handle  administrator inputs.

Conclusion

This Arduino project provides a robust, real-time parking solution. By integrating FSM logic and noise-filtering, the system ensures high reliability and stability. The use of internal pull-up resistors and optimized wiring maintains a compact footprint, demonstrating how smart software and hardware integration effectively solve complex automation needs.

<img width="568" height="512" alt="image" src="https://github.com/user-attachments/assets/6a0434b1-47ed-434d-9156-3843a9d5e681" />
