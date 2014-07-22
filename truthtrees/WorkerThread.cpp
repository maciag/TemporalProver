#include "WorkerThread.hpp"

WorkerThread::WorkerThread(StringFormula formula) :
		truthTree(formula), stop(false) {

}

WorkerThread::WorkerThread(string formula) :
		truthTree(formula), stop(false) {

}

void WorkerThread::run() {

	while (truthTree.decomposeStep()) {
		stopLock.lock();
		if (stop)
			return;
		stopLock.unlock();
		//emit stepDone(QString::fromStdString(truthTree.toFormattedString()));
	}
	truthTree.eliminateNodes();
	
	stopLock.lock();
	if (stop)
		return;
	stopLock.unlock();
	//emit stepDone(QString::fromStdString(truthTree.toFormattedString()));

	emit allDone(QString::fromStdString(truthTree.toFormattedString()), truthTree.getResult());

}

void WorkerThread::startComputation() {

	this->start();

}

void WorkerThread::abortComputation() {

	stopLock.lock();
	stop = true;
	stopLock.unlock();

}
