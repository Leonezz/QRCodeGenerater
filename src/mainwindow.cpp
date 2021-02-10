#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), statusLabel(new QLabel)
{
    ui->setupUi(this);
    qrCodeImage = QImage(qrWidth, qrWidth, QImage::Format::Format_RGB888);
    this->setWindowTitle(tr("QRCode Generater"));
    this->statusBar()->addWidget(this->statusLabel);

    this->ui->comboBox->addItems({"正常模式", "自动模式"});

    connect(ui->pushButtonGenerate, &QPushButton::clicked,
            this, &MainWindow::onGeneratePushButtonClicked);
    connect(ui->actionsave, &QAction::triggered, this, &MainWindow::onSaveActionTriggered);
}

void MainWindow::encode(const QString &msg)
{
    qrCodeImage.fill(QColor(255, 255, 255));
    const int qrAreaWidth = qrWidth - 2 * qrBorder;
    QRcode *code = QRcode_encodeString(msg.toStdString().c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, 1);
    const int qrNodeCnt = code->width;
    const int qrNodeWidth = qrAreaWidth / qrNodeCnt;

    QPainter painter(&this->qrCodeImage);
    painter.setPen(QColor(0, 0, 0));
    painter.setBrush(QColor(0, 0, 0));
    for (int i = 0; i < qrNodeCnt; ++i)
    {
        for (int j = 0; j < qrNodeCnt; ++j)
        {
            if (code->data[i * qrNodeCnt + j] & 0x01)
            {
                painter.drawRect(i * qrNodeWidth + qrBorder, j * qrNodeWidth + qrBorder, qrNodeWidth, qrNodeWidth);
            }
        }
    }
}

void MainWindow::onSaveActionTriggered()
{
    const QString fileName =
        QFileDialog::getSaveFileName(this, tr("save Image"), ".");
    if (fileName.isEmpty())
    {
        this->statusLabel->setText(tr("Error: select a file path before saving"));
        return;
    }
    saveQRImageToLocal(fileName);
}

void MainWindow::saveQRImageToLocal(const QString &fileName)
{
    this->qrCodeImage.save(fileName, "png");
}

void MainWindow::generateQRCodesFromDataBase()
{
    QFile dataBaseJsonFile("./json/database.json");
    if (!dataBaseJsonFile.open(QIODevice::ReadOnly))
    {
        this->statusLabel->setText("Error: database file open failed.");
        return;
    }
    QByteArray jsonBytes = dataBaseJsonFile.readAll();
    dataBaseJsonFile.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray jsonArr = jsonObj.value("database").toArray();
    QString id;
    const QString imgDir = "./imgs/";
    for (auto &&obj : jsonArr)
    {
        id = obj.toObject().value("id").toString();
        encode(id);
        saveQRImageToLocal(imgDir + id);
    }
}

void MainWindow::onGeneratePushButtonClicked()
{
    if (this->ui->comboBox->currentIndex() == 0)
    {
        const QString msg = ui->TextEdit->toPlainText();
        if (msg.isEmpty())
        {
            this->statusLabel->setText(tr("Error: input text before generating."));
            return;
        }
        encode(msg);
        this->ui->QRLabel->setPixmap(QPixmap::fromImage(qrCodeImage));
    }
    else
    {
        generateQRCodesFromDataBase();
    }
    
}

MainWindow::~MainWindow()
{
}
