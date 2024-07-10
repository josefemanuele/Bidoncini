import numpy as np 
import matplotlib.pyplot as plt 

#loading data 
with open('current_duty_delay.txt') as f: 
    data=f.readlines()

time = range(0, len(data))

sum = 0
for i in range(len(data)):
    sum += float(data[i].strip())

print("Sum: " + str(sum) + " mA * ms")
print("Period: " + str(len(data)) + " ms")
print("Average consumption: " + str(sum / len(data)) + " mA")