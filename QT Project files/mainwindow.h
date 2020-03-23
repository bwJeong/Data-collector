#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QTimer>
#include <QQueue>
#include <QDir>
#include <QFileDialog>
#include <QLocale>
#include <QDateTime>
#include <QStandardPaths>
#include <QDebug>
#include "windows.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setPort(QString portName);
    void makeSaveDirectory();

signals:
    void dataCameIn();

private slots:
    void readData();
    void saveAs();
    void makeAutoSaveFile();
    void closeAutoSaveFile();
    void autoSave();
    QString TCHARToQString(const TCHAR *ptsz);

    void on_btnConnect_toggled(bool checked);
    void on_btnPortScan_clicked();
    void on_btnSaveAs_toggled(bool checked);
    void on_btnAutoSave_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QSerialPort *port;
    QTimer *rxTimer;
    QFile *saveFile, *autoSaveFile;

    QQueue<char> rxBuffer;
    QString hourCheck = "";
    QString tmpOneLine = "";
    QString oneLine = "";
};

#endif // MAINWINDOW_H
