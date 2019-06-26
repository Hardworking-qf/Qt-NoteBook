#ifndef REPLACEWINDOW_H
#define REPLACEWINDOW_H

#include <QDialog>

namespace Ui {
class ReplaceWindow;
}

class ReplaceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceWindow(QWidget *parent = nullptr);
    ~ReplaceWindow();
    void set_search_text(QString);
    void setButtonsEnable(bool);
    void send_signal(int);
signals:
    void send_replace_signal(QString,QString,bool,bool,int);
private slots:
    void on_find_input_textEdited(const QString &arg1);

    void on_Find_clicked();

    void on_Replace_clicked();

    void on_All_Replace_clicked();

    void on_Quit_clicked();

private:
    Ui::ReplaceWindow *ui;
};

#endif // REPLACEWINDOW_H
