#! /bin/sh
$EXTRACTRC --tag-group=none --tag=Title --tag=Description `find . -name "*.regexp"` > predefined-regexps.cpp
$XGETTEXT *.cpp *.h */*.cpp */*.h *.y -o $podir/kregexpeditor.pot
rm -f predefined-regexps.cpp
