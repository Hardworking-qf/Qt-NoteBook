#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QDialog>

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();
    void set_search_text(QString);

signals:
    void send_search_signal(QString,bool,bool,bool);
    void search_signal();
private slots:
    void on_Find_clicked();
    void on_Quit_clicked();
    void on_input_textEdited(const QString &arg1);

private:
    Ui::SearchWindow *ui;
};

#endif // SEARCHWINDOW_H
