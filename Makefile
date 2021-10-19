final:
	powershell g++ bitchcoin.cpp -o ./bin/bitchcoin.exe -lpthread -lcrypto -lssl; ./bin/bitchcoin.exe
Windows:
	powershell g++ ./bitchcoin.cpp -o ./bin/bitchcoin.exe -lpthread -lcrypto -lssl; ./bin/bitchcoin.exe
love:
	powershell g++ ./bitchcoin.cpp -o ./bin/bitchcoin.exe -lpthread -lcrypto -lssl -mtune=generic -march=znver2 -Ofast; ./bin/bitchcoin.exe
testfile:
	powershell g++ ./test.cpp -o ./bin/test.exe -lcrypto -lssl; ./bin/test.exe
testfile2:
	powershell g++ ./test2.cpp -o ./bin/test2.exe; ./bin/test2.exe
debug:
	powershell g++ bitchcoin.cpp -o ./bin/bitchcoin.exe -lpthread -lcrypto -lssl -g; gdb ./bin/bitchcoin.exe
