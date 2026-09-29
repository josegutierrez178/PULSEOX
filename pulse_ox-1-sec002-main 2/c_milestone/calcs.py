from c_milestone.data_cms import time_1_ms,  ir_1, red_1, red_2, time_2_ms
from c_milestone.red_led import find_peaks_valleys, statistics
from c_milestone.ir_led import peak_and_valley_class, statistics

def SPO2(red_peaks, red_valleys, IRpeaks, IRvalleys): #convert their data to an overall max and an overall min, then sp02
    """
    Calculate SpO2 (blood oxygen saturation) from red and infrared LED sensor data.
    
    This function averages the SP02 values to calculate SpO2 by using a peak and a corresponding valey of the same index within the red and infared light lists.
    It begins by balancing the, lists, iterating through them and putting the calculated SP02 values in a new list. 
    From here it then averages all the SP02 values and applies then the empirical formula: SpO2 = 110 - 25*R.
    
    Parameters
    ----------
    red_peaks : list
        Peak values from red LED sensor readings.
    red_valleys : list
        Valley values from red LED sensor readings.
    IRpeaks : list
        Peak values from infrared LED sensor readings.
    IRvalleys : list
        Valley values from infrared LED sensor readings.
    
    Returns
    -------
    float
        SpO2 percentage (0-100) if sufficient data points exist (>=5 each),
        False otherwise
    """    
    
    if len(red_peaks) >=5 and len(red_valleys) >=5 and len(IRpeaks) >=5 and len(IRvalleys) >=5: #makes sure there is enough in the list
        
        list_r = []
        n = min(len(red_peaks), len(red_valleys), len(IRpeaks), len(IRvalleys)) #evens the lists out so they are all the same length
        for i in range(n): #iterates over the list, caclulating an R value for
            numerator = (red_peaks[i] - red_valleys[i]) / (red_valleys[i])
            denominator = (IRpeaks[i] - IRvalleys[i]) / (IRvalleys[i])
            R_ls = numerator / denominator
            list_r.append(R_ls)
        total = sum(list_r) # adds up all the R values
        elmnts = len(list_r) 
        R = total/elmnts #averages 
        #avgs
        
        SPO_2 = 110 - 25 * R #plugs in
        return SPO_2
    else:
        return False

def index_to_time(index_list, time_list):
    """ This function matches the times to the peaks/valleys """
    times_at_indecies = [time_list[i] for i in index_list] 
    return times_at_indecies





def bpm_from_times(light_list, time_list):
    """
    Calculate heart rate (beats per minute) from sensor readings.
    
    This function takes in the red light or infared light readings, as well as the times.
    The function then uses other functions to get the peaks and valleys. From here these peaks and valleys are matched with their 
    times at which they occured. From here these things are used to find the BPM by finding the average period. 
    With average period; 60 / avg period(seconds) grants beats per minute. 
    
    Parameters
    ----------
    light_list : list
        Sensor readings from either red or IR LED 
    time_list : list
        Corresponding timestamps in milliseconds.
    
    Returns
    -------
    float
        Heart rate in beats per minute (BPM).
    """


    time_list_seconds= [t / 1000 for t in time_list] #converts to seconds
    peaksss, valleys = find_peaks_valleys(light_list) #grabs IR valleys
    times_for_calc =[] #creates empty list for times 
    count = 0
    i = 0
    while i in range(len(light_list)) and count < len(valleys): #find time diffrences between consecutive valleys 
        if light_list[i] == valleys[count]:
            times_for_calc.append(time_list_seconds[i])
            count += 1
        i += 1
    
    total_dif = 0
    for i in range (1, len(times_for_calc)):  # finds total diffrence 
        total_dif += times_for_calc[i] - times_for_calc[i-1]
    
    average_period = total_dif / (len(times_for_calc) - 1) # finds average period
    bpm = 60/ average_period #finds bpm
    return bpm 


    



def main():
    """ This function is here for debugging purposes. """
    red_peak, red_valleys = find_peaks_valleys(red_1)
    ir_peak, ir_valley = peak_and_valley_class(ir_1)
    SPO2(red_peak, red_valleys, ir_peak, ir_valley)
    

    
if __name__ == '__main__':
    main()