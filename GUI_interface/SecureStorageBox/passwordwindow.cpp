#include "passwordwindow.h"
#include "ui_passwordwindow.h"

PasswordWindow::PasswordWindow (QWidget *parent, const std::string& msgParm) :
    QDialog (parent), ui (new Ui::PasswordWindow), confirm (false)
{
  ui->setupUi (this);
  ui->textEditMsg->setText (QString (msgParm.c_str ()));
}

PasswordWindow::~PasswordWindow ()
{
  delete ui;
}

std::string
PasswordWindow::getPassword ()
{

  return ui->lineEditPassword->text ().toStdString ();
}

void
PasswordWindow::on_pushButtonCancel_3_released ()
{
  confirm = false;
  close ();
}

void
PasswordWindow::on_pushButtonOk_3_released ()
{
  confirm = true;
  close ();
}

bool
PasswordWindow::confirmStatus ()
{
  return confirm;
}
