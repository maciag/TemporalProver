all: prover tester

tester: tester.cpp prover
	g++ -std=c++11 -c -o obj/tester.o tester.cpp
	dmd -oftester lib/functions.a obj/tester.o obj/TruthTree.o obj/StringFormula.o obj/LexicalTable.o obj/FormulaNode.o obj/GUI.o obj/moc_GUI.o -L-lstdc++ -L-lQt5Core -L-lQt5Gui -L-lQt5Quick -L-lQt5Qml -L-lQt5Widgets -L-lstdc++
	
prover: main.cpp truthtrees/TruthTree.cpp truthtrees/FormulaNode.cpp truthtrees/StringFormula.cpp truthtrees/LexicalTable.cpp  lib/functions.a obj/GUI.o
	g++ -std=c++11 -c -o obj/LexicalTable.o truthtrees/LexicalTable.cpp 
	g++ -std=c++11 -c -o obj/StringFormula.o truthtrees/StringFormula.cpp 
	g++ -std=c++11 -c -o obj/FormulaNode.o truthtrees/FormulaNode.cpp
	g++ -std=c++11 -c -o obj/TruthTree.o -I ./include/ truthtrees/TruthTree.cpp 
	g++ -std=c++11 -c -o obj/main.o main.cpp
	dmd -ofprover lib/functions.a obj/main.o obj/TruthTree.o obj/FormulaNode.o obj/StringFormula.o obj/LexicalTable.o obj/GUI.o obj/moc_GUI.o -L-lstdc++ -L-lQt5Core -L-lQt5Gui -L-lQt5Quick -L-lQt5Qml -L-lQt5Widgets -L-lstdc++
	
obj/GUI.o : gui/GUI.cpp gui/GUI.hpp
	cd gui; qmake && make; cd ..
	
lib/functions.a: functions/prefix.d functions/lexical.d
	dmd -lib functions/*.d -oflib/functions.a
	
clean:
	rm obj/*
	rm lib/*
