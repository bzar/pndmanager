#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include "qtpndman.h"
#include <QTimer>

class DownloadWorker : public QObject
{
  Q_OBJECT
public:
  DownloadWorker(QPndman::Handle* handle);
  
public slots:
  void start();
  
private slots:
  void process();
  
signals:
  void ready(QPndman::Handle* handle);
  void error(QPndman::Handle* handle);
  
private:
  QPndman::Handle* handle;
  QTimer timer;
};


#endif