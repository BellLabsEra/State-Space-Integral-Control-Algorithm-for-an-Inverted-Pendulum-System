# State-Space-Integral-Control-Algorithm-for-an-Inverted-Pendulum-System

An academic project for ECE 4550 - Control System Design, where I applied the collective knowledge I gained on low-level embedded systems and control theory to develop software for a state-space integral control algorithm that was deployed on a QUBE-SERVO 2 produced by Quanser. 

**The primary control elements of this system are the …**
- nonlinear 4th-order Plant model
- Position Controller with ...
    - Estimator design (2-state variables)
    - Regulator design 
        - Anti-Windup compensation on the controlled motor voltage
        - Reference commands for zero and step cases (2 separate experiments)
        - Forward Euler approximation method
    - Actuator 
        - sends motor voltage values

**The embedded system parts comprise the …**
- Programming in C, MATLAB, and Assembly.
    - Data logging & graphing with MATLAB
    - implement the controller in C
    - implement data logging on flash RAM and data transfer in C
- Configuring register values to control the ISR (interrupt service routine)
- Configuring timer interrupts for better real-time numerical integration results
- UART communication protocol
- TI LaunchPad (Microcontroller)
- QUBE-SERVO 2

Image of QUBE-SERVO 2:
![QUBE_SERVO_2](emptyLink)

In the previous project, I conducted experiments on reference command shaping a Gantry-Crane system in order to suppress vibrations (oscillations) and graphed the reference, 2-positions, and motor voltage response data in MATLAB.