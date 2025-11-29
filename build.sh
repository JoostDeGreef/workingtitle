#!/bin/bash

mkdir -p out
#(cd 3rdParty;./proper.sh)
(cd out;cmake ..;make)
