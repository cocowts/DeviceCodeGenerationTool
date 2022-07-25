#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QStringLiteral>
#include <QDateTime>
#include <QHash>
#include <QPrinter>
#include <QPrintDialog>
#include <QComboBox>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

class MainWindow : public QWidget
{
    Q_OBJECT

    struct DataItem{
        QString rawNum;
        QString covNum;
        QString dataTime;
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

protected slots:
    void onExportBtnClicked();
    void onPrinterSetupBtnClicked();

private:
    void initUi();
    void initCtrl();
    void initLua();
    QImage generateQRcode(const QString &str, int width, int hight);
    QString lua_conv(const QString &num);

private:
    QLineEdit *m_pRawNumEdit      = new QLineEdit(this);
    QLineEdit *m_pCovNumEdit      = new QLineEdit(this);
    QLabel *m_pTotalCntLbl        = new QLabel(QStringLiteral("总计:0次"), this);
    QLabel *m_pLegalCntLbl        = new QLabel(QStringLiteral("有效:0次"), this);
    QPushButton *m_pExportBtn     = new QPushButton(QStringLiteral("导出本次"), this);
    QComboBox *m_pPageFormat      = new QComboBox(this);
    QPushButton *m_pPrinterSetuptBtn = new QPushButton(QStringLiteral("打印设置"), this);

    int m_rawNumCnt = 0;
    QHash<QString, DataItem> m_record;

    QPrintDialog m_pPrintDialog;

    lua_State *m_lua_hander = nullptr;
};
#endif // MAINWINDOW_H
