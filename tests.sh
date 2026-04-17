##############                  ##############
##############                  ##############
##############                  ##############
##############  IRC TEST SUITE  ##############
##############                  ##############
##############                  ##############
##############                  ##############

EXECUTABLE="ircserv"
PORT="6667"
PASS="pass"
ADDR="0.0.0.0"

SERVER_PID=""

start_server() {
    ./$EXECUTABLE "$PORT" "$PASS" &
    SERVER_PID=$!
    sleep 1
}

stop_server() {
    if [ -n "$SERVER_PID" ]; then
        kill "$SERVER_PID" 2>/dev/null
        wait "$SERVER_PID" 2>/dev/null
    fi
}

run_test() {
    NAME="$1"
    COMMANDS="$2"
    EXPECTED="$3"
#   -C replaces \n with \r\n for us
    RESULT=$(printf "%b" "$COMMANDS" | nc -w 1 -C "$ADDR" "$PORT")

    if printf "%s" "$RESULT" | grep -q "$EXPECTED"; then
        printf "%s: OK\n" "$NAME"
    else
        printf "%s: KO\n" "$NAME"
        printf "Expected: %s\n" "$EXPECTED"
        printf "Got: %s\n" "$RESULT"
    fi
}

cleanup() {
    stop_server
}

trap cleanup EXIT INT TERM

start_server

run_test "461 PASS need more params" "PASS\n" "461 .* PASS :Not enough parameters"
run_test "464 PASS wrong password" "PASS wrongpass\n" "464 .* :Password incorrect"
run_test "431 NICK no nickname given" "PASS $PASS\nNICK\n" "431 .* :No nickname given"
run_test "432 NICK erroneous nickname" "PASS $PASS\nNICK inv@lid\n" "432 .* inv@lid :Erroneous nickname"
run_test "461 USER need more params" "PASS $PASS\nNICK nick1\nUSER user 0 *\n" "461 nick1 USER :Not enough parameters"
run_test "462 PASS already registered" "PASS $PASS\nNICK nick2\nUSER user 0 * :Real Name\nPASS $PASS\n" "462 nick2 :You may not reregister"
run_test "462 USER already registered" "PASS $PASS\nNICK nick3\nUSER user 0 * :Real Name\nUSER user 0 * :Other Name\n" "462 nick3 :You may not reregister"
run_test "411 PRIVMSG no recipient" "PASS $PASS\nNICK nick4\nUSER user 0 * :Real Name\nPRIVMSG\n" "411 nick4 :No recipient given (PRIVMSG)"
run_test "412 PRIVMSG no text" "PASS $PASS\nNICK nick5\nUSER user 0 * :Real Name\nPRIVMSG nick2\n" "412 nick5 :No text to send"
run_test "401 PRIVMSG no such nick" "PASS $PASS\nNICK nick6\nUSER user 0 * :Real Name\nPRIVMSG ghost :hello\n" "401 nick6 ghost :No such nick/channel"
run_test "403 PRIVMSG no such channel" "PASS $PASS\nNICK nick7\nUSER user 0 * :Real Name\nPRIVMSG #ghost :hello\n" "403 nick7 #ghost :No such channel"
run_test "461 JOIN need more params" "PASS $PASS\nNICK nick8\nUSER user 0 * :Real Name\nJOIN\n" "461 nick8 JOIN :Not enough parameters"
run_test "461 PART need more params" "PASS $PASS\nNICK nick10\nUSER user 0 * :Real Name\nPART\n" "461 nick10 PART :Not enough parameters"
run_test "403 PART no such channel" "PASS $PASS\nNICK nick11\nUSER user 0 * :Real Name\nPART #ghost\n" "403 nick11 #ghost :No such channel"
run_test "461 INVITE need more params" "PASS $PASS\nNICK nick13\nUSER user 0 * :Real Name\nINVITE nick2\n" "461 nick13 INVITE :Not enough parameters"
run_test "403 INVITE no such channel" "PASS $PASS\nNICK nick14\nUSER user 0 * :Real Name\nINVITE nick2 #ghost\n" "403 nick14 #ghost :No such channel"
run_test "401 INVITE no such nick" "PASS $PASS\nNICK nick15\nUSER user 0 * :Real Name\nJOIN #chan15\nINVITE ghost #chan15\n" "401 nick15 ghost :No such nick/channel"
run_test "461 KICK need more params" "PASS $PASS\nNICK nick16\nUSER user 0 * :Real Name\nKICK #chan16\n" "461 nick16 KICK :Not enough parameters"
run_test "403 KICK no such channel" "PASS $PASS\nNICK nick17\nUSER user 0 * :Real Name\nKICK #ghost nick2\n" "403 nick17 #ghost :No such channel"
run_test "461 MODE need more params" "PASS $PASS\nNICK nick19\nUSER user 0 * :Real Name\nMODE\n" "461 nick19 MODE :Not enough parameters"
run_test "403 MODE no such channel" "PASS $PASS\nNICK nick20\nUSER user 0 * :Real Name\nMODE #ghost +i\n" "403 nick20 #ghost :No such channel"
run_test "472 MODE unknown mode" "PASS $PASS\nNICK nick21\nUSER user 0 * :Real Name\nJOIN #chan21\nMODE #chan21 +z\n" "472 nick21 z :is unknown mode char to me"

stop_server
