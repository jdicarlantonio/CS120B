# Test file for lab5_part3


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

# SAMPLE:
test "pressing button once"
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTC 0x01
checkResult

test "pressing button twice"
set count = 0x00
set state = INIT
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTC 0x02
checkResult

test "pressing button thrice"
set count = 0x00
set state = INIT
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTC 0x04
checkResult

test "pressing button after 14"
set count = 0x0E
set state = WAIT
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTC 0x00
checkResult

test "backwards sequence"
set count = 0x08
set state = WAIT
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTC 0x10
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
