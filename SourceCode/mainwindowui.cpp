#include "mainwindow.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint)
{
    initUi();
    initLua();
    initCtrl();
}

MainWindow::~MainWindow()
{
    lua_close(m_lua_hander);
}

void MainWindow::initUi()
{
    m_pCovNumEdit->setReadOnly(true);

    m_pRawNumEdit->setToolTip("扫码的原始数据(请始终保持焦点)");
    m_pCovNumEdit->setToolTip("准换后的结果");

    m_pRawNumEdit->setPlaceholderText("扫码的原始数据(请始终保持焦点)");
    m_pCovNumEdit->setPlaceholderText("最近准换后的结果");

    m_pPageFormat->addItem("30 * 30");
    m_pPageFormat->addItem("40 * 40");
    m_pPageFormat->addItem("60 * 40");

    m_pPrinterSetuptBtn->hide();

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget(m_pRawNumEdit,  0, 0, 1, 4);
    pLayout->addWidget(m_pCovNumEdit,  1, 0, 1, 4);
    pLayout->addWidget(m_pTotalCntLbl, 2, 0, 1, 1);
    pLayout->addWidget(m_pLegalCntLbl, 2, 1, 1, 1);
    pLayout->addWidget(m_pPageFormat,  2, 2, 1, 1);
    pLayout->addWidget(m_pExportBtn,   2, 3, 1, 1);
    //pLayout->addWidget(m_pPrinterSetuptBtn,   2, 4, 1, 1);

    setLayout(pLayout);
    setFixedSize(450, 130);
    setWindowIcon(QIcon(":/image/logo.png"));
    setWindowTitle("扫码、转换、打印");
}


