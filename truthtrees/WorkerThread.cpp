#include "WorkerThread.hpp"

WorkerThread::WorkerThread(StringFormula formula) :
		truthTree(formula), stop(false) {

}

WorkerThread::WorkerThread(string formula) :
		truthTree(formula), stop(false) {

}

void WorkerThread::run() {

}

WorkerThread::startComputation() {

	this->start();

}

WorkerThread::abortComputation() {



}
