#include "lineoverflow.h"
#include "ui_lineoverflow.h"

LineOverFlow::LineOverFlow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineOverFlow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setFixedSize(140,126);
}

LineOverFlow::~LineOverFlow()
{
    delete ui;
}

void LineOverFlow::on_OK_clicked()
{
    close();
}
