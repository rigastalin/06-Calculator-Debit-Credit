#include "credit.h"
#include "ui_credit.h"

Credit::Credit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Credit)
{
    ui->setupUi(this);
}

Credit::~Credit()
{
    delete ui;
}

void Credit::on_pushButton_calculate_clicked()
{
    double sum = ui->doubleSpinBo_sumCredit->value();
    double procent = ui->doubleSpinBox_procent->value();
    int period = ui->spinBox_period->value();

    double monthProc = (procent/12)/100;

    if (sum != 0.0 && procent != 0.0 && period != 0.0) {
        ui->tableWidget_periods->setColumnCount(4);
        ui->tableWidget_periods->setRowCount(period);
        ui->tableWidget_periods->setHorizontalHeaderLabels(QStringList() << "Платеж, руб" << "Тело кредита, руб" << "Проценты, руб" << "Остаток, руб");

        int method = 0;
        if (ui->radioButton_type1->isChecked()) {
            method = 1;
        }
        if (ui->radioButton_type2->isChecked()) {
            method = 2;
        }

        QTableWidgetItem *item;
        double monthPay = 0.0;
        double currentLoan = sum; 

        double sumPaid = 0.0;
        double monthProcPay = 0.0;

        double bodyCredit = 0.0;

        double fullLoan = 0.0;
        double koefAnn = 0.0;

        double procMonthLoan = 0.0;

        if (method == 1) {
            koefAnn = (monthProc * (pow(1 + monthProc, period))) / ((pow(1 + monthProc, period)) - 1);
            monthPay = koefAnn * sum;
            // monthPay = sum * (monthProc + (monthProc / (pow(1 + monthProc, period) - 1)));

            for (int i = 0; i < period; i++) {
                monthProcPay = currentLoan * monthProc;
                bodyCredit = monthPay - monthProcPay;
                currentLoan = currentLoan - bodyCredit;

                item = new QTableWidgetItem(QString::number(monthPay, 'f', 2));
                ui->tableWidget_periods->setItem(i, 0, item);
                item = new QTableWidgetItem(QString::number(bodyCredit, 'f', 2));
                ui->tableWidget_periods->setItem(i, 1, item);
                item = new QTableWidgetItem(QString::number(monthProcPay, 'f', 2));
                ui->tableWidget_periods->setItem(i, 2, item);
                item = new QTableWidgetItem(QString::number(currentLoan, 'f', 2));
                ui->tableWidget_periods->setItem(i, 3, item);

                sumPaid = sumPaid + monthProcPay;
            }
            fullLoan = sum + sumPaid;

            ui->label_sumToPay_B->setText(QString::number(fullLoan, 'f', 2));
            ui->label_procToPay_B->setText(QString::number(sumPaid, 'f', 2));
            ui->label_monthToPay_B->setText(QString::number(monthPay, 'f', 2));
        }
        if (method == 2) {
            bodyCredit = currentLoan / period;
            procMonthLoan = currentLoan * monthProc;

            for (int i = 0; i < period; i++) {
                monthProcPay = currentLoan * monthProc;
                monthPay = bodyCredit + monthProcPay;

                item = new QTableWidgetItem(QString::number(monthPay, 'f', 2));
                ui->tableWidget_periods->setItem(i, 0, item);
                item = new QTableWidgetItem(QString::number(bodyCredit, 'f', 2));
                ui->tableWidget_periods->setItem(i, 1, item);
                item = new QTableWidgetItem(QString::number(monthProcPay, 'f', 2));
                ui->tableWidget_periods->setItem(i, 2, item);
                item = new QTableWidgetItem(QString::number(currentLoan, 'f', 2));
                ui->tableWidget_periods->setItem(i, 3, item);

                currentLoan = currentLoan - bodyCredit;
                sumPaid = sumPaid + monthProcPay;
            }
            fullLoan = sum + sumPaid;

            ui->label_sumToPay_B->setText(QString::number(fullLoan, 'f', 2));
            ui->label_procToPay_B->setText(QString::number(sumPaid, 'f', 2));
            ui->label_monthToPay_B->setText("~~~~~~");
        }
    } else {
        ui->doubleSpinBo_sumCredit->setValue(sum);
        ui->spinBox_period->setValue(period);
        ui->doubleSpinBox_procent->setValue(procent);
    }
}


void Credit::on_pushButton_clear_clicked()
{
    ui->doubleSpinBo_sumCredit->setValue(0);
    ui->doubleSpinBox_procent->setValue(0);
    ui->spinBox_period->setValue(0);

    ui->label_sumToPay_B->setText("0");
    ui->label_procToPay_B->setText("0");
    ui->label_monthToPay_B->setText("0");

    ui->tableWidget_periods->setColumnCount(0);
    ui->tableWidget_periods->setRowCount(0);
}

