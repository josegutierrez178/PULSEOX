# PulseOx D Milestone
**By Ali Bruder,Divine Musonza and Jose Gutierrez**

The background research for the PulseOx Project included the topics of:
- Microcontrollers 
- Breadboards
- Oximeters

## Microcontrollers

### What is a *microcontroller*?
A *microcontroller* (MCU) is a small computer on a single integrated circuit that is designed to control specific tasks within electronic systems. 

### Why are they useful? 
Microcontrollers combine all the necessary elements of a microcomputer system onto a single piece of hardware. They are small, inexpensive, low-power, and integrate all the necessary components to control a specific task in a single chip.
### Give two examples of devices that use microcontrollers.
Smart phones, washing machines

### How does a microcontroller differ from a standard computer?
A microcontroller has less power, and it is used for specific purposes while a standard computer has more power and can run several tasks at the same time.
## *Arduino* Programming Language:
### What language does Arduino use when coding?
A simplified version of C++ as it combines elements of C and C++. Additionally, Arduino automatically does certain tasks so that it is easier to work with as a beginner.

### What is an Arduino *sketch* and how should they be stored locally on your device?
A *sketch* is a file where we write programs to run on our Arduino boards. Sketches have a .ino extension, which supports the Arduino programming language (a variant of C++). 

### What are the two main functions of an Arduino sketch, and how do they differ from each other?
The two main functions in every Arduino sketch are *setup()* and *loop()*. The *setup()* function runs once at the beginning of the program and is used to initialize settings such as pin modes, libraries, or communication protocols. The *loop()* function runs repeatedly after setup() finishes. It contains the main logic of the program and is used to continuously check inputs, update outputs, and respond to changes in real time.

### What do the following functions do, and why are they important in the scope of our project? 
- *digitalRead()* - read the digital state of a specified digital pin. It checks the voltage level on the pin and returns either high or low. In our project, this will be used to read the intensity of light from the photodiode. 
- *digitalWrite()* - sets a digital output pin to either high or low. In our project, it will be used to display the results of the oximeter.
- *pinMode()* - configures a digital pin to behave as either an input or output. In our project, we will use this to set one of the pin to photodiodes and the other to set the output device.

### Do I need to keep the Arduino connected to my computer for the program to run?
No, you do not need to keep your Arduino connected to your computer for the program to run. Once the program (sketch) is uploaded to the Arduino board, it is stored in the microcontroller's non-volatile memory and can run autonomously. You only need an external power source, such as a battery or a wall adapter, for the Arduino to operate independently. 
### What is *serial data communication*?
*Serial data communication* is a method of sending digital data, one bit at a time, over a single communication channel

### Explain how it is used between a computer and a microcontroller.
Serial data communication between a computer and microcontroller sends digital data one bit at a time over a single wire, commonly using the Universal Asynchronous Receiver/Transmitter (UART) protocol. A ground connection, transmit (TX) line, and receive (RX) line are needed. Both devices must agree on a specific transmission rate (baud rate) and synchronized clocks to correctly interpret the bit stream. Computers typically use a USB-to-serial converter for this.

