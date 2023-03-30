#include "graph.h"
#include "ui_graph.h"


graph::graph(QWidget *parent, QString str_out) :
    QDialog(parent),
    ui(new Ui::graph)
{
    ui->setupUi(this);
    strOut = str_out;
}

graph::~graph()
{
    delete ui;
}


void graph::on_GraphBuilt_clicked() {
    x_step = 0.0002;
    x_start = ui->x_start->value();
    x_end = ui->x_end->value();
    y_start = ui->y_start->value();
    y_end = ui->y_end->value();

    int err = 0;
    if (x_start >= x_end || y_start >= y_end) {
        err = 1;
    }

    ui->widget->xAxis->setRange(x_start, x_end);
    ui->widget->yAxis->setRange(y_start, y_end);
    for (X = x_start; X <= x_end && err == 0; X += x_step) {
        x.push_back(X);
        char output[1000] = {0};
        double temp_result = 0;
        char var_2[500] = {0};
        // snprintf(var_2, 499, "%s", "(");
        snprintf(var_2, 400, "%lf", X);
        // snprintf(var_2 + strlen(var_2), 399, "%s", ")");
        QByteArray count = strOut.toLocal8Bit();
        count.replace("x", var_2);
        char *cstr = count.data();
        parser(cstr, &err, output);
        if (err == 0) {
            temp_result = polishNotation(output, &err);
            y.push_back(temp_result);
        }
    }
    if (err == 0) {
        ui->widget->addGraph();
        ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
        ui->widget->graph(0)->addData(x,y);
        ui->widget->replot();
        ui->widget->setInteraction(QCP::iRangeZoom, true);
        ui->widget->setInteraction(QCP::iRangeDrag, true);
    }
}
