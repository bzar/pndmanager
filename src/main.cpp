#include <QApplication>
#include <QLabel>

#include "mainwindow.h"

int main(int argc, char** argv)
{
  QApplication application(argc, argv);
  MainWindow mainwindow;
  mainwindow.show();
  application.exec();
}

