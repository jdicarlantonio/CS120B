# Test file for lab6_part2


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
test "LED1"
continue 
expectPORTB 0x01
expect state LED1
checkResult

test "LED2"
continue 
expectPORTB 0x02
expect state LED2
checkResult

test "LED3"
continue 
expectPORTB 0x04
expect state LED3
checkResult

test "button press on LED3"
setPINA 0xFE
continue 5
expectPORTB 0x04
checkResult

test "button press on LED1"
setPINA 0xFF
continue
setPINA 0xFE
continue
setPINA 0xFF
continue
setPINA 0xFE
continue 5
expectPORTB 0x01
checkResult

test "button press on LED2"
setPINA 0xFF
continue
setPINA 0xFE
continue
setPINA 0xFF
continue 2
setPINA 0xFE
continue 5
expectPORTB 0x02
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
