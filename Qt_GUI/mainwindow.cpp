#include <string>
#include <wchar.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../src/error.h"
#include "../src/IO.h"
#include "../src/filters.h"
#include "../src/core.h"
#include "../src/html_addon.h"
#include <QFile>
#include <QLoggingCategory>
QLoggingCategory category("default");
using namespace Project::Core;

bool MainWindow::isOutOrEmpty(wchar_t* str, size_t len) {
	if (str == L"") return true;

	if (!(len > 3)) return false;
	if ((str[0] == '>'&&str[1] == '>'&&str[2] == '>'))
		return true;
	else return false;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    all_math_data = Init();

	//! [enable gestures]
	ui->centralWidget->grabGesture(Qt::SwipeGesture);
	grabGesture(Qt::SwipeGesture);
	ui->inputText->grabGesture(Qt::SwipeGesture);
	//! [enable gestures]

	QImage imgCalc, imgClear, imgSqrt, imgPI;
	imgCalc.load(":/images/calc");
	imgClear.load(":/images/waste");
	imgSqrt.load(":/images/sqrt");
	imgPI.load(":/images/pi");

#ifdef Q_OS_ANDROID
#define ICON_SIZE 64
	QSize iSize = ui->toolBar->iconSize();
	iSize.setHeight(ICON_SIZE);
	iSize.setWidth(ICON_SIZE);
	ui->toolBar->setIconSize(iSize);
	ui->toolBar->addAction(QPixmap::fromImage(imgCalc.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio)), "Calc", this, SLOT(Calc()));
	ui->toolBar->addAction(QPixmap::fromImage(imgClear.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio)), "Clear", this, SLOT(ClearArea()));
	ui->toolBar->addAction(QPixmap::fromImage(imgSqrt.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio)), "√", this, SLOT(AddSQRT()));
	ui->toolBar->addAction(QPixmap::fromImage(imgPI.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio)), "π", this, SLOT(AddPI()));
#else
	ui->toolBar->addAction(QPixmap::fromImage(imgCalc.scaled(32, 32, Qt::KeepAspectRatio)), "Calc", this, SLOT(Calc()));
	ui->toolBar->addAction(QPixmap::fromImage(imgClear.scaled(32, 32, Qt::KeepAspectRatio)), "Clear", this, SLOT(ClearArea()));
	ui->toolBar->addAction(QPixmap::fromImage(imgSqrt.scaled(32, 32, Qt::KeepAspectRatio)), "√", this, SLOT(AddSQRT()));
	ui->toolBar->addAction(QPixmap::fromImage(imgPI.scaled(32, 32, Qt::KeepAspectRatio)), "π", this, SLOT(AddPI()));
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

