all: d_test cpp_test

d_test: tester.d functions.a
	dmd tester.d functions.a -ofd_test -Ifunctions
	rm *.o

cpp_test: tester.cpp functions.a
	g++ -c tester.cpp
	dmd functions.a tester.o -ofcpp_test -L-lstdc++
	rm *.o

functions.a: functions/prefix.d functions/lexical.d
	dmd -lib functions/prefix.d functions/lexical.d -offunctions.a