## Breadboarding
![Alt text](https://cdn.sparkfun.com/assets/0/a/b/a/5/5192a48fce395f1573000000.jpg?__hstc=250566617.07feade9bcd911d0be4abe9d3f6a730c.1759956528383.1759956528383.1759956528383.1&__hssc=250566617.1.1759956528383&__hsfp=2815531451.jpg "Breadboard")
### How is electricity distributed through a *breadboard*?
Breadboards, under the plastic, have rows of conductive metal in which electricity is transferred. These strips connect the holes on the surface, allowing electricity to flow between them when electronic components (such as resistors, LEDs, or wires) are inserted into those holes. Along the edges of the breadboard are power rails, which are usually labeled with a ‘+’ or ‘-’ representing the charge. These two sides, however, are not connected, and both need power. Within the middle of the breadboard is the ravine, which is there in order to make sure short-circuiting doesn’t happen. This is where DIP support comes into play is due to integrated circuits. There are circuits to connect over the ravine called Dual In-Line Package. The letters along the top and bottom serve as a guide for your circuits, and the binding posts allow you to connect various power sources. 

Electricity is evenly distributed by both sides/power rails having power. This power can then be distributed to terminal strips, which are the horizontal rows through jumper cables. A component then sits across rows so power can pass through it, and the last part connects to the negative power rail across the ravine. 

### What is the difference between an *Anode* and *Cathode* when wiring an LED?
The *Anode* is the positive lead, and the *cathode* is the negative lead; the respective charges must be hooked up in order for the device to work.

### How does the Arduino interact with external outputs? What pin would you need to use to ground the circuit?
To ground a circuit in the Arduino, connect it to one of the Arduino's dedicated GND pins.  The GND pin is connected to the negative side of the board's power supply. It serves as the reference point for measuring voltages. There is then another specific pin that channels the positive as well. 

### What pin would you need to use to provide the voltage?
Depends on the specific circuit you are creating
The 5V pin gives a constant 5V voltage
The 3.3V pin gives a constant 3.3V voltage
Digital I/O pin can provide voltage when set to high within your Arduino code

## Oximeters

### What is a *Pulse Oximeter* and how does it work?
A *Pulse Oximeter* is an externally used medical device that measures the oxygen saturation in your blood. Gives a gauge of how healthy a person's lungs are. 

### What is *Blood Oxygen Saturation* (SpO2) and what is *heart rate* (bpm)? Why are they relevant data pieces for medical professionals?
*Blood Oxygen Saturation* (SpO2) is a measure of how many hemoglobin in your blood are bound to Oxygen
*Heart rate* (bpm) is the amount of times your heart beats or pumps blood to the rest of your body in a minute. 
These measurements are important pieces of information for medical professionals because they assess organ function, help diagnose conditions or monitor the progression of an existing condition. Overall provide quick data and insight into a patient. 
How is Blood Oxygen Saturation (SpO2) calculated? Explain both in words and with a mathematical formula.
Blood Oxygen Saturation (SpO2) is calculated by the amount of oxygen your blood is actually carrying divided by the maximum carrying capacity.
$$
SpO_2 = \frac{HbO_2}{(Hb + HbO_2)} \times 100\%
$$

### How is heart rate (bpm) calculated? Explain both in words and with a mathematical formula.
Heart rate is the amount of heat beats within 60 seconds: Find your pulse, count your heartbeat for 30 seconds, then double it.

### How would we identify a heartbeat given a list of *IR resistances* (in ohms) and times (in milliseconds)?
Impedance plethysmography measures volume changes in tissues indirectly by tracking how electrical impedance (resistance) varies over time.
When the heart pumps blood into vessels, the volume of blood in the tissue increases, which lowers the electrical impedance measured between electrodes.
As blood drains between beats, impedance rises again.
Each heartbeat therefore produces a fluctuation in impedances
By analyzing IR resistances, impedance plethysmography can identify these cyclical fluctuations, with each cycle corresponding to a heartbeat.
Counting the cycles gives the number of heartbeats (and from the timing, the heart rate).
For this, identify the peaks or troughs in the resistance vs. time data (each peak = one heartbeat) and calculate the time difference between successive peaks.
Heart Rate (BPM)= 60,000/Change in Tms
Tms=time difference in milliseconds between successive peaks (heartbeats)

## Resources

Microcontroller Basics (https://www.geeksforgeeks.org/digital-logic/microcontroller-and-its-types/)
Arduino Sketches (https://docs.arduino.cc/arduino-cloud/cloud-interface/sketches/)
How an Arduino works (https://www.digikey.dk/da/articles/improve-pulse-oximetry-measurements-using-dark-current-compensation?srsltid=AfmBOora6axgpEDxMtYHdxKf7atRSz-zBc2e1CsV97VN8UfF2-stHIP)
Arduino Memory (https://forum.arduino.cc/t/does-arduino-need-to-be-connected-to-pc/566811)
Data communication between microcontroller and computer (https://www.renesas.com/en/support/engineer-school/mcu-programming-peripherals-03-serial-communication?srsltid=AfmBOoqJjcpdsU4pm4__Sfs-PyrKbIxAtYCu_Hb1lMQyJ4pJMZdNmynd)
Breadboard Basics (https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard/all)
Pulse Oximetry:
- https://www.yalemedicine.org/conditions/pulse-oximetry
- https://iowaprotocols.medicine.uiowa.edu/protocols/pulse-oximetry-basic-principles-and-interpretation
- https://www.sciencedirect.com/science/article/pii/S095461111300053X



