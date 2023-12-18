#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Widget* widget = new Widget();
    widget->show();
    widget->setMinimumSize(1000,700);
    widget->setBaseSize(1000,700);


    return app.exec();
}
