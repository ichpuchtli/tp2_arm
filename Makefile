# Defines project name
PROJECT=tp2_arm

# Defines project objects
TP2_ARM_OBJECTS=taskq.o

# Defines the part type that this project uses.
PART=LM4F120H5QR

# Set the processor variant.
VARIANT=cm4f

# The base directory for StellarisWare.
ROOT=stellaris

# Include the common make definitions.
include ${ROOT}/makedefs

# Where to find source files that do not live in this directory.
VPATH=${ROOT}/boards/ek-lm4f120xl/drivers
VPATH+=${ROOT}/utils

# Where to find header files that do not live in the source directory.
IPATH=${ROOT}/boards/ek-lm4f120xl
IPATH+=${ROOT}

# The default rule, which causes the Project Zero Example to be built.
all: ${TP2_ARM_OBJECTS} size

clean:
	@rm -rf *.o *.d


# The rule to clean out all the build products.
size:
	@arm-none-eabi-size -t ${TP2_ARM_OBJECTS}

CFLAGSgcc=-DTARGET_IS_BLIZZARD_RA2

# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
