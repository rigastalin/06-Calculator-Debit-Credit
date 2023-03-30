#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <QWidget>
#include <QMessageBox>
#include <QtMath>

#ifdef __cplusplus
extern "C" {
#include "calculator.h"
}
#endif

namespace Ui {
class graph;
}

class graph : public QDialog
{
    Q_OBJECT

public:
    graph(QWidget *parent, QString abc);
    ~graph();
    double x_start, x_end, x_step, X, y_start, y_end;
    int N;
    QVector<double> x, y;

private slots:
    void on_GraphBuilt_clicked();

private:
    Ui::graph *ui;
    QString strOut;
};

#endif // GRAPH_H
