#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include <QFileDialog>
#include "qrencode/qrencode.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGeneratePushButtonClicked();
    void onSaveActionTriggered();
private:
    void encode(const QString& msg);
    void saveQRImageToLocal(const QString& fileName);
private:
    QImage qrCodeImage;
    Ui::MainWindow *ui;
    int qrBorder = 50;
    int qrWidth = 400;
    QLabel* statusLabel;
};
#endif // MAINWINDOW_H