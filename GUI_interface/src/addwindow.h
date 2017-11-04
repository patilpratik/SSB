#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QDialog>

namespace Ui
{
  class AddWindow;
}

class AddWindow : public QDialog
{
  Q_OBJECT

public:
  explicit
  AddWindow (QWidget *parent = 0, const QString& inputFile = QString (""));
  ~AddWindow ();

private slots:
  void on_pushButtonCancel_released();

  void
  on_pushButtonOk_released ();

private:
  QString inputFile;
  Ui::AddWindow *ui;
};

#endif // ADDWINDOW_H
