# Test file for lab4_part2


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

# no buttons pressed
test "PINA: 0x00 => PORTC: 0x07, state: WAIT"
set value = 0x07
setPINA 0x00
continue 5
expectPORTC 0x07
expect state WAIT
checkResult

# increment button pressed and released
test "PINA: 0x01, PINA: 0x00 => PORTC: 0x08, state: WAIT"
set value = 0x07
set state = WAIT
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTC 0x08
expect state WAIT
checkResult 

# decrement button pressed and released
test "PINA: 0x02, PINA: 0x00 => PORTC: 0x06, state: WAIT"
set value = 0x07
set state = WAIT
setPINA 0x02
continue 5
setPINA 0x00
continue 5
expectPORTC 0x06
expect state WAIT
checkResult 

# increment button pressed and held
test "PINA: 0x01 => PORTC: 0x08, state: INCREMENT"
set value = 0x07
set state = WAIT
setPINA 0x01
continue 5
expectPORTC 0x08
expect state INCREMENT
checkResult 

# decrement button pressed and held
test "PINA: 0x02 => PORTC: 0x06, state: DECREMENT"
set value = 0x07
set state = WAIT
setPINA 0x02
continue 5
expectPORTC 0x06
expect state DECREMENT
checkResult 

# both buttons set simultaneously (spell check) and released simultaneously
test "PINA: 0x03, PINA: 0x00 => PORTC: 0x00, state: WAIT"
set value = 0x07
set state = WAIT
setPINA 0x03
continue 5
setPINA 0x00
continue 5
expectPORTC 0x00
expect state WAIT
checkResult

# increment pressed followed by decrement while increment is still down and then released
test "PINA: 0x01, PINA: 0x03, PINA: 0x00 => PORTC: 0x00, state: WAIT"
set value = 0x07
set state = WAIT
setPINA 0x01
continue 5
setPINA 0x03
continue 5
setPINA 0x00
continue 5
expectPORTC 0x00
expect state WAIT
checkResult

# decrement pressed followed by increment while decrement is still down and then released
test "PINA: 0x02, PINA: 0x03, PINA: 0x00 => PORTC: 0x00, state: WAIT"
set value = 0x07
set state = WAIT
setPINA 0x02
continue 5
setPINA 0x03
continue 5
setPINA 0x00
continue 5
expectPORTC 0x00
expect state WAIT
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
