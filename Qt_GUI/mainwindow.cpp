#include <string>
#include <wchar.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../src/error.h"
#include "../src/IO.h"
#include "../src/filters.h"
#include "../src/core.h"
#include <QFile>
#include <QLoggingCategory>
QLoggingCategory category("default");
bool MainWindow::isOutOrEmpty(wchar_t* str,size_t len) {
                 if (str == L"") return true;

                 if (!(len > 3)) return false;
                 if ((str[0] == '>'&&str[1] == '>'&&str[2] == '>'))
                     return true;
                 else return false;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Project::Core::Init();

    //! [enable gestures]
    ui->centralWidget->grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::SwipeGesture);
    ui->inputText->grabGesture(Qt::SwipeGesture);
    //! [enable gestures]

    QImage imgCalc,imgClear,imgSqrt,imgPI;
    imgCalc.load(":/images/calc");
    imgClear.load(":/images/waste");
    imgSqrt.load(":/images/sqrt");
    imgPI.load(":/images/pi");

#ifdef Q_OS_ANDROID
#define ICON_SIZE 64
    QSize iSize=ui->toolBar->iconSize();
    iSize.setHeight(ICON_SIZE);
    iSize.setWidth(ICON_SIZE);
    ui->toolBar->setIconSize(iSize);
    ui->toolBar->addAction(QPixmap::fromImage(imgCalc.scaled(ICON_SIZE,ICON_SIZE , Qt::KeepAspectRatio)),"Calc", this, SLOT(Calc()));
    ui->toolBar->addAction(QPixmap::fromImage(imgClear.scaled(ICON_SIZE,ICON_SIZE , Qt::KeepAspectRatio)),"Clear", this, SLOT(ClearArea()));
    ui->toolBar->addAction(QPixmap::fromImage(imgSqrt.scaled(ICON_SIZE,ICON_SIZE , Qt::KeepAspectRatio)),"√", this, SLOT(AddSQRT()));
    ui->toolBar->addAction(QPixmap::fromImage(imgPI.scaled(ICON_SIZE,ICON_SIZE , Qt::KeepAspectRatio)),"π", this, SLOT(AddPI()));
#else
    ui->toolBar->addAction(QPixmap::fromImage(imgCalc.scaled(32,32 , Qt::KeepAspectRatio)),"Calc", this, SLOT(Calc()));
    ui->toolBar->addAction(QPixmap::fromImage(imgClear.scaled(32,32 , Qt::KeepAspectRatio)),"Clear", this, SLOT(ClearArea()));
    ui->toolBar->addAction(QPixmap::fromImage(imgSqrt.scaled(32,32 , Qt::KeepAspectRatio)),"√", this, SLOT(AddSQRT()));
    ui->toolBar->addAction(QPixmap::fromImage(imgPI.scaled(32,32 , Qt::KeepAspectRatio)),"π", this, SLOT(AddPI()));
#endif

    /*
    QFile file("C:\\Users\\Admin\\Desktop\\new 3.html");
    if (file.open(QIODevice::ReadOnly)){
        QString html(QString::fromLocal8Bit(file.readAll()));
        ui->inputText->setHtml(html);
    }*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Calc(){
    //поле ввода в строку
    QString inputTxt=ui->inputText->toPlainText();
    int StrCount=0;
    int inputSize=inputTxt.size();
    //массив под строку, преобразование из QString в wchar_t*
    wchar_t* input=new wchar_t[inputSize+1];
    inputTxt.toWCharArray(input);
    input[inputSize]=0;
    inputSize++;
    //есть строка input, длинны inputSize, считаем количество подстрок
    for(int i=0;i<inputSize;i++){
        if((input[i]=='\n')||(input[i]==0))
            StrCount++;
    }
    if(StrCount<1) return;  //если пусто
    //массив под отдельные строки
    wchar_t** inputStrs=new wchar_t*[StrCount];
    for(int i=0;i<StrCount;i++) inputStrs[i]=NULL;  //указатели в никуда, пусть уж лучше на NULL

    int index=0;
    for(int i=0,k=0;i<inputSize;i++){   //по всей длинне общей строки
        if((input[i]=='\n')||(input[i]==0)){    //если встречаем разделитель
            if(!isOutOrEmpty(input+index,wcslen(input+index))){ //и строка не является выводом
                input[i]='\0';  //отделяем для копирки
                inputStrs[k]=new wchar_t[i-index+1];    //выделяем место
                wcscpy(inputStrs[k],input+index);   //копируем
                inputStrs[k][i]=0;  //добавляем в конец строки \0
                index=i+1;  //начало следующей строки
                k++;    //количество строк
            }else{
                input[i]='\0';  //отделяем для копирки
                index=i+1;  //начало следующей строки
            }
        }
    }
    //пересчитаем, сколько вышло на самом деле
    int inputStrsCount=0;
    for(int i=0;i<StrCount;i++)
        if(inputStrs[i]!=NULL)
            inputStrsCount++;

    inputStrsCount*=2;
    //массив под строки ввода и вывода
    wchar_t** Strs=new wchar_t*[inputStrsCount];
    for(int i=0;i<inputStrsCount;i++) Strs[i]=NULL;  //указатели в никуда, пусть уж лучше на NULL
    //прогон
    for (int i = 0,k=0;i < inputStrsCount;) {
        size_t len=wcslen(inputStrs[k])+1;
        Strs[i]=new wchar_t[len];    //выделяем место
        wcscpy(Strs[i],inputStrs[k]);   //копируем
        std::wstring outstr = Project::Core::input_to_analize(Strs[i]);
        i++;
        if (outstr==L"") {

        }
        else {
            outstr = L">>> " + outstr;
            Strs[i]=new wchar_t[outstr.length()+1];    //выделяем место
            wcscpy(Strs[i],outstr.c_str());   //копируем
            i++;
        }
        k++;
    }
    QString OUT="";
    for(int i=0;i<inputStrsCount;i++){
        if(i) OUT+='\n';
        OUT+=QString::fromWCharArray(Strs[i]);
    }
    ui->inputText->setText(OUT);
    delete input;
    for(int i=0;i<StrCount;i++)
       if(inputStrs[i]!=NULL)
        delete inputStrs[i];
    delete inputStrs;
    for(int i=0;i<inputStrsCount;i++)
       if(Strs[i]!=NULL)
        delete Strs[i];
    delete Strs;
    return;
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
//! [gesture event handler]
bool MainWindow::gestureEvent(QGestureEvent *event)
{
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    return true;
}
//! [gesture event handler]
//! [swipe function]
void MainWindow::swipeTriggered(QSwipeGesture *gesture)
{
    qCDebug(category) << "swipeTriggered(): swipe to previous";
    int a=0;
    if (gesture->state() == Qt::GestureFinished) {
        if (gesture->horizontalDirection() == QSwipeGesture::Left) {
           a++;
        } else {
           a--;
        }

    }
}
//! [swipe function]
//! [event handler]
bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}
//! [event handler]
