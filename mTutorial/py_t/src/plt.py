#!/bin/python

'''
Created on Jul 14, 2016

@author: wocson
'''

import numpy as np
import matplotlib.pyplot as plt

x = [2,3,4,5,7,9,13,15,17]
x[2]=33

plt.plot(x)
plt.ylabel('Sunlight')
plt.xlabel('Time')
plt.show()
