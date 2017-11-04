#include "addwindow.h"
#include "ui_addwindow.h"
#include <QMessageBox>
#include "file_splitter/ssb_file.h"

AddWindow::AddWindow (QWidget *parent, const QString& file) :
    ui (new Ui::AddWindow), QDialog (parent), inputFile (file.toStdString ()), isAddedStatus (
	false)
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
///
/// \todo support dir.
/// 1) check if input file is dir,
/// 2) if yes, then create tar file of that dir in a /tmp/.ssb folder
/// 3) set file_name to the tar file.
///
//    if (is_dir(file_name)) {
//        file_name = createTar(file_name);
//        removeTar = true;
//    }

  int count = 10;
  count = ui->lineEditCount->text ().toInt ();
  std::string pwd = ui->lineEditPassWd->text ().toStdString ();
  if (pwd.empty ())
    {
      QMessageBox messageBox;
      messageBox.critical (0, "Error", "Password cannot be empty!");
      messageBox.setFixedSize (500, 200);
    }
  else
    {
      SSB::SsbFile file1 (inputFile);
      file1.open ();
      if (!file1.good ())
	{
	  QMessageBox messageBox;
	  messageBox.critical (0, "Error", "File not found!");
	  messageBox.setFixedSize (500, 200);
	  //            cerr << "file not ready for use" << endl;
	}
      if (file1.openErr ())
	{
	  //        cerr << "file1 is not opened" << endl;
	}
      else
	{
	  if (file1.split (count, pwd))
	    isAddedStatus = true;
	  else
	    {
	      QMessageBox messageBox;
	      messageBox.critical (0, "Error",
				   "Error while splitting the file!");
	      messageBox.setFixedSize (500, 200);
	      //                cerr << "Error while splitting the file." << endl;

	    }
	  file1.close ();
	}
      //    if (removeTar) {
      //        remove(file_name.c_str());
      //    }

      close ();
    }
}

bool
AddWindow::isAdded ()
{
  return isAddedStatus;
}
