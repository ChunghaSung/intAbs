#!/bin/bash


time ./cbmc --mm irq main.c > try$1.out

# Chungha: I manually ran this command for each property and wrote down time
