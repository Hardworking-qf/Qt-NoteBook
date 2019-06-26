#include "searchwindow.h"
#include "ui_searchwindow.h"
#include <QCloseEvent>
#include <QShowEvent>
SearchWindow::SearchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setFixedSize(429,159);
}
SearchWindow::~SearchWindow()
{
    delete ui;
}

void SearchWindow::set_search_text(QString text){
    ui->input->setText(text);
    if(text.isEmpty())ui->Find->setEnabled(false);
    else ui->Find->setEnabled(true);
}
void SearchWindow::on_Find_clicked(){
    emit send_search_signal(ui->input->text(),ui->CaseSensitivity->isChecked(),ui->up->isChecked(),ui->Repeat->isChecked());
}

void SearchWindow::on_Quit_clicked()
{
    close();
}

void SearchWindow::on_input_textEdited(const QString &arg1)
{
    if(arg1.isEmpty())ui->Find->setEnabled(false);
    else ui->Find->setEnabled(true);
}
