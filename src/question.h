#ifndef QUESTION_H
#define QUESTION_H

#include <QWidget>

namespace Ui {
class Question;
}

class Question : public QWidget
{
    Q_OBJECT

public:
    explicit Question(QWidget *parent = nullptr);
    ~Question();

private:
    Ui::Question *ui;

signals:
    void signalQuestion(QString);
    
private slots:
    void on_pushButton_clicked();
};

#endif // QUESTION_H
