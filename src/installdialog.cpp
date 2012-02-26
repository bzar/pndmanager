#include "installdialog.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>

InstallDialog::InstallDialog(QStringList devices, QWidget* parent): QDialog(parent),
  deviceSelection(), desktopLocation("Desktop"), menuLocation("Menu"), appsLocation("Both")
{
  QVBoxLayout* layout = new QVBoxLayout;
  setLayout(layout);
  layout->addWidget(&deviceSelection);
  QHBoxLayout* locationLayout = new QHBoxLayout;
  locationLayout->addWidget(&desktopLocation);
  locationLayout->addWidget(&menuLocation);
  locationLayout->addWidget(&appsLocation);
  desktopLocation.setChecked(true);  
  layout->addLayout(locationLayout);
  
  foreach(QString device, devices)
  {
    deviceSelection.addItem(device);
  }
  
  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(buttonBox);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

InstallDialog::~InstallDialog()
{

}

QString InstallDialog::getDeviceMount()
{
  return deviceSelection.currentText();
}

QPndman::InstallLocation InstallDialog::getInstallLocation()
{
  if(desktopLocation.isChecked())
  {
    return QPndman::Desktop;
  }
  else if(menuLocation.isChecked())
  {
    return QPndman::Menu;
  }
  else
  {
    return QPndman::DesktopAndMenu;
  }
}
