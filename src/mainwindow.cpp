#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph.h"
#include "credit.h"
#include "deposit.h"
#include "question.h"

#include "qcustomplot.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "calculator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    x_var = false;
    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_multi, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_beginBreket, SIGNAL(clicked()), this, SLOT(digitsNumbers()));
    connect(ui->pushButton_endBreket, SIGNAL(clicked()), this, SLOT(digitsNumbers()));

    connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(functionsShow()));
    connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(functionsShow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digitsNumbers() {
    QPushButton *button = (QPushButton *)sender();

    QString new_label;

    if (ui->resultShow->text().contains(".") && button->text() == "0") {
        new_label = ui->resultShow->text() + button->text();
    } else {
        new_label = ui->resultShow->text() + button->text();
    }
    ui->resultShow->setText(new_label);
}

void MainWindow::on_pushButton_dot_clicked()
{
    ui->resultShow->setText(ui->resultShow->text() + ".");
}


void MainWindow::operations() {
    QPushButton *button = (QPushButton *)sender();
    double allNum;
    QString new_label;

    if (button->text() == "+/-") {
        allNum = (ui->resultShow->text()).toDouble();
        allNum *= -1;
        new_label = QString::number(allNum, 'g', 15);

        ui->resultShow->setText(new_label);
    }

}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->resultShow->clear();
}

void MainWindow::on_pushButton_eq_clicked()
{
    QString equal = ui->resultShow->text();
    qarray = equal.toLocal8Bit();
    if (this->x_var == true) {
        question = new Question;
        question->show();
        connect(question, &Question::signalQuestion, this, &MainWindow::slotQuestion);
    } else if (qarray == "") {
        ui->resultShow->clear();
    } else {
        calculation();
    }
    this->x_var = false;
}

void MainWindow::calculation() {
    char *cStr = qarray.data();
    char outstr[1000] = {0};
    int temp = 0;
    double temp_res = 0;
    parser(cStr, &temp, outstr);
    if (temp == 0) {
        temp_res = polishNotation(outstr, &temp);
    }
    char res_char[100] = {0};
    sprintf(res_char, "%g", temp_res);
    if (temp == 0) {
        ui->resultShow->setText(res_char);
    } else {
        QString err;
        if (temp == 1) {
            err = "Too many points....... ";
        } else if (temp == 2) {
            err = "Something wrong with brackets";
        } else if (temp == 3) {
            err = "Brackets are not equal";
        } else if (temp == 4) {
            err = "There are to many operators";
        } else if (temp == 5) {
            err = "There is no such function";
        } else if (temp == 6) {
            err = "Something wrong with the mod";
        } else if (temp == 7) {
            err = "Smt wrong with the polish count";
        } else if (temp == 8) {
            err = "Smg wrong with the polish notation";
        } else if (temp == 9) {
            err = "There is must be some mistake";
        }
        ui->resultShow->setText(err);
    }
}

void MainWindow::on_pushButton_back_clicked()
{
    QString str = ui->resultShow->text();
    str = str.left(str.count() - 1);
    ui->resultShow->setText(str);
}

void MainWindow::functionsShow()
{
    QPushButton *button = (QPushButton *)sender();
    QString new_label;
    new_label = ui->resultShow->text() + button->text() + "(";
    ui->resultShow->setText(new_label);
}

void MainWindow::on_pushButton_x_clicked()
{
    QPushButton *button = (QPushButton *)sender();
    QString new_label;
    new_label = ui->resultShow->text() + button->text();
    ui->resultShow->setText(new_label);
    x_var = true;
}

void MainWindow::slotQuestion(QString str)
{
    QString x_a = str;
    bool ok;
    x_a.toDouble(&ok);
    if (ok == false) {
        ui->resultShow->setText("Something wrong with data for X");
    } else {
        QByteArray byteArray = x_a.toLocal8Bit();
        char* var = byteArray.data();
        char var_2[500] = {0};
        snprintf(var_2, 499, "%s", "(");
        snprintf(var_2 + strlen(var_2), 400, "%s", var);
        snprintf(var_2 + strlen(var_2), 400, "%s", ")");
        qarray.replace("x", var_2);
        calculation();
    }
}

void MainWindow::on_pushButton_gr_clicked()
{
    // this->graph=new Graph(this);
    // this->graph->show();

    graph window(nullptr, ui->resultShow->text());
    window.setModal(true);
    window.exec();
}

void MainWindow::on_pushButton_credit_clicked()
{
    Credit window(nullptr);
    window.setModal(true);
    window.exec();
}

void MainWindow::on_pushButton_deposit_clicked()
{
    deposit window(nullptr);
    window.setModal(true);
    window.exec();
}

#ifdef __cplusplus
}
#endif






