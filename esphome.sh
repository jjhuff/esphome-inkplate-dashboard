#!/bin/bash
ARGS="-p 6052:6052"

if [ -c "/dev/ttyUSB0" ]; then
  ARGS+=" --device /dev/ttyUSB0"
fi

docker run --rm -it  -v "${PWD}":/config $ARGS esphome/esphome:2023.6.4 $@