void MainWindow::Calc() {
    //wstring str = L"biboran";
    //str = Project::HTML::ApplyEffects(str, Project::HTML::Actions::BOLD, Project::HTML::Actions::ITALIC);
    //str = Project::HTML::ChangeTextColor(str, Project::HTML::HTMLColors::RED);
    //str = Project::HTML::AddHeader(str,24);
    //QString qstr=QString::fromWCharArray(str.c_str());
    //ui->inputText->setHtml(qstr);
    //return;
	//поле ввода в строку
	QString inputTxt = ui->inputText->toPlainText();
	int StrCount = 0;
	int inputSize = inputTxt.size();
	//массив под строку, преобразование из QString в wchar_t*
	wchar_t* input = new wchar_t[inputSize + 1];
	inputTxt.toWCharArray(input);
	input[inputSize] = 0;
	inputSize++;
	//есть строка input, длинны inputSize, считаем количество подстрок
	for (int i = 0; i < inputSize; i++) {
		if ((input[i] == '\n') || (input[i] == 0))
			StrCount++;
	}
	if (StrCount < 1) return;  //если пусто
	//массив под отдельные строки
	wchar_t** inputStrs = new wchar_t*[StrCount];
	for (int i = 0; i < StrCount; i++) inputStrs[i] = NULL;  //указатели в никуда, пусть уж лучше на NULL

	int index = 0;
	for (int i = 0, k = 0; i < inputSize; i++) {   //по всей длинне общей строки
		if ((input[i] == '\n') || (input[i] == 0)) {    //если встречаем разделитель
            input[i] = '\0';  //отделяем для копирки
			inputStrs[k] = new wchar_t[i - index + 1];    //выделяем место
			wcscpy(inputStrs[k], input + index);   //копируем
			inputStrs[k][i] = 0;  //добавляем в конец строки \0
			index = i + 1;  //начало следующей строки
			k++;    //количество строк
		}
	}
	//пересчитаем, сколько вышло на самом деле
	int inputStrsCount = 0;
	for (int i = 0; i < StrCount; i++)
		if (inputStrs[i] != NULL)
			inputStrsCount++;

    int len = all_math_data->size_s();	//количество линий в мат векторе
    int out_strings = 0;
    data_list* place;
    wstring *in_str;
    bool needReCalc = false;
	//прогон
	for (int i = 0; i < inputStrsCount; i++) {
        if (!isOutOrEmpty(inputStrs[i],wcslen(inputStrs[i]))) {

            in_str = new wstring(inputStrs[i]);

            place = all_math_data->at(i - out_strings);
			if (place != NULL) {
				if ((place->compare_in(in_str)) || (needReCalc)) {//если строчки не совпадают
					place->in = *in_str;
					place->run_string();
					needReCalc = true;
				}
				else {
					//строки совпали
				}
			}
			else {
				place = new data_list(in_str);
				all_math_data->push_back(place);
				//в run_string текст ошибки сразу записывается в поле вывода.
				place->run_string();
			}
		}	//if (!isOutOrEmpty(strs[i]))
		else {
			out_strings++;//посчитать количество строк вывода из предыдущих выполнений программы
		}
	}
    if (inputStrsCount - out_strings < len)
	{
		//если количество введённых строк (с учётом вывода в предыду оказалось меньше числа злементов в списке данных - удаляем лишнее
        all_math_data->delete_starting_at(inputStrsCount - out_strings);
	}

	if (!input_to_analize(all_math_data))
	{
		//очень глобальная ошибка. пока что она не предусмотрена нигде.
		//err
	}
	//далее all_math_data уже обработана в ядре
    len = all_math_data->size_s();//количество элементов.
    inputStrsCount-=out_strings;
	//резервируем массив под строки ввода и вывода
    wchar_t** Strs = new wchar_t*[inputStrsCount * 2];
    for (int i = 0; i < inputStrsCount*2; i++) Strs[i] = NULL;  //указатели в никуда, пусть уж лучше на NULL

    for (int i = 0, k = 0; i < len; i++){
                //in
                wstring tIn=all_math_data->at(i)->in;
                tIn=Project::HTML::ApplyEffects(tIn, Project::HTML::Actions::ITALIC);
                //tIn=Project::HTML::ChangeTextColor(tIn, Project::HTML::HTMLColors::MEDIUMBLUE);
                tIn=Project::HTML::NewString(tIn);
                Strs[k] = new wchar_t[wcslen(tIn.c_str())+1];
                wcscpy(Strs[k++],tIn.c_str());
                //out
                if(!all_math_data->at(i)->out.empty()){
                    wstring tOut=L">>> "+all_math_data->at(i)->out;
                    //tOut=Project::HTML::ApplyEffects(tOut, Project::HTML::Actions::BOLD);
                    if(all_math_data->at(i)->math==NULL)
                        tOut=Project::HTML::ChangeTextColor(tOut, Project::HTML::HTMLColors::MAROON);
                    else
                        tOut=Project::HTML::ChangeTextColor(tOut, Project::HTML::HTMLColors::GRAY);
                    tOut=Project::HTML::NewString(tOut);
                    Strs[k] = new wchar_t[wcslen(tOut.c_str())+1];
                    wcscpy(Strs[k++],tOut.c_str());
                }
    }
#define DEFAULT_BACKGROUND
#ifdef DEFAULT_BACKGROUND
	QString OUT = "";
    for (int i = 0; i < inputStrsCount*2; i++) {
        if(Strs[i]!=NULL)
            OUT += QString::fromWCharArray(Strs[i]);
	}
    ui->inputText->setText(OUT);
#else
    wstring ALL_Strs=L"";
    for (int i = 0; i < inputStrsCount*2; i++) {
        if(Strs[i]!=NULL)
            ALL_Strs += Strs[i];
    }
    ALL_Strs=Project::HTML::AddHeader(ALL_Strs,14,Project::HTML::HTMLColors::GAINSBORO);
    QString OUT = QString::fromWCharArray(ALL_Strs.c_str());
    ui->inputText->setHtml(OUT);
#endif
	delete input;
	for (int i = 0; i < StrCount; i++)
		if (inputStrs[i] != NULL)
			delete inputStrs[i];
	delete inputStrs;
	for (int i = 0; i < inputStrsCount; i++)
		if (Strs[i] != NULL)
			delete Strs[i];
	delete Strs;
	return;
}

void MainWindow::AddPI() {
	ui->inputText->setText(ui->inputText->toPlainText() + "π");
}

void MainWindow::AddSQRT() {
	QTextCursor a = ui->inputText->textCursor();
	ui->inputText->setText(ui->inputText->toPlainText() + "root(,)");
	a.atEnd();
	a.setPosition(a.position() - 2);
	ui->inputText->setTextCursor(a);
}

void MainWindow::ClearArea() {
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
	int a = 0;
	if (gesture->state() == Qt::GestureFinished) {
		if (gesture->horizontalDirection() == QSwipeGesture::Left) {
			a++;
		}
		else {
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
