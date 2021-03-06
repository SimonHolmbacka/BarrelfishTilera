#!/bin/bash
#
# This script is intended to build a folder with different links to other 
# files and folders files in $TILERA_ROOT/tile/usr/include.
#
# The tilepro-build includes these folders. Therefore it
# assumes that the variable $TILE_IMPORTS points to the folder
# generated by this script.
#
# This script builds around the fact that including
# $TILERA_ROOT/tile/usr/include into the build breaks on
# stdio.h - so we only use .h-files which are linked there.
#
# When completed successfully there should be only a
# line printing the success of this script.
#

SUBDIR=tile_imports
TARGET=$1
if [ -z "$TARGET" ]
then
	echo "Usage: \"$0 dir\" where dir is the directory under which the folder \"$SUBDIR\" should be created."
	exit 1
fi
if [ ! -d "$TARGET" ]
then
	echo "Error: Directory \"$TARGET\" does not exist!"
	exit 2
fi
if [ ! -d "$TILERA_ROOT" ]
then
	echo "Error: Variable \"TILERA_ROOT\" not set or not pointing to a directory!"
	exit 4
fi
if [ -d $SUBDIR ]
then
	echo "Error: Directory \"$SUBDIR\" does already exist in \"$TARGET\"!"
	exit 4
fi

# So we can pop back to the current directory and are free to switch until thens
pushd . > /dev/null

mkdir $SUBDIR
cd $SUBDIR
ln -s ${TILERA_ROOT}/tile/usr/lib/gcc/tile-unknown-linux-gnu/4.4.3/include/feedback.h feedback.h
mkdir arch
cd arch
ln -s ${TILERA_ROOT}/tile/usr/include/arch/abi.h abi.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/spr_def.h spr_def.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/sim_def.h sim_def.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/interrupts.h interrupts.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/spr.h spr.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/inline.h inline.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/udn.h udn.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/sn.h sn.h
ln -s ${TILERA_ROOT}/tile/usr/include/arch/idn.h idn.h

popd > /dev/null

echo "Successfully created dir \"${TARGET}/${SUBDIR}\"!"
