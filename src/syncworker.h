#ifndef SYNCWORKER_H
#define SYNCWORKER_H

#include "qtpndman.h"
#include <QTimer>

class SyncWorker : public QObject
{
  Q_OBJECT
public:
  SyncWorker(QPndman::SyncHandle* handle);
  
public slots:
  void start();
  
private slots:
  void process();
  
signals:
  void ready(QPndman::SyncHandle* handle);
  void error(QPndman::SyncHandle* handle);
  
private:
  QPndman::SyncHandle* handle;
  QTimer timer;
};


#endif