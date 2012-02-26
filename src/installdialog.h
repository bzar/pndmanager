#ifndef PNDMANAGER_INSTALLDIALOG_H
#define PNDMANAGER_INSTALLDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QRadioButton>

#include "packagemodel.h"

class InstallDialog : public QDialog
{
  Q_OBJECT
public:
  explicit InstallDialog(QStringList devices, QWidget* parent = 0);
  virtual ~InstallDialog();

  QString getDeviceMount();
  QPndman::InstallLocation getInstallLocation();
  
private:
  QComboBox deviceSelection;
  QRadioButton desktopLocation;
  QRadioButton menuLocation;
  QRadioButton appsLocation;
};
#endif