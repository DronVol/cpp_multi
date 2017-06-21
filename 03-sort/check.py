import numpy as np

arr_init = np.fromfile('random', dtype=np.int32)
arr = np.fromfile('result', dtype=np.int32)

print "length: {0} : {1}".format(len(arr_init),len(arr)) 
try:
	print "check: {0} errors".format(np.sum(np.abs(arr - np.sort(arr_init))))
except:
	print "wrong lengths"