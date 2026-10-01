# Pulseox

## Description

The goal of this project is to create a low-cost, Arduino-based pulse oximeter that measures and displays a user’s blood oxygen saturation (SpO₂) and heart rate (BPM) in real time. The device uses infrared (IR) and red light sensors to collect physiological data, processes it directly on the microcontroller, and outputs the results through the Serial Monitor. 

## Background

### Arduino Data Processing

This role developed and implemented the logic for identifying peaks and valleys in both the red and infrared (IR) light signals. The Arduino continuously collects sensor data, storing averaged samples from each LED in fixed-size arrays (`redValues[]` and `irValues[]`). To manage the limited memory of the microcontroller, the arrays function as circular buffers meaning that once they reach their maximum length, new samples overwrite the oldest ones. Parameters such as `MAX_SAMPLES` and `periods_stored` let users control how much data is captured in each period. This approach provides a memory-efficient way to maintain a live rolling window of data, similar to how real pulse oximeters update their readings continuously.

### Arduino Data Analysis and Display

This role handled the real-time calculation of blood oxygen saturation (SpO₂) and heart rate using the processed sensor data. After the Arduino verified that enough valid peaks and valleys were detected, the program computed the ratio of ratios (R) — a comparison of the AC (pulsating) and DC (baseline) components of the red and IR light signals. Using this ratio, SpO₂ was estimated with the linear model `SpO₂ = 110 − 25 × R`, producing realistic oxygen saturation values after calibration. The calculated results were displayed directly on the Serial Monitor with clear labels (ex. “Computed SpO₂: 97.25”), updating automatically each time a new data set was analyzed. Built-in safeguards prevent invalid calculations, such as divide-by-zero errors, ensuring stable and reliable readings during live operation.

### Arduino Data Acquisition and Python Data Import and Visualization

This role designed the system to acquire real sensor data in real time while minimizing electrical noise. During each cycle, the Arduino averages multiple analog readings over a 20 ms sampling window for both the red and IR LEDs. This temporal averaging helps stabilize noisy signals caused by ambient light or electrical interference. After acquisition, the Arduino prints each averaged pair of red and IR values in a comma-separated format (ex. `RED,512,IR,490`), which can later be copied directly from the Serial Monitor into a text file (`pulseox_data.txt`). The Python code `plot_data.py` reads this file, extracts the numeric values, and generates two labeled plots showing the measured SpO₂ and heart rate (BPM) over time. This role bridges the gap between embedded data acquisition and data visualization, allowing both real-time feedback during testing and post-analysis with Python.

## Features

Our Arduino pulse oximeter system performs real-time measurement, analysis, and display of blood oxygen saturation (SpO₂) and heart rate (BPM). The device accomplishes this by:
- Collecting live sensor data from red and infrared LEDs using an analog light sensor.
- Averaging readings over a 20 ms window to reduce signal noise caused by environmental light.
- Storing samples in circular arrays that automatically replace the oldest data as new values arrive.
- Detecting peaks and valleys in both light signals to identify heartbeats and amplitude changes.
- Computing the R ratio from the AC and DC components of the red and IR signals.
- Estimating SpO₂ using a linear model (`SpO₂ = 110 – 25 × R`) and outputting the result with labeled readings to the Serial Monitor.
- Exporting data in an easy-to-parse comma-separated format that can be read by the companion Python visualization script.

This process replicates how a real medical pulse oximeter processes optical signals to monitor vital signs continuously and noninvasively.

## Usage

### Hardware Setup
The system consists of an Arduino board, one red LED, one infrared LED, and an analog ambient light sensor. The LEDs are powered through digital pins, and the sensor reads reflected light intensity through analog input `A0`.
![TinkerCAD Schematic View](Pulse_Oximeter_Circuit_Schematic.png "TinkerCAD Schematic View")
![Assembled System](Breadboard_Connection.png "Assembled System")


### Running the Arduino Program
1. Connect your Arduino to your computer via USBC or USB
2. Open the Arduino IDE and load the project file located at: pulse_ox-1-sec002/b_milestone/b_milestone.ino
3. Verify and upload the sketch to your board
4. Open the Serial Monitor (9600 baud). You should see continuous lines of data in this format:  
  RED,512,IR,490  
RED,520,IR,495   
...  
Computed SPO2: 97.25
5. Once the data is printed, copy the Serial Monitor output into a text file named `pulseox_data.txt`

### Visualizing Data in Python

Use the provided `plot_data.py` file to plot the exported SpO₂ and BPM data. Do this by running the `plot_data.py` function in terminal. This code parses the Arduino output file and displays two subplots:
- Top Plot: Blood oxygen saturation (%) over time
- Bottom Plot: Heart rate (BPM) over time


## Testing

Testing was performed in three main stages to ensure both functional accuracy and reliability:

- Signal Sampling Verification: We confirmed that the analog readings from the IR and red LEDs changed predictably when a fingertip was placed over the sensor and responded appropriately to variations in ambient light. The readings remained stable and consistent across repeated trials, with minimal noise or drift. In addition to verifying the signal itself, we also tested the hardware to ensure that the wiring, LEDs, and photodiodes were functioning properly. During testing, we adjusted or replaced LEDs when necessary to improve signal clarity and overall accuracy. We further examined how variations in finger position, pressure, and distance from the sensor affected the signal amplitude and stability. These tests verified that both the hardware and sampling processes were operating reliably, establishing a strong foundation for accurate SpO₂ and BPM calculations in later analysis.

- Peak and Valley Detection: We verified that the Arduino correctly identified peaks and valleys in both the red and IR datasets by comparing the detected counts with visual plots of the same data. We also conducted noise and sensitivity tests to evaluate the robustness of our system and ensure reliable performance under varying conditions. Additionally, the accuracy of our peak and valley detection was also validated through the calculation stage, since these values were directly used to determine BPM and SpO₂ measurements.

- SpO₂ and BPM Calculation: The calculated SpO₂ readings and heart rate values were compared to established physiological ranges (SpO₂: 95–100%, BPM: 60–100). To further validate accuracy, our device’s results were compared against a commercial pulse oximeter. The measured values closely matched those from the reference device, demonstrating consistent accuracy and reliability in our system’s performance.

Video Demonstration:
https://warpwire.duke.edu/w/fx8JAA/

## Authors and Acknowledgment

Authors: Jose Gutierrez, Divine Musonza, Ali Bruder 

The team would like to acknowledge Conner Bolen and Dr. Pastorino for their support in completing the B milestone.

## AI Use Statement

Generative AI (ChatGPT) was used to support our learning process and improve documentation quality. Specifically, AI was used to:
- Help us understand fundamental Arduino programming concepts, such as variable types, loop structures, and syntax conventions.
- Clarify the purpose and proper use of specific Arduino functions, such as pinMode(), digitalWrite(), and analogRead().
- Explain common C++ operators (for example, how the increment operator ++ increases a variable by one) and the difference between prefix (++x) and postfix (x++) usage.

No AI-generated code was copied or implemented directly into our Arduino or Python files. All final programming logic was written and tested independently by our team, in full accordance with the PulseOx B Milestone AI policy.