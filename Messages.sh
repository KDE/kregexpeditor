#! /bin/sh
perl extractrc-from-regexp `find . -name "*.regexp"` > predefined-regexps.cpp
$XGETTEXT *.cpp *.h */*.cpp */*.h -o $podir/kregexpeditor.pot
rm -f predefined-regexps.cpp
