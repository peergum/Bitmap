all: photon.bin electron.bin

electron.bin: examples/usage/usage.ino
	particle compile electron $< --saveTo electron.bin

photon.bin: examples/usage/usage.ino
	particle compile photon $< --saveTo photon.bin
