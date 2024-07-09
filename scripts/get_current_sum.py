import numpy as np 
import matplotlib.pyplot as plt 

#loading data 
with open('current.txt') as f: 
    data=f.readlines()

time = range(0, len(data))

sum = 0
for i in range(len(data)):
    sum += float(data[i].strip())

print(sum)
print(len(data))
print(sum / len(data))