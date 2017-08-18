#!/bin/sh

find . -name '*.errs' | xargs rm -f
find . -name '*.out' | xargs rm -f
find . -name '*.time' | xargs rm -f
find . -name '*.bc' | xargs rm -f
find . -name '*.ll' | xargs rm -f
