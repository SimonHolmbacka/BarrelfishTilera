#!/bin/bash
#
# This script is intended to assure 2 things:
#
# 1) That the bootloader has only sections, which cannot be changed.
# The purpose is to assure that the bootloader can
# be shared by multiple cores.
#
# Only the stack (whichs position is core-dependent) and dynamically allocated
# data (also core-dependent) should be used.
#
# Maybe if needed, it could be checked that the writable sections contain only 
# well-known parameters.
#
# 2) That the position of the L2 page table is at the assumed position,
# so that links in the L1 page table to the L2 page table are correct.
#
# @param $1 the name of the linked bootloader file (of type EXEC)

IS_EXEC=`readelf -h $1 | grep EXEC`
if  [ -z "$IS_EXEC" ];
then
	echo No valid executable
	exit 2
fi

# nm $1 | grep "[0-9a-f]* [dD]" reads all symbols in the data section
# grep -v outputs all, except the match.
# So all known (harmless) symbols should be added here
#DATA_SECTION_CONTENTS=`nm $1 | grep "[0-9a-f]* [dD]"`
#DATA_SECTION_CONTENTS=$(nm $1 | grep "[0-9a-f]* [dD]")
#echo "Symbols in bootloader data section:"
#echo ${DATA_SECTION_CONTENTS%.}
FILTERED_SECTIONS=`nm $1 | grep "[0-9a-f]* [dD]" | grep -v kernel_elf_image | grep -v loader_l1 | grep -v loader_l2 | grep -v mem_l2_check`
if  [ -n "$FILTERED_SECTIONS" ];
then
	echo "ERROR: Found unknown writable symbol(s) in data section of bootloader ($1): $FILTERED_SECTIONS"
	exit 1
fi

echo "Mem check:"
nm $1 | grep "00000000 d mem_l2_check"
RESULT=$?
if [ $RESULT != 0 ]
then
	echo "\"mem_l2_check\" not found or does not contain 0x0. Check the definition in loader.S & mem_loc.h"
	exit 1
fi
exit 0

