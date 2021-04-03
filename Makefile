collatz: collatz.cpp
	g++ -Wall -Wextra -o collatz collatz.cpp

.PHONY: clean
clean:
	rm -f collatz 2>/dev/null
