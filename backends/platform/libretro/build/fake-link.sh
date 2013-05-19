#!/bin/sh

# get last argument, which should be the output filename
for last; do true; done

$1 rcs $last `find -name *.o`
