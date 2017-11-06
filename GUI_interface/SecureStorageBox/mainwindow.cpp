#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "passwordwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "addwindow.h"
#include "file_splitter/ssb_pool.h"
#include "file_splitter/ssb_file.h"
#include <vector>
#include <QTimer>

using namespace SSB;

void
MainWindow::readFileList ()
{

  SSB::SsbPool *poolObj = SSB::SsbPool::getPool ();
  std::vector<std::string> fileList = poolObj->list ();
  ui->listWidgetFiles->clear ();
  for (int i = 0; i < fileList.size (); ++i)
    {
      ui->listWidgetFiles->addItem (QString (fileList[i].c_str ()));
    }
  poolObj->freePool ();
  ui->lineEdit_inputFile->setText ("");
  ui->pushButtonAdd->setEnabled (true);
  ui->pushButtonDelete->hide ();
  ui->pushButtonAdd->show ();
}

MainWindow::MainWindow (QWidget *parent) :
    QMainWindow (parent), ui (new Ui::MainWindow), aw (NULL), mDoubleClicked (
	false), mSingleClickedItem (NULL)
{
  ui->setupUi (this);
  readFileList ();
  ui->pushButtonAdd->setEnabled (false);

  QObject::connect(ui->listWidgetFiles, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listItemDoubleClicked(QListWidgetItem*)));
  QObject::connect(ui->listWidgetFiles, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listItemClicked(QListWidgetItem*)));

  ui->pushButtonDelete->hide ();
}

MainWindow::~MainWindow ()
{
  if (NULL != aw)
    {
      delete aw;
      aw = NULL;
    }
  delete ui;
}

void
MainWindow::on_pushButtonExit_released ()
{
  close ();
}

void
MainWindow::on_pushButtonAdd_released ()
{

  QString inputFile = ui->lineEdit_inputFile->text ();
  aw = new AddWindow (this, inputFile);
  aw->exec ();

  if (true == aw->isAdded ())
    {
      ui->listWidgetFiles->addItem (inputFile);
      ui->lineEdit_inputFile->setText ("");
      ui->pushButtonAdd->setEnabled (false);
      readFileList ();
    }
}

void
MainWindow::on_pushButtonBrowse_released ()
{
  QString inputFile = QFileDialog::getOpenFileName (this);
  ui->lineEdit_inputFile->setText (inputFile);
  if (0 < inputFile.size ())
    {
      ui->pushButtonAdd->setEnabled (true);
    }
}

void
MainWindow::on_lineEdit_inputFile_textChanged (const QString &arg1)
{
  if (0 < arg1.size ())
    {
      ui->pushButtonAdd->setEnabled (true);
      ui->pushButtonDelete->hide ();
      ui->pushButtonAdd->show ();
    }
}

void
MainWindow::listItemClicked (QListWidgetItem* listWidgetItem)
{
  if (!mDoubleClicked)
    {
      QTimer::singleShot (100, this, SLOT (listItemClickedTimeout ()));
      mSingleClickedItem = listWidgetItem;
    }
}

void
MainWindow::listItemClickedTimeout ()
{
  if (!mDoubleClicked)
    {
      ui->lineEdit_inputFile->setText (mSingleClickedItem->text ());
      ui->pushButtonAdd->hide ();
      ui->pushButtonDelete->show ();
    }
  else
    mDoubleClicked = false;
}

void
MainWindow::listItemDoubleClicked (QListWidgetItem* listWidgetItem)
{
  mDoubleClicked = true;

  std::string inputFile = listWidgetItem->text ().toStdString ();
  SSB::SsbFile file (inputFile);
  const std::string msg = "Please provide password for file " + inputFile;
  PasswordWindow pwdWind (this, msg);
  pwdWind.exec ();
  std::string pwd = pwdWind.getPassword ();
  if (pwdWind.confirmStatus ())
    {
      if (file.stitch (pwd))
	{
	  QMessageBox messageBox;
	  messageBox.information (this, "Information", "File decrypted!");
	  messageBox.setFixedSize (500, 200);
	  readFileList ();
	}
      else
	{
	  QMessageBox messageBox;
	  messageBox.critical (
	      0, "Error", "Error while decrypting file. Plese check password!");
	  messageBox.setFixedSize (500, 200);
	}
    }
}

void
MainWindow::on_pushButtonDelete_released ()
{
  std::string inputFile = ui->lineEdit_inputFile->text ().toStdString ();
  SSB::SsbFile file (inputFile);
  QMessageBox::StandardButton reply (QMessageBox::Yes);
  file.open ();
  if (!file.good ())
    {
      // file is not present in the file system. Indicates it is not restored from SSB.
      // gets confirmation from user about deleting the file from ssb
      QString msg ("The file is not found in your filesystem.");
      msg.append ("Continue only if you have restored the file from SSB");
      msg.append ("To continue, press \"y\", else press \"n\"");
      reply = QMessageBox::question (this, "Remove", msg,
				     QMessageBox::Yes | QMessageBox::No);
    }

  if (reply == QMessageBox::Yes)
    {
      const std::string msg = inputFile + " will be deleted!";
      PasswordWindow pwdWind (this, msg);
      pwdWind.exec ();
      std::string pwd = pwdWind.getPassword ();
      if (pwdWind.confirmStatus ())
	{
	  if (file.remove (pwd))
	    {
	      QMessageBox messageBox;
	      messageBox.information (this, "Information", "File removed!");
	      messageBox.setFixedSize (500, 200);
	      readFileList ();
	    }
	  else
	    {
	      QMessageBox messageBox;
	      messageBox.critical (0, "Error", "Error while removing file!");
	      messageBox.setFixedSize (500, 200);
	    }
	}
    }

}
