#!/bin/bash
# Bison and Flex command lines :
flex -Pqregexp -ogen_qregexplexer.cpp qregexpparser.l
bison -d -p qregexp -o gen_qregexpparser.cc qregexpparser.y