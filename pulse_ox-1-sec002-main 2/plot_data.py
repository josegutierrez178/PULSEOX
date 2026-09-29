import numpy as np
import matplotlib.pyplot as plt
import serial
import time

def create_txt(port='/dev/cu.usbmodemF412FA6F8EB02', baud_rate=9600, filename='pulseox_data.txt', with_timestamp=True):
    """Reads data from Arduino via serial port and saves it to a text file. 
    Takes the port name and baud rate as arguments and creates the txt file."""
    try:
        ser = serial.Serial(port, baud_rate)
        time.sleep(2)  # wait for the serial connection to initialize
        print(f"Recording data from {port} to {filename}... Press Ctrl+C to stop.")

        with open(filename, 'a') as f:
            while True:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').strip()
                    print(line)
                    f.write(line + '\n')

    except KeyboardInterrupt:
        print("\nRecording stopped by user.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial connection closed.")

def read_serial_file(filename):
    """ For each line in txt file from arduino:
      1. splits by comma
      2. if it starts with "Computed SPO2", parse spo2 and bpm values
      3. if it stats with "RED", parse red and ir values and time values 
      4. return lists of spo2, bpm, red readings, ir readings and time readings """
    spo2_values = []
    red_values = []
    ir_values = []
    bpm_values = []
    time_values = []
    with open(filename, "r") as pulseoxdata:
        for line in pulseoxdata:
            line = line.strip()

            # skip empty lines
            if not line:
                continue

            # Parse lines like: "Computed SpO2: 90.00 | BPM: 72.2"
            if line.startswith("Computed SpO2"):
                try:
                    spo2 = float(line.split(":")[1].strip())
                    spo2_values.append(spo2)
                    bpm = float(line.split(":")[3].strip())
                    bpm_values.append(bpm)
                except (IndexError, ValueError):
                    continue

            # Parse lines like: "RED,157,IR,163"
            elif line.startswith("RED"):
                try:
                    parts = line.split(",")
                    # expected format: ['RED', '157', 'IR', '163', 't', '20000'fffffffffgtgtg]
                    red_val = int(parts[1])
                    ir_val = int(parts[3])
                    time_val = int(parts[5])
                    red_values.append(red_val)
                    ir_values.append(ir_val)
                    time_values.append(time_val)
                except (IndexError, ValueError):
                    continue
    return spo2_values, red_values, ir_values, bpm_values, time_values

def plot_red_ir(red, ir, time):
    # create figure with two subplots (plt.subplots) 
    # Plots SpO2 (%) and heart rate (BPM) versus sample number in two subplots.
    fig, axs = plt.subplots(2, 1)
    axs[0].plot(time, red, '-')                
    axs[0].set(xlabel = 'Time', ylabel = 'Red Values')
    axs[0].set_title('Red Resistance vs. Time')
    axs[1].plot(time, ir, '-')                 
    axs[1].set(xlabel = 'Time', ylabel = 'IR Values')
    axs[1].set_title('IR Resistance vs. Time')
    fig.tight_layout()
    fig.savefig('red_ir_plot.png')
    plt.show()

def main():
    """Main function that runs the program and calls all other functions"""
    filename = "pulseox_data.txt"
    create_txt('/dev/cu.usbmodemF412FA6F8EB02', 9600, 'pulseox_data.txt')
    spo2list, redlist, irlist, bpmlist, timelist = read_serial_file('pulseox_data.txt')
    red = np.array(redlist)
    ir = np.array(irlist)
    times = np.array(timelist)
    plot_red_ir(red, ir, times)

if __name__ == "__main__":
    main()