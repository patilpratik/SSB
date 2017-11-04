#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "addwindow.h"
#include "file_splitter/ssb_pool.h"
#include <vector>
void
MainWindow::readFileList ()
{

  std::vector<std::string> fileList = SSB::SsbPool::getPool ()->list ();
  for (int i = 0; i < fileList.size (); ++i)
    {
      ui->listWidgetFiles->addItem (QString (fileList[i].c_str ()));
    }
}

MainWindow::MainWindow (QWidget *parent) :
    QMainWindow (parent), aw (NULL), ui (new Ui::MainWindow)
{
  ui->setupUi (this);
  readFileList ();
  ui->pushButtonAdd->setEnabled (false);
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
  aw->show ();

  ui->listWidgetFiles->addItem (inputFile);
  ui->lineEdit_inputFile->setText ("");
  ui->pushButtonAdd->setEnabled (false);
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
    }
}
