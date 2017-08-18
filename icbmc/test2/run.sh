#!/bin/bash

TIME="`which time` -f %e"

MAX_TIME=30m

TIMEOUT="timeout $MAX_TIME"

$TIMEOUT $TIME ./cbmc $1

