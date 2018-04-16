#!/bin/bash

if [ $# -lt 2 ]
then
  echo "Usage: ./upload <device> <.ino file>"
  exit 1
fi

arduino --board arduino:avr:uno --port $1 --upload $2
