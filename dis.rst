=======
dis6502
=======

------------------------------
flow-tracing 6502 disassembler
------------------------------

.. |date| date::
.. |time| date:: %H:%M

:Manual section: 1

:Authors: Robert Bond; Udi Finkelstein; Eric Smith; Peter H. Froehlich.
:Date: |date| |time|
:Version: 0.13
:Copyright: This document is licensed under the same terms as dis6502
            itself, see the file COPYING for details.

SYNOPSIS
========

dis6502 (-b|-l|-c|-r address) [-a|-7|-v address] {-p file} object-file

DESCRIPTION
===========

dis6502 disassembles 6502 binary files. Binary formats understood include
Atari binary load files (L menu command) and boot files, Commodore 64
program files, and raw binary files. Equate and control files can be
included via the -p option to name well known locations and to control
the dissassembly process. The output includes a cross reference.

The dissassembly process is a two pass operation: First the program flow is
traced starting with the init and run parameters in the file headers. The
dump routine then prints out the information.

OPTIONS
=======

Formats
-------

-b
  Binary file is an Atari boot file.
-l
  Binary file is an Atari load file.
-c
  Binary file is a Commodore 64 program file.
-r address
  Binary file is raw 6502 code starting at the given address (in
  C-notation, e.g. 0xA000).

Options
-------

-p file
  Read in the given predefines file. Up to 20 -p options may be included.
-a
  Generate output suitable for reassembly.
-v address
  Use given address for 6502 NMI, RESET, IRQ/BRK vectors (default is 0x10000).
-7
  Mask character data to 7 bit.

Predefines Files
----------------

The predefines files are line-oriented similar to assembly files. Each line
can contain one directive as follows:

name .eq number (or name .equ number)
  Define given name to stand for given number. In the generated disassembly,
  the name will be used instead of the number as appropriate.

  If a data reference is made to an address that does not have an
  assigned name, but address-1 does, the reference will be disassembled
  as name+1. This is convenient for two-byte variables, particularly in
  zero page.

.stop number
  Stop the trace process at the given address.

.trace number
  Continue the trace process at the given address.

.jtab2 <addr>,<addr>,<count>
  table of code pointers, split into high and low byte tables

.rtstab <addr>,<addr>,<count>
  like .jtab2, but each entry contains the target address minus
  one, for use with RTS

Numbers may be decimal numbers or hexadecimal numbers. The first character
of a hexadecimal number must be "$". For example, "100" is the decimal number
100, "$100" is the hexadecimal number 100 which is 256 is decimal. Names are
sequences of letters and digits starting with a letter.

EXAMPLE
=======

| TODO

EXIT CODES
==========

dis6502 will exit with code `0` if it was able to complete the disassembly
and with code `1` if it encountered a problem. In the latter case it should
print an error message.

BUGS
====

Mostly features actually. Okay, just kidding. Yes, there probably
are bugs.

SEE ALSO
========

HISTORY
=======

dis6502 was originally developed by Robert Bond to disassemble Atari 8-bit
binary files. Robert posted dis6502 to the Usenet newsgroup net.sources on
October 9, 1986 and to comp.sources.unix on June 7, 1988. Udi Finkelstein
ported dis6502 to the Amiga, added support for Commodore 64 object files,
and posted it to comp.sources.amiga on November 4, 1988. These versions were
in the public domain.

Eric Smith worked on dis6502 in the 2000-2003 time frame. He added support
for raw binary files, alternate vector addresses, two-byte data labels in
the disassembly, the .equ, .jtab2, and .rtstab directives, and the -7 option.
He also removed the need for line numbers in the predefines file, replaced
the many recursive calls with a trace queue, and brought the source code up
to ANSI C standards. His version was released under a GPL v2 license.

Peter Froehlich picked up dis6502 in April 2015. So far he a has mostly done
internal cleanup work with the goal of bringing the source code up to C11,
but he continues to work on new features as well. His version is also under
GPL v2.
