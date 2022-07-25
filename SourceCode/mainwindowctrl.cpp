#include "mainwindow.h"

#include <QKeyEvent>
#include <QEvent>
#include <QTextStream>
#include <QString>
#include <QPrintDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPainter>
#include <QApplication>
#include <QTextEdit>
#include <QPrinter>
#include <QMessageBox>

#include "qrencode/qrencode.h"

#include <QDebug>

void MainWindow::initCtrl()
{
    QMessageBox::information(this, "提醒", "请保证将欲使用的打印机为【系统默认打印机】  ");

    m_pPrintDialog.hide();

    m_pRawNumEdit->installEventFilter(this);

    connect(m_pExportBtn, &QPushButton::clicked, this, &MainWindow::onExportBtnClicked);
    connect(m_pPrinterSetuptBtn, &QPushButton::clicked, this, &MainWindow::onPrinterSetupBtnClicked);
}

void MainWindow::initLua()
{
    m_lua_hander = lua_open();
    if (m_lua_hander == nullptr) {
        QMessageBox::critical(this, "错误", "Lua 引擎无法工作，请联系维护人员  ");
        exit(-1);
    }

    luaL_openlibs(m_lua_hander);

    if (luaL_dofile(m_lua_hander,"numconv.lua")) {
        lua_close(m_lua_hander);
        QMessageBox::critical(this, "错误", "'numconv.lua' 文件错误，请联系维护人员  ");
        exit(-1);
    }
}

QString MainWindow::lua_conv(const QString &num)
{
    if (m_lua_hander == nullptr)
        return "";

    QString ret;

    lua_getglobal(m_lua_hander, "conv");
    lua_pushstring (m_lua_hander, num.toLatin1().data());
    lua_call(m_lua_hander, 1, 1);
    ret = lua_tostring(m_lua_hander, -1);
    lua_pop(m_lua_hander, 1);

    return ret;
}

// 0922067cdfa1e354d4
// 0922067cdfa1e354d5
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    static bool isClear = false;
    if (obj == m_pRawNumEdit && ev->type() == QEvent::KeyPress) {
        QKeyEvent *kev = static_cast<QKeyEvent*>(ev);
        if (kev->key() == Qt::Key_Return || kev->key() == Qt::Key_Enter) {
            QString rawNum = m_pRawNumEdit->text().trimmed();
            QString covNum = lua_conv(rawNum);

            if (!covNum.isEmpty()) {
                QImage image;
                if (m_pPageFormat->currentText() == "60 * 40") {
                    image = generateQRcode(covNum, 260, 260);
                } else if (m_pPageFormat->currentText() == "40 * 40") {
                    image = generateQRcode(covNum, 210, 210);
                } else if (m_pPageFormat->currentText() == "30 * 30") {
                    image = generateQRcode(covNum, 150, 150);
                }

                auto qrcodePath = QCoreApplication::applicationDirPath() + "/qrcode.jpg";  //
                if (image.save(qrcodePath)) {
                    QTextEdit t;
                    qrcodePath.remove(0, 1);
                    qrcodePath.insert(0, "\u202AC");

                    QPrintDialog d;
                    d.hide();

                     auto printer = d.printer();
                     printer->setFullPage(true);

                     if (m_pPageFormat->currentText() == "60 * 40") {
                         t.document()->setPageSize(QSizeF(600, 370));
                         t.insertHtml(QStringLiteral("<div style=\"text-align:center; display: flex;justify-content:center;align-items: center;\">"
                                      "<div>"
                                      "<img src=\"file://%1\" alt=\"1.jpg\">"
                                      "</div>"
                                      "<div style=\"font-size:32px;margin-top:20px\">%2</div>"
                                      "</div>"
                                      "</div>").arg(qrcodePath).arg(covNum.data()));
                         printer->setPageMargins(QMarginsF(0, 4, 0, 5), QPageLayout::Millimeter);
                     } else if (m_pPageFormat->currentText() == "40 * 40") {
                         t.document()->setPageSize(QSizeF(370, 370));
                         t.insertHtml(QStringLiteral("<div style=\"text-align:center; display: flex;justify-content:center;align-items: center;\">"
                                      "<div>"
                                      "<img src=\"file://%1\" alt=\"1.jpg\">"
                                      "</div>"
                                      "<div style=\"font-size:22px;margin-top:20px\">%2</div>"
                                      "</div>"
                                      "</div>").arg(qrcodePath).arg(covNum.data()));
                         printer->setPageMargins(QMarginsF(0, 5, 0, 0), QPageLayout::Millimeter);
                     } else if (m_pPageFormat->currentText() == "30 * 30") {
                         t.document()->setPageSize(QSizeF(200, 200));
                         t.insertHtml(QStringLiteral("<div style=\"text-align:center; display: flex;justify-content:center;align-items: center;\">"
                                      "<div>"
                                      "<img src=\"file://%1\" alt=\"1.jpg\">"
                                      "</div>"
                                      "<div style=\"font-size:12px;margin-top:7px\">%2</div>"
                                      "</div>"
                                      "</div>").arg(qrcodePath).arg(covNum.data()));
                         printer->setPageMargins(QMarginsF(0.3, 2, 0, 0), QPageLayout::Millimeter);
                     }
                     t.print(printer);
                }

                if (!m_record.contains(rawNum)) {
                    m_record.insert(rawNum, {rawNum, covNum, QDateTime::currentDateTime().toString()});
                    m_pLegalCntLbl->setText(QStringLiteral("有效:%1次").arg(m_record.count()));
                    m_pCovNumEdit->clear();
                    m_pCovNumEdit->setText(covNum);
                }
                ++m_rawNumCnt;
                m_pTotalCntLbl->setText(QStringLiteral("总计:%1次").arg(m_rawNumCnt));
            } else {
                m_pCovNumEdit->clear();
                m_pCovNumEdit->setText("编码不合法");
            }
            isClear = true;
        } else if (isClear) {
            m_pRawNumEdit->clear();
            isClear = false;
        }
    }

    return false;
}

