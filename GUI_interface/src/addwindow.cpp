#include "addwindow.h"
#include "ui_addwindow.h"

AddWindow::AddWindow (QWidget *parent, const QString& file) :
    QDialog (parent), ui (new Ui::AddWindow), inputFile (file)
{
  ui->setupUi (this);
  if (0 < file.size ())
    {
      ui->textEditConfirmation->setText (
	  file
	      + " will be encrypted and added to SSB.\n\nPlease try retriving the file before removing original file.");
    }
  else
    {
      ui->textEditConfirmation->setText ("No file provided.");
      ui->pushButtonOk->setEnabled (false);
    }
}

AddWindow::~AddWindow ()
{
  delete ui;
}

void
AddWindow::on_pushButtonCancel_released ()
{
  close ();
}

void
AddWindow::on_pushButtonOk_released ()
{
  close ();
}
