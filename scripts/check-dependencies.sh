#!/bin/sh

# ARCH should be set on the command line to the target architecture

checkExists () {
    echo -n "Checking for $1: "
    if command -v $1 > /dev/null 2>&1 ; then
        command -v $1
        return 0
    else
        echo "Not found"
        return 1
    fi
}

set -e

checkExists ${ARCH}-elf-g++
checkExists ${ARCH}-elf-ld
