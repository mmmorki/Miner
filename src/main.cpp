#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app{argc, argv };

    auto* window{ new MainWindow{ nullptr } };
    window->show();

    return app.exec();
}