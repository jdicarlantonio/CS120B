# Test file for lab3_part3


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Add tests below
test "PINA: 0x30 => PORTC: 0xC0"
setPINA 0x30
continue 2
expectPORTC 0xC0
checkResult

test "PINA: 0x31 => PORTC: 0xE0"
setPINA 0x31
continue 2
expectPORTC 0xE0
checkResult

test "PINA: 0x34 => PORTC: 0xF0"
setPINA 0x34
continue 2
expectPORTC 0xF0
checkResult

test "PINA: 0x36 => PORTC: 0xB8"
setPINA 0x36
continue 2
expectPORTC 0xB8
checkResult

test "PINA: 0x39 => PORTC: 0xB8"
setPINA 0x36
continue 2
expectPORTC 0xB8
checkResult

test "PINA: 0x3C => PORTC: 0xBE"
setPINA 0x3C
continue 2
expectPORTC 0xBE
checkResult

test "PINA: 0x3F => PORTC: 0xBF"
setPINA 0x3F
continue 2
expectPORTC 0xBF
checkResult

test "PINA: 0x02 => PORTC: 0x60"
setPINA 0x02
continue 2
expectPORTC 0x60
checkResult

test "PINA: 0x06 => PORTC: 0x38"
setPINA 0x06
continue 2
expectPORTC 0x38
checkResult

test "PINA: 0x0D => PORTC: 0x3F"
setPINA 0x0D
continue 2
expectPORTC 0x3F
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
