#ifndef PASSWORDWINDOW_H
#define PASSWORDWINDOW_H

#include <QDialog>

namespace Ui
{
  class PasswordWindow;
}

class PasswordWindow : public QDialog
{
  Q_OBJECT

public:
  explicit
  PasswordWindow (QWidget *parent = 0,
		  const std::string& msg = std::string (""));
  ~PasswordWindow ();

  std::string
  getPassword ();

  bool
  confirmStatus ();private slots:
  void on_pushButtonCancel_3_released();

  void
  on_pushButtonOk_3_released ();

private:
  Ui::PasswordWindow *ui;
  bool confirm;
};

#endif // PASSWORDWINDOW_H
