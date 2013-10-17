ifndef TILERA_ROOT
$(error The 'TILERA_ROOT' environment variable is not set.)
endif

BF_ROOT = ../../../..
BF_INCL = $(BF_ROOT)/include/arch/tilepro/
BF_INCL_KERN = $(BF_ROOT)/kernel/include/arch/tilepro/
HV_DIR = $(TILERA_ROOT)/src/sys/hv

BIN = $(TILERA_ROOT)/bin/

CC = $(BIN)tile-cc
MONITOR = $(BIN)tile-monitor

CC_FLAGS = -std=c99 -I$(BF_INCL)

SIMULATOR_ARGS = \
  --simulator \
  --functional \
  --image 2x2 \
  --console
  
MONITOR_UPLOAD_RUN_ARGS = \
  --batch-mode \
  --mkdir /opt/testbench \
  --cd /opt/testbench \
  --upload $< $< \
  -- $<
  
all: build

build: testbench

hv_glue.S:
	perl $(HV_DIR)/mkgluesyms.pl $(HV_DIR)/include/hv/hypervisor.h > $@
	
hv_glue.o: hv_glue.S
	$(CC) -c -o $@ $<

endian_test.o: endian_test.c testcase.h $(BF_INCL)/machine/endian.h
	$(CC) $(CC_FLAGS) -c -o $@ $<

asm_inlines_test.o: asm_inlines_test.c testcase.h $(BF_INCL)/barrelfish_kpi/asm_inlines_arch.h
	$(CC) $(CC_FLAGS) -c -o $@ $<
	
phys_mem.o: ../phys_mem.c test_macros.h
	$(CC) $(CC_FLAGS) -I$(HV_DIR)/include -imacros test_macros.h -c -o $@ $<
	
phys_mem_test.o: phys_mem_test.c testcase.h $(HV_DIR)/include $(BF_ROOT)/include $(BF_INCL_KERN)
	$(CC) $(CC_FLAGS) -I$(HV_DIR)/include -I$(BF_ROOT)/include -I$(BF_INCL_KERN) -imacros test_macros.h -c -o $@ $<

test_panic.o: test_panic.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

testcase.o: testcase.c testcase.h
	$(CC) $(CC_FLAGS) -c -o $@ $<

testbench: testbench.c testcase.o test_panic.o hv_glue.o endian_test.o asm_inlines_test.o phys_mem_test.o phys_mem.o
	@echo Building test bench
	$(CC) $(CC_FLAGS) -o $@ $< testcase.o test_panic.o hv_glue.o endian_test.o asm_inlines_test.o phys_mem_test.o phys_mem.o
	
test: testbench
	@echo Starting $@...
	$(MONITOR) $(SIMULATOR_ARGS) $(MONITOR_UPLOAD_RUN_ARGS)
	@echo Simulation done
	
clean:
	rm -f endian_test.o
	rm -f asm_inlines_test.o
	rm -f phys_mem_test.o phys_mem.o
	rm -f hv_glue.o hv_glue.S
	rm -f test_panic.o 
	rm -f testcase.o
	rm -f testbench
	

.PHONY: test clean
