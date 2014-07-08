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
		emit stepDone(truthTree.toFormattedString());
	}
	truthTree.eliminateNodes();
	stopLock.lock();
	if (stop)
		return;
	stopLock.unlock();
	emit stepDone(truthTree.toFormattedString());

	emit AllDone(truthTree.toFormattedString, truthTree.getResult());

}

WorkerThread::startComputation() {

	this->start();

}

WorkerThread::abortComputation() {

	stopLock.lock();
	stop = true;
	stopLock.unlock();

}
