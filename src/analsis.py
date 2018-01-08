#!/usr/bin/python3
# TODO: this is just an example from matplotlib, please make this into a real
# program

import sys
import numpy as np
import matplotlib.pyplot as plt

sll_ins = (8, 11, 90, 109, 891, 1143)
vector_ins = (8, 13, 71, 117, 738, 1159)
lsm_ins = (12, 16, 267, 221, 5277, 3364)
btree_ins = (55, 41, 705, 505, 8500, 5621)
pmem_sll_ins = (69, 284, 664, 2821, 7102, 28433)
pmem_vector_ins = (27, 119, 234, 769, 1741, 6660)
pmem_lsm_ins = (30, 133, 471, 1450, 7210, 15397)
pmem_btree_ins = (807, 3237, 9128, 35880, 95698, 352762)

sll_get = (8, 5, 389, 280, 39465, 26510)
vector_get = (2, 2, 26, 19, 243, 200)
lsm_get = (9, 7, 1093, 901, 61147, 47820)
btree_get = (5, 4, 77, 56, 1291, 775)
pmem_sll_get = (6, 6, 475, 331, 61115, 33063)
pmem_vector_get = (2, 3, 24, 23, 262, 221)
pmem_lsm_get = (162, 614, 3532, 9905, 117633, 236333)
pmem_btree_get = (9, 12, 163, 120, 1417, 1182)

sll_mem = (640, 640, 6400, 6400, 64000, 64000)
vector_mem = (520, 520, 4104, 4104, 32776, 32776)
lsm_mem = (4144, 4144, 4144, 4144, 4144, 4144)
btree_mem = (912, 912, 9528, 9528, 95856, 95856)
pmem_sll_mem = (640, 640, 6400, 6400, 64000, 64000)
pmem_vector_mem = (520, 520, 4104, 4104, 32776, 32776)
pmem_lsm_mem = (4144, 4144, 4144, 4144, 4144, 4144)
pmem_btree_mem = (912, 912, 9528, 9528, 95856, 95856)
N = 6

ind = np.arange(N)  # the x locations for the groups
width = 0.1       # the width of the bars

fig1, ax1 = plt.subplots()
fig2, ax2 = plt.subplots()
fig3, ax3 = plt.subplots()

sll_rects1 = ax1.bar(ind, sll_ins, width, color='b')
vector_rects1 = ax1.bar(ind + width, vector_ins, width, color='r')
lsm_rects1 = ax1.bar(ind + 2*width, lsm_ins, width, color='#aaaaaa')
btree_rects1 = ax1.bar(ind + 3*width, btree_ins, width, color='y')
pmem_sll_rects1 = ax1.bar(ind + 4*width, pmem_sll_ins, width, color='c')
pmem_vector_rects1 = ax1.bar(ind + 5*width, pmem_vector_ins, width, color='m')
pmem_lsm_rects1 = ax1.bar(ind + 6*width, pmem_lsm_ins, width, color='g')
pmem_btree_rects1 = ax1.bar(ind + 7*width, pmem_btree_ins, width, color='k')

sll_rects2 = ax2.bar(ind, sll_get, width, color='b')
vector_rects2 = ax2.bar(ind + width, vector_get, width, color='r')
lsm_rects2 = ax2.bar(ind + 2*width, lsm_get, width, color='#aaaaaa')
btree_rects2 = ax2.bar(ind + 3*width, btree_get, width, color='y')
pmem_sll_rects2 = ax2.bar(ind + 4*width, pmem_sll_get, width, color='c')
pmem_vector_rects2 = ax2.bar(ind + 5*width, pmem_vector_get, width, color='m')
pmem_lsm_rects2 = ax2.bar(ind + 6*width, pmem_lsm_get, width, color='g')
pmem_btree_rects2 = ax2.bar(ind + 7*width, pmem_btree_get, width, color='k')

