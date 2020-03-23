#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    port = new QSerialPort();

    makeSaveDirectory();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPort(QString portName)
{
    port->setPortName(portName);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
}

void MainWindow::makeSaveDirectory()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir saveDirectory(documentsPath + "/MUONDETECTOR_SAVEFILES");
    saveDirectory.mkpath(documentsPath + "/MUONDETECTOR_SAVEFILES");
}

void MainWindow::readData()
{
    QByteArray tmpBuffer = port->readAll();

    for (auto elem : tmpBuffer) {
        rxBuffer.enqueue(elem);

        if (QString(elem) == "\n") {

            while (!rxBuffer.isEmpty()) {
                tmpOneLine += rxBuffer.dequeue();
            }

            oneLine = tmpOneLine;
            tmpOneLine.clear();
            dataCameIn();

            ui->lbRx->setStyleSheet("QLabel { background-color: rgb(0, 255, 0);}");
            QTimer::singleShot(100, ui->lbRx, [&]() {
                ui->lbRx->setStyleSheet("QLabel { background-color : red;}");
            } );
        }
    }
}

void MainWindow::saveAs()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd");
    QString time = dateTime.toString("HH:mm:ss.zzz");
    saveFile->write(date.toStdString().c_str());
    saveFile->write(" ");
    saveFile->write(time.toStdString().c_str());
    saveFile->write(" ");
    saveFile->write(oneLine.toStdString().c_str());
    saveFile->flush();
}

void MainWindow::makeAutoSaveFile()
{
    QString saveDirectoryPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/MUONDETECTOR_SAVEFILES";
    QString fileName = QDateTime::currentDateTime().toString("/yyMMdd_HH0000");
    autoSaveFile = new QFile(saveDirectoryPath + fileName + "_AutoSave.txt");
    autoSaveFile->open(QIODevice::WriteOnly);
}

void MainWindow::closeAutoSaveFile()
{
    autoSaveFile->close();
    delete autoSaveFile;
}

void MainWindow::autoSave()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    if ((hourCheck != dateTime.toString("HH")) && (hourCheck != "")) { // [Need update]
        closeAutoSaveFile();
        makeAutoSaveFile();
    }

    hourCheck = dateTime.toString("HH");
    QString date = dateTime.toString("yyyy-MM-dd");
    QString time = dateTime.toString("HH:mm:ss.zzz");
    autoSaveFile->write(date.toStdString().c_str());
    autoSaveFile->write(" ");
    autoSaveFile->write(time.toStdString().c_str());
    autoSaveFile->write(" ");
    autoSaveFile->write(oneLine.toStdString().c_str());
    autoSaveFile->flush();
}

QString MainWindow::TCHARToQString(const TCHAR *ptsz)
{
    int len = wcslen((wchar_t*)ptsz);
    char* psz = new char[2 * len + 1];
    wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
    QString s = psz;

    delete[] psz;

    return s;
}

void MainWindow::on_btnConnect_toggled(bool checked)
{
    if(checked) {
        setPort(ui->cbPortName->currentText());
        if(!port->open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, "Error", "Cannot start!", "OK");
            ui->btnConnect->setChecked(false);
            ui->btnConnect->setText("Start");
        }
        else {
            connect(port, SIGNAL(readyRead()), this, SLOT(readData()));
            ui->btnConnect->setText("Stop");
            ui->cbPortName->setEnabled(false);
            ui->btnPortScan->setEnabled(false);
        }
    }
    else {
        disconnect(port, SIGNAL(readyRead()), this, SLOT(readData()));
        port->close();
        ui->btnConnect->setText("Start");
        ui->cbPortName->setEnabled(true);
        ui->btnPortScan->setEnabled(true);
        ui->lbRx->setStyleSheet("QLabel { background-color : red;}");
    }
}

void MainWindow::on_btnPortScan_clicked()
{
    ui->cbPortName->clear();

    HKEY hKey;
    RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

    TCHAR szData[20], szName[100];
    DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
    memset(szData, 0x00, sizeof(szData));
    memset(szName, 0x00, sizeof(szName));

    while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL)) {
        index++;

        RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
        QString tem = TCHARToQString(szData);
        ui->cbPortName->addItem(tem);

        memset(szData, 0x00, sizeof(szData));
        memset(szName, 0x00, sizeof(szName));
        dwSize = 100;
        dwSize2 = 20;
    }

    RegCloseKey(hKey);
}

void MainWindow::on_btnSaveAs_toggled(bool checked)
{
    if (checked) {
        QString saveDirectoryPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/MUONDETECTOR_SAVEFILES";
        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    tr("Save as..."),
                    saveDirectoryPath,
                    "txt files (*.txt)"
                    );

        if (fileName != "") {
            ui->btnSaveAs->setText("Saving...");
            ui->btnAutoSave->setEnabled(false);
            saveFile = new QFile(fileName);
            if (saveFile->open(QIODevice::WriteOnly)) {
                connect(this, SIGNAL(dataCameIn()), this, SLOT(saveAs()));
            }
        }
        else {
            ui->btnSaveAs->setCheckable(false);
            ui->btnSaveAs->setCheckable(true);
        }
    }
    else {
        if (saveFile->isOpen()) {
            disconnect(this, SIGNAL(dataCameIn()), this, SLOT(saveAs()));
            saveFile->close();
            delete saveFile;
            ui->btnSaveAs->setText("Save as...");
            ui->btnAutoSave->setEnabled(true);
        }
    }
}

void MainWindow::on_btnAutoSave_toggled(bool checked)
{
    if (checked) {
        makeAutoSaveFile();
        connect(this, SIGNAL(dataCameIn()), this, SLOT(autoSave()));
        ui->btnAutoSave->setText("Auto saving...");
        ui->btnSaveAs->setEnabled(false);
    }
    else {
        disconnect(this, SIGNAL(dataCameIn()), this, SLOT(autoSave()));
        closeAutoSaveFile();
        ui->btnAutoSave->setText("Auto save (Every hour)");
        ui->btnSaveAs->setEnabled(true);
    }
}
