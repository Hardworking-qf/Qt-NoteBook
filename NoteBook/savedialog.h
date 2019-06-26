#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget *parent = nullptr);
    void change_path(QString);
    ~SaveDialog();

signals:
    void send_status(int);

private slots:
    void on_save_clicked();
    void on_notsave_clicked();
    void on_cancel_clicked();


private:
    Ui::SaveDialog *ui;
};

#endif // SAVEDIALOG_H
