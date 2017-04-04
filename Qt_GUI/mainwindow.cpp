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
    wchar_t* input=(wchar_t*)malloc((size+1)*2);
    wcscpy(input,str.toStdWString().c_str());

    wchar_t* output=Project::Core::input_to_analize(input);

    QString outstr=QString::fromWCharArray(output,wcslen(output));
    ui->outputText->setText(outstr);
    free(input);
}
