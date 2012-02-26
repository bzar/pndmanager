#include "syncworker.h"

SyncWorker::SyncWorker(QPndman::SyncHandle* handle) : handle(handle), timer()
{
  timer.setInterval(100);
  timer.setSingleShot(false);
  connect(&timer, SIGNAL(timeout()), this, SLOT(process()));
}

void SyncWorker::start()
{
  timer.start();
}
  
void SyncWorker::process()
{
  int status = handle->sync();
  handle->update();
  
  if(status == 0 && handle->getDone())
  {
    emit ready(handle);
    deleteLater();
  }
  else if(status <= 0)
  {
    emit error(handle);
    deleteLater();
  }
}
  
