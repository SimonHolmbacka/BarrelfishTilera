#!/bin/sh

#
# This shell script is called from symbolic_targets.mk.
# It parses menu.lst, returns the module list, passes module addresses to tile-mkboot to create bootrom file.
#
# @param $1, this is the module menu list, we need to trim it
# @param $2, this is hypervisor configuration file for tile-mkboot
# @param $3, kernel file for tile-mkbook
# @param $4, bootrom file to be generated
#

# check the number of arguments
if [ $# -ne 4 ] ; then
   echo "ERROR! tilepro-mkboot-menulist.sh failed! The number of argument is incorrect!"
   exit 1
fi

# define the new parsed module menu list named "menu.lst.modules"
TILEPRO_MODULES=$1.modules

# put "./tilepro/menu.lst.modules" at the beginning of this file
echo ./${TILEPRO_MODULES} > ${TILEPRO_MODULES}

# parse the menu list, removing all the comments and space and so on.
cat $1 | \
sed -n -e 's/#.*//' \
    -e 's/[ \t]\+/ /g' \
    -e 's/ $//;s/^ //' \
    -e '/^\(module\|modulenounzip\)/ { s@ *\(module\|modulenounzip\) /?*\(.*\)@./\2@ ; p }' >> ${TILEPRO_MODULES}

# remove the words after the first space, save it into a variable
MKBOOT_MODULES=`sed -e 's/ .*//' ${TILEPRO_MODULES}`

# truncate the file into one line, save it in a variable
#MKBOOT_MODULES=`tr -s '\n' ' ' < ${TILEPRO_MODULES}`

echo "MKBOOT MODULES: " ${MKBOOT_MODULES}

# call tile-mkboot to make bootrom file
TILE_MKBOOT="tile-mkboot -o $4 --hvc $2 kernel=$3 ${MKBOOT_MODULES}"
echo $TILE_MKBOOT
eval $TILE_MKBOOT

# check if the shell script exits correctly
if [ $? -ne 0 ] ; then
   echo "ERROR! tilepro-mkboot-menulist.sh failed to exit!"
   exit 1
fi

