#include "question.h"
#include "ui_question.h"

Question::Question(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Question)
{
    ui->setupUi(this);
}

Question::~Question()
{
    delete ui;
}

void Question::on_pushButton_clicked()
{
    emit signalQuestion(ui->lineEdit->text());
    close();
}

