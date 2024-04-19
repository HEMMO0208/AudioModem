#include "main_window.h"
#include <QtWidgets/QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    main_window w;
    w.show();

    return a.exec();
}
