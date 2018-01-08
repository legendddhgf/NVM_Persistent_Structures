#!/usr/bin/python3
# TODO: this is just an example from matplotlib, please make this into a real
# program

import sys
import numpy as np
import matplotlib.pyplot as plt

if (len(sys.argv) != 2):
	print("Need to specify location of folder with data")
	sys.exit()

men_means = (20, 35, 30, 35, 27)
men_std = (2, 3, 4, 1, 2)

sll_means = (1, 3, 4, 5, 6)
vector_means = (55, 32, 2, 4, 6)
lsm_means = (5, 7, 78, 3, 4)
btree_means = (3, 6, 7, 3, 2)
N = 5

ind = np.arange(N)  # the x locations for the groups
width = 0.2       # the width of the bars

fig1, ax1 = plt.subplots()
fig2, ax2 = plt.subplots()
fig3, ax3 = plt.subplots()

#rects1 = ax.bar(ind, men_means, width, color='r', yerr=men_std)
rects1 = ax1.bar(ind, men_means, width, color='r')

women_means = (25, 32, 34, 20, 25)
women_std = (3, 5, 2, 3, 3)
#rects2 = ax.bar(ind + width, women_means, width, color='y', yerr=women_std)
rects2 = ax1.bar(ind + width, women_means, width, color='y')

# add some text for labels, title and axes ticks
ax1.set_ylabel('Time for Various Insertions')
ax1.set_title('Scores by group and gender')
ax1.set_xticks(ind + width / 2)
ax1.set_xticklabels(('40', '400', '4000'))

ax1.legend((rects1[0], rects2[0]), ('SLL', 'Vector'))


def autolabel(rects, ax):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%d' % int(height),
                ha='center', va='bottom')

autolabel(rects1, ax1)
autolabel(rects2, ax1)

plt.show()
