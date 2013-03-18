#
# Defines project name
#
PROJECT=tp2_arm

#
# Defines the part type that this project uses.
#
PART=LM4F120H5QR

#
# Set the processor variant.
#
VARIANT=cm4f

#
# The base directory for StellarisWare.
#
ROOT=stellaris

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=${ROOT}/boards/ek-lm4f120xl/drivers
VPATH+=${ROOT}/utils

#
# Where to find header files that do not live in the source directory.
#
IPATH=${ROOT}/boards/ek-lm4f120xl
IPATH+=${ROOT}

#
# The default rule, which causes the Project Zero Example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/${PROJECT}.axf

#
# Rule to flash lm4f120xl
#
flash: all
	sudo lm4flash ${COMPILER}/${PROJECT}.bin

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building.
#
${COMPILER}/${PROJECT}.axf: ${COMPILER}/${PROJECT}.o
${COMPILER}/${PROJECT}.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/${PROJECT}.axf: ${ROOT}/driverlib/${COMPILER}-cm4f/libdriver-cm4f.a
${COMPILER}/${PROJECT}.axf: ${PROJECT}.ld
SCATTERgcc_${PROJECT}=${PROJECT}.ld
ENTRY_${PROJECT}=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA2

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
