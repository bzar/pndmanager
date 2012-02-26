#include "downloadworker.h"
#include <QDebug>

DownloadWorker::DownloadWorker(QPndman::Handle* handle) : handle(handle), timer()
{
  timer.setInterval(100);
  timer.setSingleShot(false);
  connect(&timer, SIGNAL(timeout()), this, SLOT(process()));
}

void DownloadWorker::start()
{
  qDebug() << "DownloadWorker::start";
  timer.start();
}
  
void DownloadWorker::process()
{
  int status = handle->download();
  handle->update();
  
  if(status == 0 || handle->getDone())
  {
    qDebug() << "DownloadWorker::process -> ready";
    emit ready(handle);
    deleteLater();
  }
  else if(status <= 0)
  {
    qDebug() << "DownloadWorker::process -> error";
    emit error(handle);
    deleteLater();
  }
}
  
