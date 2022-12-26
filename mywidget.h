#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtWidgets>

struct Task {
    int beginIndex;
    int endIndex;
    QList<qint64>* listA;
    QList<qint64>* listB;
};

class mywidget : public QWidget
{
    Q_OBJECT
public:
    mywidget(QWidget *parent = 0);

signals:

public slots:
    void click();
private slots:
    void progressValueChanged(int v);
    void finished();
    void clickC();
    void clickS();
private:
    QLineEdit *inputN;
    QLabel *inputLabelN;
    QLineEdit *inputThreads;
    QLabel *inputLabelThreads;
    QTextEdit *edit;
    QFutureWatcher<qint64> *watcher;
    QElapsedTimer time;
    QPushButton *startB;
    QPushButton *stopB;
    QList<qint64> arr1;
    QList<qint64> arr2;
    QList<Task> tasks;
    QFuture<qint64> future;
    //QFuture<qreal> future;

};

#endif // MYWIDGET_H
