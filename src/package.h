#ifndef PNDMANAGER_PACKAGE_H
#define PNDMANAGER_PACKAGE_H

#include "qtpndman.h"

class Package : public QPndman::Package
{
  Q_OBJECT
  Q_PROPERTY(bool installed READ getInstalled NOTIFY installedChanged);
  Q_PROPERTY(qint64 bytesDownloaded READ getBytesDownloaded NOTIFY bytesDownloadedChanged);
  Q_PROPERTY(qint64 bytesToDownload READ getBytesToDownload NOTIFY bytesToDownloadChanged);

public:
  Package(QPndman::Package const& p, bool installed, QObject* parent = 0);
  
  bool getInstalled() const;
  qint64 getBytesDownloaded() const;
  qint64 getBytesToDownload() const;
  void updateFrom(QPndman::Package package);
  
public slots:
  void setInstalled();
  void setInstalled(bool);
  void setBytesDownloaded(qint64);
  void setBytesToDownload(qint64);

signals:
  void installedChanged(bool);
  void bytesDownloadedChanged(qint64);
  void bytesToDownloadChanged(qint64);
  
private:
  bool installed;
  qint64 bytesDownloaded;
  qint64 bytesToDownload;
};

#endif