#ifndef GOTOWINDOW_H
#define GOTOWINDOW_H

#include <QDialog>

namespace Ui {
class GotoWindow;
}

class GotoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GotoWindow(QWidget *parent = nullptr);
    ~GotoWindow();

signals:
    void send_line_num(int);

private slots:
    void on_row_input_textEdited(const QString &arg1);

    void on_go_to_clicked();

    void on_cancel_clicked();

private:
    Ui::GotoWindow *ui;
};

#endif // GOTOWINDOW_H
