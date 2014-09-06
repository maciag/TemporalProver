HEADERS += \
    functions/prefix.h \
    gui/GUI.hpp \
    include/tree.hh \
    truthtrees/FormulaNode.hpp \
    truthtrees/LexicalTable.hpp \
    truthtrees/StringFormula.hpp \
    truthtrees/TruthTree.hpp \
    truthtrees/WorkerThread.hpp

SOURCES += \
    gui/GUI.cpp \
    truthtrees/FormulaNode.cpp \
    truthtrees/LexicalTable.cpp \
    truthtrees/StringFormula.cpp \
    truthtrees/TruthTree.cpp \
    truthtrees/WorkerThread.cpp \
    main.cpp \
    tester.cpp \
    functions/prefix.cpp


CONFIG += c++11
QT += core gui quick widgets