// 0922067cdfa1e354d4
// 0922067cdfa1e354d5
//bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
//{
//    static bool isClear = false;
//    if (obj == m_pRawNumEdit && ev->type() == QEvent::KeyPress) {
//        QKeyEvent *kev = static_cast<QKeyEvent*>(ev);
//        if (kev->key() == Qt::Key_Return || kev->key() == Qt::Key_Enter) {
//            QString rawNum = m_pRawNumEdit->text().trimmed();
//            if (rawNum.length() == 18 && rawNum.left(4) == "0922") {
//                auto rawNumMid = rawNum.mid(6, -1);
//                QString covNum;
//                for(int i=0; i<rawNumMid.size(); ++i) {
//                    covNum.append(QString::number(rawNumMid.at(i).toLatin1(), 16));
//                }
//                QImage image = generateQRcode(covNum, 260, 260);
//                auto qrcodePath = QCoreApplication::applicationDirPath() + "/qrcode.jpg";  //
//                if (image.save(qrcodePath)) {
//                    QTextEdit t;

//                    qrcodePath.remove(0, 1);
//                    qrcodePath.insert(0, "\u202AC");

//                    t.document()->setPageSize(QSizeF(600, 370));
//                    t.insertHtml(QStringLiteral("<div style=\"text-align:center; display: flex;justify-content:center;align-items: center;\">"
//                                 "<div>"
//                                 "<img src=\"file://%1\" alt=\"1.jpg\">"
//                                 "</div>"
//                                 "<div style=\"font-size:32px;margin-top:20px\">%2</div>"
//                                 "</div>"
//                                 "</div>").arg(qrcodePath).arg(covNum.data()));

//                     QPrintDialog d;
//                     d.hide();

//                     auto printer = d.printer();
//                     printer->setFullPage(true);
//                     printer->setPageMargins(QMarginsF(0, 4, 0, 5), QPageLayout::Millimeter);
//                     t.print(printer);

//                     qDebug() << m_pPrintDialog.printer()->isValid();
//                }

//                if (!m_record.contains(rawNumMid)) {
//                    m_record.insert(rawNumMid, {rawNumMid, covNum, QDateTime::currentDateTime().toString()});
//                    m_pLegalCntLbl->setText(QStringLiteral("有效:%1次").arg(m_record.count()));
//                    m_pCovNumEdit->clear();
//                    m_pCovNumEdit->setText(covNum);
//                }
//                ++m_rawNumCnt;
//                m_pTotalCntLbl->setText(QStringLiteral("总计:%1次").arg(m_rawNumCnt));
//            } else {
//                m_pCovNumEdit->clear();
//                m_pCovNumEdit->setText("编码不合法");
//            }
//            isClear = true;
//        } else if (isClear) {
//            m_pRawNumEdit->clear();
//            isClear = false;
//        }
//    }

//    return false;
//}

void MainWindow::onExportBtnClicked()
{
    auto path = QFileDialog::getSaveFileName(this, "扫码记录导出", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "表格 (*.csv)");
    if (!path.isEmpty()) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream ts(&file);
            ts << QStringLiteral("源数值") << "," << QStringLiteral("转换后编码") << "," << QStringLiteral("转换时间") << '\n';
            foreach (const auto &item , m_record) {
                ts << item.rawNum << ", " << item.covNum << ", " << item.dataTime << '\n';
            }
            ts.flush();
            file.close();
        }
    }
}

void MainWindow::onPrinterSetupBtnClicked()
{
    m_pPrintDialog.exec();
}

QImage MainWindow::generateQRcode(const QString &str, int width, int hight)
{
   QRcode *qrcode;
   qrcode = QRcode_encodeString(str.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);

   qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
   double scale_x = (double)width / (double)qrcode_width;
   double scale_y = (double) hight /(double) qrcode_width;
   QImage mainimg=QImage(width,hight,QImage::Format_ARGB32);
   QPainter painter(&mainimg);
   QColor background(Qt::white);
   painter.setBrush(background);
   painter.setPen(Qt::NoPen);
   painter.drawRect(0, 0, width, hight);
   QColor foreground(Qt::black);
   painter.setBrush(foreground);
   for( qint32 y = 0; y < qrcode_width; y ++) {
       for(qint32 x = 0; x < qrcode_width; x++) {
           unsigned char b = qrcode->data[y * qrcode_width + x];
           if(b & 0x01) {
               QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
               painter.drawRects(&r, 1);
           }
       }
   }

   return mainimg;
}
