#include <QApplication>
#include "RandomNumberGenerator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RandomNumberGenerator generator;
    generator.show();
    return app.exec();
}