all : CC     = gcc
all : CPP 	 = g++
all : OPENMP = -fopenmp
all : ARCH   = 
all : LIBS   = -lm -lgomp -lrt -ldl -lsqlite3
all : CFLAGS = -O2 -c -g -fmessage-length=0  -std=c++11 -Wunused-variable
all : TARGET = "CDFLM"

SUBDIRS  := $(wildcard ../) $(wildcard ../*/)
CPP_SRCS := $(wildcard ../*/*.cpp) $(wildcard ../*/*/*.cpp) 
C_SRCS   := $(wildcard ../*/*.c) $(wildcard ../*/*/*.c)
OBJS     := $(patsubst ../%.cpp, ./%.o, $(CPP_SRCS)) $(patsubst ../include/sqlite/%.c, ./include/sqlite/%.o, $(C_SRCS))
CPP_DEPS := $(patsubst ../%.cpp, ./%.d, $(CPP_SRCS))
RM := rm -rf

INCLUDE =-I./ -I../include -I../cfo 

print:
	@echo $(SUBDIRS)
	@echo ""
	@echo $(CPP_SRCS)
	@echo ""
	@echo $(C_SRCS)
	@echo ""
	@echo $(OBJS)
	@echo ""
	@echo $(CPP_DEPS)
	@echo ""
	@echo $(INCLUDE)
	@echo ""
	@echo $

# Add inputs and outputs from these tool invocations to the build variables 

all: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MinGW C++ Linker'
	$(CPP) -o $(TARGET) $(OPENMP) $(ARCH) $(OBJS) $(USER_OBJS) $(LIBS) 
	@echo 'Finished building target: $@'
	@echo ' '

include/%.o: ../include/%.cpp
	@mkdir -p $(@D)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CPP) $(INCLUDE) $(CFLAGS) $(OPENMP) $(ARCH) $(LIBS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@mkdir -p $(@D)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CPP) $(INCLUDE) $(CFLAGS) $(OPENMP) $(ARCH) $(LIBS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(CPP_DEPS)$(C_DEPS) CFO CFO.mic
	-@echo ' '

.PHONY: all clean dependents mic
.SECONDARY:

