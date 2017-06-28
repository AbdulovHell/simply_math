#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSwipeGesture>
#include "../src/core.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    bool event(QEvent *event) override;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isOutOrEmpty(wchar_t*,size_t);

private slots:
    void Calc();
    void AddPI();
    void ClearArea();
    void AddSQRT();

private:
    bool gestureEvent(QGestureEvent *event);
    void swipeTriggered(QSwipeGesture*);
    Ui::MainWindow *ui;
    Project::Core::data_list* all_math_data;
};

#endif // MAINWINDOW_H
