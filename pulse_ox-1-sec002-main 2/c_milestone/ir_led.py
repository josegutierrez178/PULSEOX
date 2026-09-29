from c_milestone.data_cms import time_1_ms, ir_1, time_2_ms, ir_2
import statistics

def peak_and_valley_class(ir_list):
        IRmax = []
        IRmin = []
        median_val = statistics.median(ir_list)
        for i in range(1,len(ir_list) - 1):
            if i < 30:
                if ir_list[i] > max(ir_list[0:i]) and ir_list[i] > max(ir_list[i+1: i+31]):
                    IRmax.append(ir_list[i])
            elif i >= 30 and i < len(ir_list) - 30:
                if max(ir_list[i-30:i]) < ir_list[i] and max(ir_list[i+1:i+31]) < ir_list[i] and ir_list[i] > median_val:
                    IRmax.append(ir_list[i])
            elif len(ir_list) - i < 30:
                if ir_list[i] > max(ir_list[i-30:i]) and ir_list[i] > max(ir_list[i+1: len(ir_list)]):
                    IRmax.append(ir_list[i])

            if i < 30:
                if ir_list[i] < min(ir_list[0:i]) and ir_list[i] < min(ir_list[i+1: i+31]):
                    IRmin.append(ir_list[i])
            elif i >= 30 and i < len(ir_list) - 30:
                if min(ir_list[i-30:i]) > ir_list[i] and min(ir_list[i+1:i+31]) > ir_list[i] and ir_list[i] < median_val:
                    IRmin.append(ir_list[i])
            elif len(ir_list) - i < 30:
                if ir_list[i] < min(ir_list[i-30:i]) and ir_list[i] < min(ir_list[i+1: len(ir_list)]):
                    IRmin.append(ir_list[i])

        return IRmax, IRmin


def main():
    IRmax_1, IRmin_1 = peak_and_valley_class(ir_1)
    IRmax_2, IRmin_2 = peak_and_valley_class(ir_2)
    
    print("IR 1 Peaks:\n", IRmax_1)
    print()
    print("IR 1 Valleys:\n", IRmin_1)
    print()
    print("IR 2 Peaks:\n", IRmax_2)
    print()
    print("IR 2 Valleys:\n", IRmin_2)
    
if __name__ == '__main__':
    main()
