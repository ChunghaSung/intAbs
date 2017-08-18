# Summary

Benchmark programs with script files to run tests

# Run test

- single run: ./run.sh + ``test directory name``
- run all: ./runAll.sh, it runs the test directories written in testdirs.sh

# Build tables

- To get a simple result table: ./makeTable_ncomb_box.sh OR ./makeTalbe_ncomb_box2.sh
- To get a table about how many pairs are filtered: ./makeTable_pairs.sh

# Description for test files

All files need ``priority.info`` file which indicates priority information of each routine

- test1~test6: small tests (self made)

- blink1~blink3: modified version of Blink LED Demo from  http://processors.wiki.ti.com/index.php/MSP430_LaunchPad_LED_Timer. Based on original files I created own version of LED demo.

- brake1~brake3: modified version of Brake benchmark programs from DATE15.

- logger1~logger3: modified version of Logger benchmark from DATE15.

- rcmain: I uploaded original file of rc_core from DATE15 paper. I modified the file to make interrupt routines with priority information as same as other files.

- rgbled, usbkbd, usbmouse: these files are from kernel code. And, I uploaded original files. I modified these files to test by getting interrupt routines with priority information as same as other files.

- ic2_pca_isa, i8xx_tco, sc1200wdt, wdt_pci: These files are from https://github.com/markus-kusano/watts, and I added priority information on them.

