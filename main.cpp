#include <QApplication>
#include <QStyleFactory>
#include "mywidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
    mywidget w;
    w.setWindowTitle("Задача №1");
    w.show();
	
    return a.exec();
}
