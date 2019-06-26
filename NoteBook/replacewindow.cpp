#include "replacewindow.h"
#include "ui_replacewindow.h"

ReplaceWindow::ReplaceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setFixedSize(405,202);
}

ReplaceWindow::~ReplaceWindow()
{
    delete ui;
}
void ReplaceWindow::set_search_text(QString text){
    ui->find_input->setText(text);
    setButtonsEnable(!text.isEmpty());
}
void ReplaceWindow::setButtonsEnable(bool enable){
    ui->Find->setEnabled(enable);
    ui->Replace->setEnabled(enable);
    ui->All_Replace->setEnabled(enable);
}
void ReplaceWindow::on_find_input_textEdited(const QString &arg1){
    setButtonsEnable(!arg1.isEmpty());
}
void ReplaceWindow::send_signal(int mode){
    emit send_replace_signal(ui->find_input->text(),ui->replace_input->text(),ui->CaseSensitivity->isChecked(),ui->Repeat->isChecked(),mode);
}
void ReplaceWindow::on_Find_clicked(){//0
    send_signal(0);
}
void ReplaceWindow::on_Replace_clicked(){//1
    send_signal(1);
}
void ReplaceWindow::on_All_Replace_clicked(){//2
    send_signal(2);
}
void ReplaceWindow::on_Quit_clicked(){
    close();
}
