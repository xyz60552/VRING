#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QFileDialog>
#include <QPushButton>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QFile>
#include <QAction>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMessageBox>

QString poo="/",lp,op;
QString plano[13]={"08:00 AM","08:50 AM","09:50 AM","10:40 AM","11:30 AM","02:00 PM","02:50 PM","03:50 PM","04:40 PM","05:30 PM","07:00 PM","07:50 PM"};
QString planl[13]={"08:45 AM","09:35 AM","10:35 AM","11:25 AM","12:15 AM","02:45 PM","03:35 PM","04:35 PM","05:25 PM","06:15 PM","07:45 PM","08:35 PM"};
void MainWindow::getsaved()
{
        QByteArray line01, line02;

        QFile fl("leavingpath.txt");
        if (!fl.exists())
        {
            qDebug() << "The file is not exist!" ;
        }
        else if (!fl.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug() << "error" ;
        else
        {
            line02 = fl.readLine();
            if(line02!=""){
                lp=line02;
                ui->pathl->setText(lp);
                poo=QFileInfo(op).absoluteDir().path()+"/";
            }
        }
        fl.close();

        QFile fo("openingpath.txt");
        if (!fo.exists())
        {
            qDebug() << "The file is not exist!" ;
        }
        else if (!fo.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug() << "error" ;
        else
        {
            line01 = fo.readLine(); //逐行读取的方式读取全部内容。
            if(line01!=""){
                op=line01;
                poo=QFileInfo(op).absoluteDir().path()+"/";
                ui->patho->setText(op);
            }
        }
        fo.close();


}
void MainWindow::timerUpdate(void)
{
    int i;
    static qint8 ock[12]={0},lck[12]={0};
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("hh:mm AP");
    ui->timelab->setText(str);
    for(i=0;i<12;i++)
    {
        if(ock[(i+1)%12]==0&&str==plano[i])
        {
            ock[(i+1)%12]=1;
            ock[i]=0;
            player(op,8);
            break;
        }
    }
    for(i=0;i<12;i++)
    {
        if(lck[(i+1)%12]==0&&str==planl[i])
        {
            lck[(i+1)%12]=1;
            lck[i]=0;
            player(lp,13);
            break;
        }
    }

}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->systemTraySet();
    this->statusBar()->setSizeGripEnabled(false);
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    show();
    qDebug()<<"always top";
    //Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(Qt::Widget);
    //setWindowFlags(NULL);
    show();
    qDebug()<<"no always top";

    getsaved();
       QTimer *timer = new QTimer(this);
       timerUpdate();
       connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
       timer->start(2000);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::player(QString x,int vol)
{
    if(x.isEmpty()) return;
    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile(x));
    //connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    audioOutput->setVolume(vol);
    player->play();
}
void MainWindow::on_openset_clicked()
{

         QString po = QFileDialog::getOpenFileName(
                    this, "选择要播放的上课铃",
                    poo,
                    "声音文件 (*.wav *.mp3)");
        if (!po.isEmpty())
        {
            poo=QFileInfo(po).absoluteDir().path()+"/";
            op=po;
            ui->patho->setText(op);
        }
}


void MainWindow::on_leaveset_clicked()
{
    QString pl = QFileDialog::getOpenFileName(
               this, "选择要播放的下课铃",
               poo,
               "声音文件 (*.wav *.mp3)");
    if (!pl.isEmpty())
    {
        poo=QFileInfo(pl).absoluteDir().path()+"/";
        lp=pl;
        ui->pathl->setText(lp);
    }
}

void MainWindow::saveData(){
    //保存为TXT文件
    /*bool exist;

    QDir *folder = new QDir;

     * exist = folder->exists("C:/Users/no2/Desktop");//查看目录是否存在（例子是保存到桌面）

    if(!exist){//不存在就创建
        bool ok = folder->mkdir("C:/Users/no2/Desktop");
        if(ok){
            QMessageBox::warning(this,tr("创建目录"),tr("创建成功!"));//添加提示方便查看是否成功创建
        }else{
            QMessageBox::warning(this,tr("创建目录"),tr("创建失败"));
        }
    }
    */

    QFile f("openingpath.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){//不追加写入 不添加结束符\r\n
         QMessageBox::warning(this,"错误","打开文件失败,数据保存失败");
        return ;
    }
    else{
        f.write(op.toUtf8());
    }
    f.close();
    QFile f2("leavingpath.txt");
    if(!f2.open(QIODevice::WriteOnly | QIODevice::Text)){//不追加写入 不添加结束符\r\n
         QMessageBox::warning(this,"错误","打开文件失败,数据保存失败");
        return ;
    }
    else{
        f2.write(lp.toUtf8());
    }
    f2.close();

}
void MainWindow::systemTraySet()
{
    /**
     * 创建菜单对象和托盘图标对象
     * */
    systemTrayIcon = new QSystemTrayIcon(this);
    // 设置图标 首先要添加一个 qt resource文件，然后将文件添加进去，右键选择文件复制链接即可
    systemTrayIcon->setIcon(QIcon(":/new/prefix1/light-bulb-2010022_1280.png"));
    systemTrayIcon->setToolTip("上下课铃");
    // 设置托盘 单击点击事件显示窗口
    connect(systemTrayIcon ,&QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason)
    {
        if(reason == QSystemTrayIcon::Trigger){
            if(this->isHidden())
            {
                Qt::WindowFlags m_flags = windowFlags();
                setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
                show();
                qDebug()<<"always top";
                //Qt::WindowFlags m_flags = windowFlags();
                setWindowFlags(Qt::Widget);
                //setWindowFlags(NULL);
                show();
                qDebug()<<"no always top";
            }
            else
                this->hide();
        }

    });
    // 设置右键菜单
    QMenu* menu = new QMenu(this);
    // 显示选项
    QAction* showBtn = new QAction( "显示/隐藏", this);
    connect(showBtn , &QAction::triggered, this, [=]()
    {
        if(this->isHidden())
        {
            Qt::WindowFlags m_flags = windowFlags();
            setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
            show();
            qDebug()<<"always top";
            //Qt::WindowFlags m_flags = windowFlags();
            setWindowFlags(Qt::Widget);
            //setWindowFlags(NULL);
            show();
            qDebug()<<"no always top";
        }
        else
            this->hide();
    });
    menu->addAction(showBtn);
    // 退出选项
    QAction* exitBtn = new QAction( "退出", this);
    connect(exitBtn , &QAction::triggered, this, [=]()
    {
        qDebug()<<"exit";
        saveData();
        QApplication::exit(0);
    });
    menu->addAction(exitBtn);

    systemTrayIcon->setContextMenu(menu);
    // 显示
    systemTrayIcon->show();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    // 使用ignore，这样窗口就不会关闭了
    event->ignore();
}
