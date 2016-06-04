# Hardware project

This project is part of an enginnering school programming project and aims to built a GPS software of an embedded system: compass, favorites places, waypoints, navigation, etc.

## Project

Hardware used:

+ Texas Instrument MSP430F169
+ Adafruit Ultimate GPS Breakout
+ 4D Systems uOLED-128-G2

A presentation of the project is available in [French](http://gaelfoppolo.github.io/gps/presentation).

## Documentation

An [HTML](http://gaelfoppolo.github.io/gps/) documentation is available, with graph to a better understanding of the process (see [Files](http://gaelfoppolo.github.io/gps/files.html) tabs).

An [PDF](http://gaelfoppolo.github.io/gps/latex/refman.pdf) (LaTex) documentation is also available.

To update the documentation, make sure you have installed [Doxygen](http://www.stack.nl/~dimitri/doxygen/) and [Graphviz](http://www.graphviz.org/).

Then browse to the root of the project folder and run: ``doxygen doxygen.cfg``

To update the LaTEX documentation, browse ``doc/latex`` and run ``make``

## Usage and options

To compile the project, you'll need [CrossWorks for MSP430](http://www.rowley.co.uk/msp430/releases.htm).

Open ``project.hzp`` to launch project. See [CrossWorks for MSP430 Reference Manual](http://cdn.rowleydownload.co.uk/msp430/documentation/msp430_crossworks_reference_manual.pdf) for further explanations.