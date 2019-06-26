#include "gotowindow.h"
#include "ui_gotowindow.h"
#include <QRegExpValidator>
GotoWindow::GotoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GotoWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setFixedSize(293,130);
    QRegExp regExp("[1-9][0-9]*");
    ui->row_input->setValidator(new QRegExpValidator(regExp, this));
}

GotoWindow::~GotoWindow()
{
    delete ui;
}

void GotoWindow::on_row_input_textEdited(const QString &arg1)
{
    if(arg1=="")ui->go_to->setEnabled(false);
    else ui->go_to->setEnabled(true);
}
void GotoWindow::on_go_to_clicked(){
    emit send_line_num(ui->row_input->text().toInt());
    close();
}
void GotoWindow::on_cancel_clicked(){
    emit send_line_num(0);
    close();
}
