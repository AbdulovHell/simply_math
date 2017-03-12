#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../src/core.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Project::Core::Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked(){
    //var_const pi = var_const("pi", 3.1415926535897932384626433832);
    //var_const e = var_const("e", 2.71828182846);

    //vector<var_const> general_var_const = { pi,e };

    QString str=ui->inputText->toPlainText();
    int size=str.toStdString().size();
    char* input=(char*)malloc(size+1);
    strcpy(input,str.toStdString().c_str());

    char* output=Project::Core::input_to_analize(input);

    QString outstr=QString::fromLocal8Bit(output,strlen(output));
    ui->outputText->setText(outstr);
    //int check = input_to_analize(&general_var_const);
}
