#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDialog>

namespace Ui {
class deposit;
}

class deposit : public QDialog
{
    Q_OBJECT

public:
    explicit deposit(QWidget *parent = nullptr);
    ~deposit();

private slots:
    void on_pushButton_calculate_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_addDeposit_clicked();

private:
    Ui::deposit *ui;

protected:
    double nalogFun(double nalog, double profit);
};

#endif // DEPOSIT_H
