from c_milestone.data_cms import *
from c_milestone.calcs import bpm_from_times, SPO2
from c_milestone.red_led import find_peaks_valleys
from c_milestone.ir_led import peak_and_valley_class

def main():
    red_peak, red_valleys = find_peaks_valleys(red_2)
    ir_peak, ir_valley = peak_and_valley_class(ir_2)
    print(round(SPO2(red_peak, red_valleys, ir_peak, ir_valley)))
    print(bpm_from_times(red_1, time_1_ms))
    
    







    
if __name__ == '__main__':
    main()
