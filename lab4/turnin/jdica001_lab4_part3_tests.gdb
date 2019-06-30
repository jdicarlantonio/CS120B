# Test file for lab4_part3


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

# correct sequence entered
test "PINA: 0x04, PINA: 0x00, PINA: 0x02, PINA: 0x00 => PORTB: 0x01, state: UNLOC"
setPINA 0x04
continue 5
setPINA 0x00
continue 5
setPINA 0x02
continue 5
setPINA 0x00
continue 5
expectPORTB 0x01
expect state UNLOCK
checkResult

# door lock by pressing inHouse
test "PINA: 0x80, PINA: 0x00 => PORTB: 0x00, state: WAIT"
set state = WAIT
setPINA 0x80
continue 5
setPINA 0x00
continue 5
expectPORTB 0x00
expect state WAIT
checkResult

# incorrect sequence
test "PINA: 0x01, PINA: 0x00, PINA: 0x02, PINA: 0x00 => PORTB: 0x00, state: WAIT"
setPINA 0x01
continue 5
setPINA 0x00
continue 5
setPINA 0x02
continue 5
setPINA 0x00
continue 5
expectPORTB 0x00
expect state WAIT
checkResult

# incorrect button after sharp
test "PINA: 0x04, PINA: 0x00, PINA: 0x01, PINA: 0x00 => PORTB: 0x00, state: WAIT"
setPINA 0x04
continue 5
setPINA 0x00
continue 5
setPINA 0x01
continue 5
setPINA 0x00
continue 5
expectPORTB 0x00
expect state WAIT
checkResult

# multiple buttons pressed at once
test "PINA: 0x06, PINA: 0x00 => PORTB: 0x00, state: WAIT"
setPINA 0x06
continue 5
setPINA 0x00
continue 5
expectPORTB 0x00
expect state WAIT
checkResult

# all three buttons pressed at once
test "PINA: 0x07, PINA: 0x00 => PORTB: 0x00, state: WAIT"
setPINA 0x07
continue 5
setPINA 0x00
continue 5
expectPORTB 0x00
expect state WAIT
checkResult

# holding down sharpt and simultaneously pressing Y
test "PINA: 0x04, PINA: 0x06 => PORTB: 0x00, state: WAIT"
setPINA 0x04
continue 5
setPINA 0x06
continue 5
expectPORTB 0x00
expect state WAIT
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
