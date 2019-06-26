#ifndef LINEOVERFLOW_H
#define LINEOVERFLOW_H

#include <QDialog>

namespace Ui {
class LineOverFlow;
}

class LineOverFlow : public QDialog
{
    Q_OBJECT

public:
    explicit LineOverFlow(QWidget *parent = nullptr);
    ~LineOverFlow();

private slots:
    void on_OK_clicked();

private:
    Ui::LineOverFlow *ui;
};

#endif // LINEOVERFLOW_H
