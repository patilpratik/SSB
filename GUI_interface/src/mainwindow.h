#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "addwindow.h"

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit
  MainWindow (QWidget *parent = 0);
  ~MainWindow ();

  void
  readFileList ();

private slots:

  void on_pushButtonExit_released();

  void
  on_pushButtonAdd_released ();

  void
  on_pushButtonBrowse_released ();

  void
  on_lineEdit_inputFile_textChanged (const QString &arg1);

  signals:

private:
  AddWindow *aw;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
