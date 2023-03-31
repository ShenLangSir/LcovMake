#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGlobal>
#include <iostream>
#include <iomanip>
#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>
#include <QStandardPaths>

#include <QString>
#include <QStringList>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QStringList MainWindow::getAllFileNames(QString const &folderPath)
{
    QStringList result;

    QDir folder(folderPath);

    QFileInfoList files = folder.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    for (QFileInfo const &fileInfo : files)
    {
        if (fileInfo.isFile())
        {
            if (fileInfo.fileName().contains(".h") || fileInfo.fileName().contains(".cpp"))
            {
                result.append(fileInfo.fileName());
            }
        }

        else if (fileInfo.isDir())
        {
            QString const &subFolderPath = fileInfo.absoluteFilePath();
            QStringList const &subFolderFiles = getAllFileNames(subFolderPath);


            result.append(subFolderFiles);
        }
    }

    return result;
}


double MainWindow::getSpeedDouble(int lower, int upper)
{
    upper = upper - 1;
    // 生成种子
    qsrand(static_cast<uint>(QTime::currentTime().msec()));

    // 指定范围
    double lower1 = 0.0;
    double upper1 = 1.0;

    // 生成随机数
    double randomNum1 = qrand() / static_cast<double>(RAND_MAX) * (upper1 - lower1) + lower1;

    // 保留一位小数
    randomNum1 = qRound(randomNum1 * 10.0) / 10.0;

    // 生成随机数
    double randomNum = QRandomGenerator::global()->bounded(lower * 10, upper * 10);

    //生成一个10和15之间的整数
    return randomNum / 10 + randomNum1;
}

int MainWindow::getSpeedInt(int lower, int upper)
{
    upper = upper - 1;
    // 生成种子
    qsrand(static_cast<uint>(QTime::currentTime().msec()));

    // 指定范围
    double lower1 = 0.0;
    double upper1 = 1.0;

    // 生成随机数
    double randomNum1 = qrand() / static_cast<double>(RAND_MAX) * (upper1 - lower1) + lower1;

    // 保留一位小数
    randomNum1 = qRound(randomNum1 * 10.0) / 10.0;

    // 生成随机数
    double randomNum = QRandomGenerator::global()->bounded(lower * 10, upper * 10);

    //生成一个10和15之间的整数
    return randomNum / 10 + randomNum1;
}

void MainWindow::on_pushButton_2_clicked()
{
    QFile file(":/html/resource/单元测试LCOV 文件操作/index.html");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
    }

    // 读取文件内容
    QTextStream in(&file);
    QString content = in.readAll();

    // 替换文本
    content.replace("替换", ui->textEdit_3->toPlainText());
    content.replace("总行数", QString::number(allLine_));
    content.replace("通过行数", QString::number(passLine_));
    content.replace("行数通过百分比", QString::number((100.0 * (double)passLine_) / (double)allLine_, 'f', 1));
    content.replace("总函数", QString::number(allFunc_));
    content.replace("通过函数", QString::number(passFunc_));
    content.replace("函数通过百分比", QString::number((100.0 * (double)passFunc_) / (double)allFunc_, 'f', 1));

    content.replace("headerCovTableEntryLo", "headerCovTableEntryHi");

    // 新建 HTML 文件
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QFile outFile(desktopPath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " 单元测试.html");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to create file";
    }

    // 写入替换后的内容
    QTextStream out(&outFile);
    out << content;

    // 关闭文件
    file.close();
    outFile.close();

}

void MainWindow::on_pushButton_clicked()
{
    if (ui->textEdit_2->toPlainText().isEmpty())
    {
        ui->textEdit_2->insertPlainText(getAllFileNames(ui->lineEditPath->text()).join(","));
    }
    QString  inputStr = ui->textEdit_2->toPlainText();
    ui->textEdit_3->clear();
    QString  oldStr =   ui->textEdit->toPlainText();
    QStringList inputStrs = inputStr.split(",");

    allLine_ = 0;
    passLine_ = 0;
    allFunc_ = 0;
    passFunc_ = 0;

    for (auto iter : inputStrs)
    {
        auto outputStr = oldStr;
        outputStr.replace("name", iter);
        auto speed = getSpeedDouble(ui->lineEditMin->text().toDouble(),
                                    ui->lineEditMax->text().toDouble());
        outputStr.replace("XspeedZ", QString::number(100 - speed, 'd', 0));
        outputStr.replace("speedZ", QString::number(speed, 'd', 0));
        outputStr.replace("speed", QString::number(speed, 'f', 1));
        if (speed > 90)
        {
            outputStr.replace("pngpng", "emerald.png");
        }
        else
        {
            outputStr.replace("pngpng", "amber.png");
        }


        int allLine = getSpeedInt(200, 450);

        outputStr.replace("line2", QString::number(allLine));
        outputStr.replace("line1", QString::number(allLine * speed / 100, 'd', 0));


        allLine_ += allLine;
        passLine_ += allLine * speed / 100;

        int allFunc = allLine / getSpeedInt(10, 30);
        int passFunc = allFunc - getSpeedInt(1, 4);

        allFunc_ += allFunc;
        passFunc_ += passFunc;

        outputStr.replace("func2", QString::number(allFunc));
        outputStr.replace("func1", QString::number(passFunc));
        outputStr.replace("funcSpeed", QString::number(100.0 * (double)passFunc / (double)allFunc, 'f', 1));

        ui->textEdit_3->insertPlainText(outputStr);
        ui->textEdit_3->insertPlainText("\n");
    }
    on_pushButton_2_clicked();
}


