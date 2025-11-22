#!/bin/bash

rm -rf googletest 
rm -rf src
rm -rf tmp
rm -f BuildNumber.txt

find . -name "*~" -exec rm -f "{}" \;

