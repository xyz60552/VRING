#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *systemTrayIcon;
    void systemTraySet();
    void closeEvent(QCloseEvent *event);
public slots:
    void timerUpdate(void);
private slots:
    void on_openset_clicked();
    void on_leaveset_clicked();
    void saveData();
    void getsaved();
    void player(QString,int);

};

#endif // MAINWINDOW_H
