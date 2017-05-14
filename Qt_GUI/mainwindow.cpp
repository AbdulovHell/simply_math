#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../src/error.h"
#include "../src/IO.h"
#include "../src/filters.h"
#include "../src/core.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Project::Core::Init();

    QImage img;
    img.load(":/images/calc");
    ui->toolBar->addAction(QPixmap::fromImage(img.scaled(32,32 , Qt::KeepAspectRatio)),"Calc", this, SLOT(Calc()));
    img.load(":/images/waste");
    ui->toolBar->addAction(QPixmap::fromImage(img.scaled(32,32 , Qt::KeepAspectRatio)),"Clear", this, SLOT(ClearArea()));
    img.load(":/images/sqrt");
    ui->toolBar->addAction(QPixmap::fromImage(img.scaled(32,32 , Qt::KeepAspectRatio)),"√", this, SLOT(AddSQRT()));
    img.load(":/images/pi");
    ui->toolBar->addAction(QPixmap::fromImage(img.scaled(32,32 , Qt::KeepAspectRatio)),"π", this, SLOT(AddPI()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Calc(){
    //char *a=(char*)malloc(50);
    //strcpy(a,Project::IO::to_char_string(0.125,Project::IO::var_type::TOWER,0));
    QString str=ui->inputText->toPlainText();
    size_t size=str.toStdString().size();
    wchar_t* input=new wchar_t[(size+1)*2];
    wcscpy(input,str.toStdWString().c_str());

    std::wstring output=Project::Core::input_to_analize(input);

    QString outstr=QString::fromStdWString(output);
    ui->inputText->setText(outstr);
    delete [] input;
}

void MainWindow::AddPI(){
    ui->inputText->setText(ui->inputText->toPlainText()+"π");
}

void MainWindow::AddSQRT(){
    QTextCursor a=ui->inputText->textCursor();
    ui->inputText->setText(ui->inputText->toPlainText()+"root(,)");
    a.atEnd();
    a.setPosition(a.position()-2);
    ui->inputText->setTextCursor(a);
}

void MainWindow::ClearArea(){
    ui->inputText->clear();
}

/*
class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget* pwgt = 0) : QMainWindow(pwgt)
    {
        addToolBar(Qt::TopToolBarArea, createToolBar());
        addToolBar(Qt::BottomToolBarArea, createToolBar());
        addToolBar(Qt::LeftToolBarArea, createToolBar());
        addToolBar(Qt::RightToolBarArea, createToolBar());
    }

    QToolBar* createToolBar()
    {
        QToolBar* ptb = new QToolBar("Linker ToolBar");

        ptb->addAction(QPixmap(img1_xpm), "1", this, SLOT(slotNoImpl()));
        ptb->addAction(QPixmap(img2_xpm), "2", this, SLOT(slotNoImpl()));
        ptb->addSeparator();
        ptb->addAction(QPixmap(img3_xpm), "3", this, SLOT(slotNoImpl()));
        ptb->addAction(QPixmap(img4_xpm), "4", this, SLOT(slotNoImpl()));

        return ptb;
    }

public slots:
    void slotNoImpl()
    {
        QMessageBox::information(0, "Message", "Not implemented");
    }
};*/
