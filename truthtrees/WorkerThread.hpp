#ifndef WORKERTHREAD_HPP_
#define WORKERTHREAD_HPP_

#include <QtCore>
#include <QMutex>
#include "StringFormula.hpp"
#include "TruthTree.hpp"

class WorkerThread : public QThread{
	Q_OBJECT

	TruthTree truthTree;
	bool stop;
	QMutex stopLock;
	void run();

public:
	WorkerThread(StringFormula formula);
	WorkerThread(string formula);

public slots:
	void startComputation();
	void abortComputation();

signals:
	void stepDone(QString forrmatedTree);
	void allDone(QString forrmatedTree, bool result);

};



#endif /* WORKERTHREAD_HPP_ */
