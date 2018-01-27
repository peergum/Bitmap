all: ${PARTICLE_PLATFORM}.bin

${PARTICLE_PLATFORM}.bin: examples/usage/usage.ino src/Bitmap.*
	echo -e "${PARTICLE_LOGIN}\n${PARTICLE_PASS}"|particle login
	particle compile ${PARTICLE_PLATFORM} $< --saveTo ${PARTICLE_PLATFORM}.bin