sll_rects3 = ax3.bar(ind, sll_mem, width, color='b')
vector_rects3 = ax3.bar(ind + width, vector_mem, width, color='r')
lsm_rects3 = ax3.bar(ind + 2*width, lsm_mem, width, color='#aaaaaa')
btree_rects3 = ax3.bar(ind + 3*width, btree_mem, width, color='y')
pmem_sll_rects3 = ax3.bar(ind + 4*width, pmem_sll_mem, width, color='c')
pmem_vector_rects3 = ax3.bar(ind + 5*width, pmem_vector_mem, width, color='m')
pmem_lsm_rects3 = ax3.bar(ind + 6*width, pmem_lsm_mem, width, color='g')
pmem_btree_rects3 = ax3.bar(ind + 7*width, pmem_btree_mem, width, color='k')

# add some text for labels, title and axes ticks

ax1.set_yscale('log')
ax2.set_yscale('log')
ax3.set_yscale('log')
ax1.set_ylabel('Time for Various Insertion calls [us]')
ax2.set_ylabel('Time for Various GetElement calls [us]')
ax3.set_ylabel('Dyanmic Memory Usage for Various Insertions [B]')
ax1.set_title('Insertion() time')
ax2.set_title('Get() time')
ax3.set_title('Dynamic Memory Usage')
ax1.set_xticks(ind + width / 2)
ax2.set_xticks(ind + width / 2)
ax3.set_xticks(ind + width / 2)
ax1.set_xticklabels(('L 40', 'D 40', 'L 400', \
	'D 400', 'L 4000', 'D 4000'))
ax2.set_xticklabels(('L 40', 'D 40', 'L 400', \
	'D 400', 'L 4000', 'D 4000'))
ax3.set_xticklabels(('L 40 op', 'D 40', 'L 400', \
	'D 400', 'L 4000', 'D 4000'))

ax1.legend((sll_rects1[0], vector_rects1[0], lsm_rects1[0], btree_rects1[0], \
		pmem_sll_rects1[0], pmem_vector_rects1[0], pmem_lsm_rects1[0], pmem_btree_rects1[0]), \
		('SLL', 'Vector', 'LSM', 'BTree', \
		'PMEM SLL', 'PMEM Vector', 'PMEM LSM', 'PMEM BTree'))

ax2.legend((sll_rects2[0], vector_rects2[0], lsm_rects2[0], btree_rects2[0], \
		pmem_sll_rects2[0], pmem_vector_rects2[0], pmem_lsm_rects2[0], pmem_btree_rects2[0]), \
		('SLL', 'Vector', 'LSM', 'BTree', \
		'PMEM SLL', 'PMEM Vector', 'PMEM LSM', 'PMEM BTree'))

ax3.legend((sll_rects3[0], vector_rects3[0], lsm_rects3[0], btree_rects3[0], \
		pmem_sll_rects3[0], pmem_vector_rects3[0], pmem_lsm_rects3[0], pmem_btree_rects3[0]), \
		('SLL', 'Vector', 'LSM', 'BTree', \
		'PMEM SLL', 'PMEM Vector', 'PMEM LSM', 'PMEM BTree'))




def autolabel(rects, ax):
	"""
	Attach a text label above each bar displaying its height
	"""
	if True:
		return
	for rect in rects:
		height = rect.get_height()
		ax.text(rect.get_x() + rect.get_width()/2., 1.05*height, \
			'%d' % int(height), \
			ha='center', va='bottom')

autolabel(sll_rects1, ax1)
autolabel(vector_rects1, ax1)
autolabel(lsm_rects1, ax1)
autolabel(btree_rects1, ax1)
autolabel(pmem_sll_rects1, ax1)
autolabel(pmem_vector_rects1, ax1)
autolabel(pmem_lsm_rects1, ax1)
autolabel(pmem_btree_rects1, ax1)

autolabel(sll_rects2, ax2)
autolabel(vector_rects2, ax2)
autolabel(lsm_rects2, ax2)
autolabel(btree_rects2, ax2)
autolabel(pmem_sll_rects2, ax2)
autolabel(pmem_vector_rects2, ax2)
autolabel(pmem_lsm_rects2, ax2)
autolabel(pmem_btree_rects2, ax2)

autolabel(sll_rects3, ax3)
autolabel(vector_rects3, ax3)
autolabel(lsm_rects3, ax3)
autolabel(btree_rects3, ax3)
autolabel(pmem_sll_rects3, ax3)
autolabel(pmem_vector_rects3, ax3)
autolabel(pmem_lsm_rects3, ax3)
autolabel(pmem_btree_rects3, ax3)

plt.show()
