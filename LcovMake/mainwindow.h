#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    double getSpeedDouble(int min, int max);
    int getSpeedInt(int lower, int upper);
    QStringList getAllFileNames(const QString &folderPath);

    int  allLine_;
    int  passLine_;
    int  allFunc_;
    int  passFunc_;
};

#endif // MAINWINDOW_H
