import numpy as np 
import matplotlib.pyplot as plt 

#loading data 
with open('current.txt') as f: 
    data=f.readlines()

time = range(0, len(data))

for i in range(len(data)):
    data[i] = float(data[i].strip())

# plotting
plt.plot(time, data, label='Energy consumption') 
plt.xlabel('Time [ms]') 
plt.ylabel('Current [mA]') 
plt.legend() 
plt.savefig("./plot.png")