from c_milestone.data_cms import time_1_ms, red_1, time_2_ms, red_2
import statistics

def main():
    # TODO: your code here
    lightmax, lightmin = (find_peaks_valleys(red_2))
    print(lightmax, "\n\n")
    print(lightmin, "\n\n")

def find_peaks_valleys(light_list):
    return find_peak(light_list), find_valley(light_list)

def find_peak(light_list):
    peaks = []
    median_val = statistics.median(light_list)
    for i in range(1,len(light_list) - 1):
        if i < 30:
            if light_list[i] > max(light_list[0:i]) and light_list[i] > max(light_list[i+1: i+31]):
                peaks.append(light_list[i])
        elif i >= 30 and i < len(light_list) - 30:
            if max(light_list[i-30:i]) < light_list[i] and max(light_list[i+1:i+31]) < light_list[i] and light_list[i] > median_val:
                peaks.append(light_list[i])
        elif len(light_list) - i < 30:
            if light_list[i] > max(light_list[i-30:i]) and light_list[i] > max(light_list[i+1: len(light_list)]):
                peaks.append(light_list[i])
    return peaks

def find_valley(light_list):
    valleys = []
    median_val = statistics.median(light_list)
    for i in range(1,len(light_list) - 1):
        if i >= 30 and i < len(light_list) - 30:
            if min(light_list[i-30:i]) > light_list[i] and min(light_list[i+1:i+31]) > light_list[i] and light_list[i] < median_val:
                valleys.append(light_list[i])
        elif len(light_list) - i < 30:
            if light_list[i] < min(light_list[i-30:i]) and light_list[i] < min(light_list[i+1: len(light_list)]):
                valleys.append(light_list[i])
    return valleys

    
if __name__ == '__main__':
    main()
