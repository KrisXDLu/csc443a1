import os.path
import subprocess
import matplotlib.pyplot as plt

filename = "experiment.txt"
byte_size = 134217728 # 128mb
block_range = ['100', '200', '500', '1000', 
				'5000', '10000', '100000', 
				'1048576','2097152','3145728'
				]

# run c and get output
# TODO: 1.2
def write_experiment():
	for block_size in block_range:
		# output = 0
		# for i in range(10):
			# output += 
		output = subprocess.getoutput("./create_random_file " + filename + " " + str(byte_size) + " " + block_size)
		# output = output/10
		print(output)

def read_experiment():
	for block_size in block_range:
		# output = 0
		# for i in range(10):
		# 	output +=
		output = subprocess.getoutput("./get_histogram " + filename + " " + block_size)
		# output = output/10
		print(output)

def main():
    if not os.path.isfile("create_random_file"):
        print("Must have program 'create_random_file' in the current directory!")
        exit()
    elif not os.path.isfile("get_histogram"):
        print("Must have program 'create_random_file' in the current directory!")
        exit()
    # write_experiment()
    # read_experiment()
    # write
	# block_size = ['100', '200', '500', '1000','5000', '10000', '100000', '1048576','2097152','3145728']
	# times = [134217728/3430, 134217728/1804, 
	# 			134217728/843, 134217728/523, 
	# 			134217728/325, 134217728/270, 
	# 			134217728/248, 134217728/249, 
	# 			134217728/240, 134217728/259
	# 			]
	# plt.figure()
	# plt.plot(block_size, times, 'bo-')
	# plt.title('Block size vs. Write data rate')
	# plt.xlabel('Block Size(bytes)')
	# plt.ylabel('Rate(kB/ms)')
	# plt.savefig('write_experiment.png')
	# plt.close()

	# read
    block_size = ['100', '200', '500', '1000','5000', '10000', '100000', '1048576','2097152','3145728']
    rate = [134217800/245, 134217800/158, 
				134217800/116, 134217800/91, 
				134217800/77, 134217800/50, 
				134217800/41, 134217800/21, 
				134217800/35, 134217800/28
				]
    plt.figure()
    plt.plot(block_size, rate, 'bo-')
    plt.title('Block size vs. Read data rate')
    plt.xlabel('Block Size(bytes)')
    plt.ylabel('Rate(kB/ms)')
    plt.savefig('read_experiment.png')
    plt.close()

if __name__ == "__main__":
    main()

# write
# Block size 100 bytes
# Total bytes 134217728 bytes
# Time 3430 millisec
# Block size 200 bytes
# Total bytes 134217728 bytes
# Time 1804 millisec
# Block size 500 bytes
# Total bytes 134217728 bytes
# Time 843 millisec
# Block size 1000 bytes
# Total bytes 134217728 bytes
# Time 523 millisec
# Block size 5000 bytes
# Total bytes 134217728 bytes
# Time 325 millisec
# Block size 10000 bytes
# Total bytes 134217728 bytes
# Time 270 millisec
# Block size 100000 bytes
# Total bytes 134217728 bytes
# Time 248 millisec
# Block size 1048576 bytes
# Total bytes 134217728 bytes
# Time 249 millisec
# Block size 2097152 bytes
# Total bytes 134217728 bytes
# Time 240 millisec
# Block size 3145728 bytes
# Total bytes 134217728 bytes
# Time 259 millisec


# read
# Computed the histogram in 245 ms.
# BLOCK SIZE 100 bytes
# TOTAL BYTES 134217800 bytes
# TIME 245 milliseconds
# Computed the histogram in 158 ms.
# BLOCK SIZE 200 bytes
# TOTAL BYTES 134217800 bytes
# TIME 158 milliseconds
# Computed the histogram in 116 ms.
# BLOCK SIZE 500 bytes
# TOTAL BYTES 134217800 bytes
# TIME 116 milliseconds
# Computed the histogram in 91 ms.
# BLOCK SIZE 1000 bytes
# TOTAL BYTES 134217800 bytes
# TIME 91 milliseconds
# Computed the histogram in 77 ms.
# BLOCK SIZE 5000 bytes
# TOTAL BYTES 134217800 bytes
# TIME 77 milliseconds
# Computed the histogram in 50 ms.
# BLOCK SIZE 10000 bytes
# TOTAL BYTES 134217800 bytes
# TIME 50 milliseconds
# Computed the histogram in 41 ms.
# BLOCK SIZE 100000 bytes
# TOTAL BYTES 134217800 bytes
# TIME 41 milliseconds
# Computed the histogram in 21 ms.
# BLOCK SIZE 1048576 bytes
# TOTAL BYTES 134217800 bytes
# TIME 21 milliseconds
# Computed the histogram in 35 ms.
# BLOCK SIZE 2097152 bytes
# TOTAL BYTES 134217800 bytes
# TIME 35 milliseconds
# Computed the histogram in 28 ms.
# BLOCK SIZE 3145728 bytes
# TOTAL BYTES 134217800 bytes
# TIME 28 milliseconds