# Pulseox

## Description


The problem we are solving is the limited opportunities students have to learn how real medical devices work. Most medical equipment is too expensive or too complex to take apart and study. By creating a low-cost, easy-to-build Arduino pulse oximeter, this project helps teach students about biomedical technology. It gives students a real glimpse into the process of creating valuable medical equipment.
## Background


This project focuses on creating a pulse oximeter using an Arduino microcontroller. A pulse oximeter is a device that is placed on a finger and uses pulsating LEDs to measure the amount of oxygen in a person’s blood. Through this project, we will learn how to design, build, and test this device. Furthermore, we will then show high school students how to build their own pulse oximeter using an Arduino microcontroller as well. Some scientific principles that relate to this project are electronics, programming, physiology, photoplethysmography, and analog signal processing. 


## Features

Our C milestone can calculate blood oxygen saturation, SP02, heartbeats per minute, BPM, and provides an online breadboard that can detect these values, simulating a pulse oximeter. This allows for This allows for testing and validation of our algorithm in a virtual environment before moving to physical hardware implementation.
This paves a clear path for our next steps: we will transfer this programming to a physical breadboard using real sensors and components, allowing us to collect live data, verify hardware functionality, and ensure accurate readings under real-world conditions.

## Usage

### Red Peak and Valley Detection

The function `find_peaks_valleys.py`, written by Divine, takes a list of red light data and outputs two lists, a list of all the `peaks` of the data and a list of all the `valleys` of the data. The expeced output is a list of maximum red light readings and minimum red light readings which will be used to calculate blood oxygen saturation and heart rate in beats per miute by the `calcs.py` functions.

### IR Peak and Valley Detection
The `ir_led.py` function, written by Alessandra, is called by the `calcs.py` function. To run it, you need to imput a list of data. This data will be strings of numbers, the values of the IR at given times throughout the light sensing. Running the function with this list sorts the values into `peaks` and `valleys`, where each value in the list is compared to 30 values before the test value and 30 values after the test value. This ensures that it is one of the absolute minimums or maximums in the graph. If the test value is the greatest out of the 81 values, it is classified as a peak, added to the `IRmax` list and returned. If the test value is the lowest out of the 81 values, it is classified as a valley, added to the `IRmin` list and returned.

### Heart Rate and Blood Oxygen Saturation Calculations

The Calcs program takes in all the peaks and valleys returned from the `ir_led` and `red_led` programs to calculate SpO₂. In addition, it uses the time data from when the peaks and valleys occur within the dataset to calculate the `average_period`. From this, the heartbeats per minute (`BPM`) are then calculated using the average period.

## Testing

We are testing two cases: one with regular time intervals and clean, easy-to-read data, and another with more irregular and unfiltered data. These test cases are valid because they demonstrate how our project performs under both normal and less ideal conditions. For both sets of data, we tested the peak and valley finding functions by running them on the set of data and seeing if the number of peaks and valleys in the oputputted list were equal to the actual number of peaks and valleys in the data. For example, the data set for `red_1` had 7 peaks and 6 valleys, so we made sure the function `find_peaks_valleys` output 7 peaks and 6 valleys that are the same values as in the original data. We know the output is correct for the amount of peaks and valleys because we compared our numerical results with the visual graph and confirmed it's accuracy.  In phase 2, we plan to make our peak and valley finding more accurate by being able to smooth the data using a python library before finding the peaks and valleys. 
When testing the accuracy of the BPM and the SpO₂ values, we printed the results of the calculations and compared them to what is considered a resonable range for BPM, 60 to 100, and SpO₂, 95% to 100%. If the numbers were within the range, we considered the function to be accurate. After recieving feedback for the milestone, we are comparing the calculation values to the exact value they should be: data set 1 = ~80 BPM and ~95% SpO₂, data set 2 = ~144 BPM and ~98% SpO₂. 

## Roadmap

In phase 2, the B milestone, the team plans on aquiring real data from ourselves and being able to accurately interpret our SP02. Additionally, building an accurate arduino board that can precisely read the different light values and store them in the ambient light sensor.

## Sprint Spreadsheet
Group sprint tracker:
https://docs.google.com/spreadsheets/d/1XIY4MDFaR6hKzzoO6vm4WhcAjP3rz90g8-_kXYk5BXg/edit?gid=0#gid=0

## Arduino
Arduino sketch in TinkerCAD: 
https://www.tinkercad.com/things/l5KjIqguhKh/editel?sharecode=yrechNrfvnaPcVbKj8-RSMID1t8iemZ0gyViuh58ZTs

## Authors and Acknowledgment

Authors: Jose Gutierrez, Divine Musonza, Ali Bruder 

The team would like to acknowledge Dr. Pastorino for his support in completing the C milestone.