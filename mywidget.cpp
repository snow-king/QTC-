#include <QtConcurrent>
#include <QtWidgets>
#include "mywidget.h"


qint64 singleThreadExecute(QList<qint64> *arr1, QList<qint64> *arr2, qint64 count) {
     qint64 res = 0;
     for (int i=0; i < count; i++) {
         if (arr1->at(i) == arr2->at(i)) {
             res++;
            }
     }
     return res;
}

qint64 perElementFunc(const Task tsk) {
     qint64 res = 0;
     for (int i=tsk.beginIndex; i<=tsk.endIndex; i++) {
         if (tsk.listA->at(i) == tsk.listB->at(i)) {
             res++;
            }
     }
     return res;
}

void reduce(qint64 & sum, const qint64 semiSum) {
     sum += semiSum;
}

mywidget::mywidget(QWidget *parent) : QWidget(parent)
{
    inputN = new QLineEdit();
    inputLabelN = new QLabel("Введите количество элементов:");

    inputThreads = new QLineEdit();
    inputLabelThreads = new QLabel("Введите количество потоков:");

    edit = new QTextEdit();
    startB = new QPushButton(tr("Старт"));
    stopB = new QPushButton(tr("Пауза"));
    QPushButton *closeB = new QPushButton(tr("Закрыть"));

    QVBoxLayout *lay = new QVBoxLayout();
    stopB->setEnabled(false);
    lay->addWidget(inputLabelN);
    lay->addWidget(inputN);
    lay->addWidget(inputLabelThreads);
    lay->addWidget(inputThreads);
    lay->addWidget(edit);
    lay->addWidget(startB);
    lay->addWidget(stopB);
    lay->addWidget(closeB);

    setLayout(lay);

    connect(startB,SIGNAL(clicked()),this,SLOT(click()));
    connect(closeB,SIGNAL(clicked()),this,SLOT(clickC()));
    connect(stopB,SIGNAL(clicked()),this,SLOT(clickS()));
}

void mywidget::click() {

    startB->setEnabled(false);
    stopB->setEnabled(true);

    int nElem = inputN->text().toInt();
    int nThreads = inputThreads->text().toInt();

    int perThread = nElem / nThreads;

    edit->append("Hello!");

    QRandomGenerator generator;

    for (int i=0; i<=nElem-1; i++) {
        arr1.append(generator.bounded(100, 100000));
        arr2.append(generator.bounded(100, 100000));
        }

    time.start();

    edit->append("Совпадений в однопотоке найдено:" + QString::number(singleThreadExecute(&arr1, &arr2, nElem)));
    edit->append("Время выполнения в однопотоке: " + QString::number(time.restart()));

    time.start();
    for (int i=0; i<=nThreads-1; i++) {
        Task tsk;
        tsk.beginIndex = i*perThread;
        tsk.endIndex = (i+1)*perThread-1;
        tsk.listA = &arr1;
        tsk.listB = &arr2;
        tasks.append(tsk);
        }

    watcher = new QFutureWatcher<qint64>();
    connect(watcher,SIGNAL(progressValueChanged(int)),this,SLOT(progressValueChanged(int)));
    connect(watcher,SIGNAL(finished()),this,SLOT(finished()));

    future = QtConcurrent::mappedReduced(tasks, perElementFunc, reduce);
    watcher->setFuture(future);
    edit->append("Совпадений в многопотоке найдено:" + QString::number(watcher->result()));
}

void mywidget::progressValueChanged(int v) {
    edit->append(tr("Progress: ")+QString::number(v));
}
void mywidget::finished() {
    edit->append(tr("Время выполнения в многопотоке")+" "+QString::number(time.elapsed()));
    startB->setEnabled(true);
    stopB->setEnabled(false);
}

void mywidget::clickC() {
     watcher->cancel();
     watcher->waitForFinished();
     close();
}
void mywidget::clickS() {
     if (watcher->isFinished()) { return; }
     if (watcher->isPaused()) {
         watcher->resume();
         edit->append(tr("Выполняется"));
         stopB->setText(tr("Остановить"));
     } else {
        watcher->pause();
        edit->append(tr("Остановлен"));
        stopB->setText(tr("Продолжить"));
     }

}
