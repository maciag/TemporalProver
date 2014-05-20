all: d_test cpp_test prover

d_test: tester.d functions.a
	dmd tester.d functions.a -ofd_test -Ifunctions
	rm *.o

cpp_test: tester.cpp functions.a
	g++ -std=c++11 -c tester.cpp
	dmd functions.a tester.o -ofcpp_test -L-lstdc++ -ofcpp_test
	rm *.o

prover: main.cpp TruthTree.cpp StringFormula.cpp functions.a
	g++ -std=c++11 -c -o StringFormula.o StringFormula.cpp 
	g++ -std=c++11 -c -o TruthTree.o -I ./include/ TruthTree.cpp 
	g++ -std=c++11 -c -o main.o main.cpp
	dmd functions.a main.o TruthTree.o StringFormula.o -ofcpp_test -L-lstdc++ -ofprover
	rm *.o
	
functions.a: functions/prefix.d functions/lexical.d
	dmd -lib functions/prefix.d functions/lexical.d -offunctions.a
	
clean:
	rm d_test
	rm prover
	rm cpp_test