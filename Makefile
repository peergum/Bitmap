all: ${PARTICLE_PLATFORM}.bin

${PARTICLE_PLATFORM}.bin: examples/usage/usage.ino src/Bitmap.*
	particle compile ${PARTICLE_PLATFORM} $< --saveTo ${PARTICLE_PLATFORM}.bin
