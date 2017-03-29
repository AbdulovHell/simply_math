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
    //char *a=(char*)malloc(50);
    //strcpy(a,Project::IO::to_char_string(0.125,Project::IO::var_type::TOWER,0));
    QString str=ui->inputText->toPlainText();
    int size=str.toStdString().size();
    char* input=(char*)malloc(size+1);
    strcpy(input,str.toStdString().c_str());

    char* output=Project::Core::input_to_analize(input);

    QString outstr=QString::fromLocal8Bit(output,strlen(output));
    ui->outputText->setText(outstr);
}
