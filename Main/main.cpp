#include <QApplication>

#include "mainwindow.h"
#include "NeuralNetwork.h"
//#include "Temp.h"

void RunAllDefaults()
{
    std::string in;

    //RunTemp();

    std::cin >> in;

    RunNeuralNetwork();

    std::cin >> in;
}

int main(int argc, char *argv[])
{
    RunAllDefaults();


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}














