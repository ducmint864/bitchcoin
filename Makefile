final:
	g++ bitchcoin.cpp -o bitchcoin -lpthread -lcrypto -lssl && ./bitchcoin
love:
	g++ bitchcoin.cpp -o bitchcoin -lpthread -lcrupto -lssl -mtune=generic -march=znver2 -Ofast && ./bitchcoin
testfile:
	g++ test.cpp -o test -lcrypto -lssl && ./test
testfile2:
	g++ test2.cpp -o test2 && ./test2
debug:
	g++ bitchcoin.cpp -o bitchcoin -lpthread -lcrypto -lssl -g && gdb bitchcoin
