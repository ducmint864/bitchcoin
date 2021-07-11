final:
	g++ bitchcoin.cpp -o ./bin/bitchcoin -lpthread -lcrypto -lssl && ./bin/bitchcoin
love:
	g++ bitchcoin.cpp -o ./bin/bitchcoin -lpthread -lcrypto -lssl -mtune=generic -march=znver2 -Ofast && ./bin/bitchcoin
testfile:
	g++ test.cpp -o ./bin/test -lcrypto -lssl && ./bin/test
testfile2:
	g++ test2.cpp -o ./bin/test2 && ./bin/test2
debug:
	g++ bitchcoin.cpp -o ./bin/bitchcoin -lpthread -lcrypto -lssl -g && gdb ./bin/bitchcoin
