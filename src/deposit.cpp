#include "deposit.h"
#include "ui_deposit.h"

deposit::deposit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deposit)
{
    ui->setupUi(this);
}

deposit::~deposit()
{
    delete ui;
}

void deposit::on_pushButton_calculate_clicked()
{
    int days = 30;
    int year = 365;
    double finalProfit = 0.0;
    double totalSum = 0.0;
    double yearProfit_i = 0.0;
    double taxesPay_i = 0.0;
    double taxesPayFull = 0.0;

    double sum = ui->doubleSpinBox_sum->value();
    int period = ui->spinBox_period->value();
    double procent = ui->doubleSpinBox_procent->value();
    double nalog = ui->doubleSpinBox_taxProcent->value();

    double arrPush[period];
    double arrPop[period];
    memset(arrPush, 0, sizeof(arrPush));
    memset(arrPop, 0, sizeof(arrPop));

    double yearProfit = 0.0;
    double dayProfit = 0.0;
    double monthProfit_i = 0.0;
    double monthSum_i = sum;

    int type = 0;
    if (ui->radioButton_once->isChecked()) {
        type = 1;
    }
    if (ui->radioButton_eachMonth->isChecked()) {
        type = 2;
    }

    int capital = 0;
    if (ui->checkBox_capitalization->isChecked()) {
        capital = 1;
    }

    if (sum != 0.0 && period != 0 && procent != 0.0 && nalog != 0.0) {
        if (ui->checkBox_addition->isChecked()) {
            if (period > 2) {
                for (int i = 0; i < period - 2; i++) {
                    QTableWidgetItem *item_p = ui->tableWidget_add->item(i, 0);
                    QTableWidgetItem *item_s = ui->tableWidget_min->item(i, 0);
                    if (item_p != 0) {
                        arrPush[i] = item_p->text().toDouble();
                    }
                    if (item_s != 0) {
                        arrPop[i] = item_s->text().toDouble();
                    }
                }
            }
        }

        if ((type == 1) || (type == 2 && capital == 0)) {
            for (int i = 0, j = 0; i < period; i++) {
                if (i != 0 && i % 12 == 0 && type == 2) {
                    taxesPay_i = nalogFun(nalog, yearProfit_i);
                    taxesPayFull = taxesPayFull + taxesPay_i;
                    yearProfit_i = 0;
                }

                if (i != 0 && i != period - 1 && period > 2) {
                    monthSum_i = monthSum_i + arrPush[j];
                    monthSum_i = monthSum_i - arrPop[j];
                    sum = sum + arrPush[j];
                    sum = sum - arrPop[j];
                    j++;
                }

                yearProfit = monthSum_i * (procent / 100);
                dayProfit = yearProfit / year;
                monthProfit_i = dayProfit * days;

                yearProfit_i = yearProfit_i + monthProfit_i;
                finalProfit = finalProfit + monthProfit_i;
            }

            if (type == 2) {
                taxesPay_i = nalogFun(nalog, yearProfit_i);
                taxesPayFull = taxesPayFull + taxesPay_i;
            } else {
                taxesPayFull = nalogFun(nalog, yearProfit_i);
            }

            if (capital == 0) {
                totalSum = totalSum + sum;
            }
            if (capital == 1) {
                totalSum = totalSum + sum + finalProfit;
            }

            ui->label_sumOfProcentEnd->setText(QString::number(finalProfit, 'f', 2));
            ui->label_sumRemain->setText(QString::number(totalSum, 'f', 2));

        } else if (type == 2 && capital == 1) {
            for (int i = 1, j = 0; i <= period; i++) {
                if (i != 1 && i != period && period) {
                    monthSum_i = monthSum_i + arrPush[j];
                    monthSum_i = monthSum_i - arrPop[j];
                    sum = sum + arrPush[j];
                    sum = sum - arrPop[j];
                    j++; 
                }

                yearProfit = (monthSum_i * procent) / 100;
                dayProfit = yearProfit / year;
                monthProfit_i = dayProfit * days;

                yearProfit_i = yearProfit_i + monthProfit_i;
                monthSum_i = monthSum_i + monthProfit_i;
                finalProfit = finalProfit + monthProfit_i;

                if (i % 12 == 0) {
                    taxesPay_i = nalogFun(nalog, yearProfit_i);
                    taxesPayFull = taxesPayFull + taxesPay_i;
                    yearProfit_i = 0;
                }
            }

            taxesPay_i = nalogFun(nalog, yearProfit_i);
            taxesPayFull = taxesPayFull + taxesPay_i;

            totalSum = sum + finalProfit;
            ui->label_taxesToPay->setText(QString::number(taxesPayFull, 'f', 2));
            ui->label_sumRemain->setText(QString::number(totalSum, 'f', 2));
        }
        ui->label_taxesToPay->setText(QString::number(taxesPayFull, 'f', 2));
    } else {
        ui->doubleSpinBox_sum->setValue(sum);
        ui->spinBox_period->setValue(period);
        ui->doubleSpinBox_procent->setValue(procent);
        ui->doubleSpinBox_taxProcent->setValue(taxesPayFull);
    }
}

double deposit::nalogFun(double procent, double yearProfit_i) 
{
    double mustPay = 0.0;
    double free = (1000000 * 11) / 100 ; // rate CB - 11% in 26.05.2022
    double payment = yearProfit_i - free;

    if (payment > 0) {
        mustPay = (payment * procent) / 100;
    } else {
        mustPay = 0.0;
    }

    return mustPay;
}



void deposit::on_pushButton_clear_clicked()
{
    ui->doubleSpinBox_sum->setValue(0);
    ui->spinBox_period->setValue(0);
    ui->doubleSpinBox_procent->setValue(0);
    ui->doubleSpinBox_taxProcent->setValue(0);

    ui->checkBox_capitalization->setChecked(0);
    ui->checkBox_addition->setChecked(0);

    ui->label_taxesToPay->setText("0");
    ui->label_sumOfProcentEnd->setText("0");
    ui->label_sumRemain->setText("0");

    ui->tableWidget_add->setColumnCount(0);
    ui->tableWidget_add->setRowCount(0);
    ui->tableWidget_min->setColumnCount(0);
    ui->tableWidget_min->setRowCount(0);
}


void deposit::on_pushButton_addDeposit_clicked()
{
    int period = ui->spinBox_period->value();
    if (period > 2) {
        ui->tableWidget_add->setColumnCount(1);
        ui->tableWidget_add->setRowCount(period - 2);
        ui->tableWidget_add->setHorizontalHeaderLabels(QStringList() << "Пополнить, руб");

        ui->tableWidget_min->setColumnCount(1);
        ui->tableWidget_min->setRowCount(period - 2);
        ui->tableWidget_min->setHorizontalHeaderLabels(QStringList() << "Снять, руб");

        ui->checkBox_addition->setChecked(1);
    }
}

