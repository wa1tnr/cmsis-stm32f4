#!/bin/sh
# _update_this_directory.sh

# $  ls -1 *FREEZE
# print_logo.sh__FREEZE
# show2K1Odyssey.txt__FREEZE
# showAardvark-a.txt__FREEZE
# showFancyChicken.txt__FREEZE
# showLightBulb.txt__FREEZE

cp -p print_logo.sh__FREEZE print_logo.sh
cp -p show2K1Odyssey.txt__FREEZE   show2K1Odyssey.txt
cp -p showAardvark-a.txt__FREEZE   showAardvark-a.txt
cp -p showFancyChicken.txt__FREEZE   showFancyChicken.txt
cp -p showLightBulb.txt__FREEZE      showLightBulb.txt

echo
echo
echo please manually cd ..  (change directory up one level)
echo
echo now type: ; echo

cat << _EOF__

  cp -p Makefile__FROZEN Makefile

_EOF__

echo making sure that __FROZEN version of the Makefile
echo already exists prior to the attempt to copy it over.

echo

# end
