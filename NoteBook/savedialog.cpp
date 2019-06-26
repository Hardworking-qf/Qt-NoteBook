#include "savedialog.h"
#include "ui_savedialog.h"

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setFixedSize(352,127);
}
void SaveDialog::change_path(QString path){
    ui->text->setText("<font color=#2B33BB>是否将更改保存到 "+(path.isEmpty()?"新建文本文档.txt":path)+"?</font>");
}

SaveDialog::~SaveDialog()
{
    delete ui;
}
void SaveDialog::on_save_clicked(){
    emit send_status(1);
    close();
}
void SaveDialog::on_notsave_clicked(){
    emit send_status(2);
    close();
}
void SaveDialog::on_cancel_clicked(){
    emit send_status(3);
    close();
}
