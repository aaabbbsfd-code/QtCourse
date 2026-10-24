#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>
#include <QJSValue>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), isCalcDone(false)
{
    ui->setupUi(this);
    ui->label_expression->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->label_display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_expr = "";
    updateExprUI();
    //===== 字体放大代码 =====
    QFont fontExpr;
    fontExpr.setPointSize(24);
    ui->label_expression->setFont(fontExpr);
    QFont fontDisp;
    fontDisp.setPointSize(30);
    ui->label_display->setFont(fontDisp);
    QFont fontBtn;
    fontBtn.setPointSize(14);
    ui->btn_0->setFont(fontBtn);
    ui->btn_1->setFont(fontBtn);
    ui->btn_2->setFont(fontBtn);
    ui->btn_3->setFont(fontBtn);
    ui->btn_4->setFont(fontBtn);
    ui->btn_5->setFont(fontBtn);
    ui->btn_6->setFont(fontBtn);
    ui->btn_7->setFont(fontBtn);
    ui->btn_8->setFont(fontBtn);
    ui->btn_9->setFont(fontBtn);
    ui->btn_dot->setFont(fontBtn);
    ui->btn_add->setFont(fontBtn);
    ui->btn_sub->setFont(fontBtn);
    ui->btn_mul->setFont(fontBtn);
    ui->btn_div->setFont(fontBtn);
    ui->btn_eq->setFont(fontBtn);
    ui->btn_clear->setFont(fontBtn);
    ui->btn_back->setFont(fontBtn);
    ui->btn_lparen->setFont(fontBtn);
    ui->btn_rparen->setFont(fontBtn);
    //====================================
    connect(ui->btn_0, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_1, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_2, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_3, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_4, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_5, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_6, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_7, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_8, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_9, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_dot, &QPushButton::clicked, this, &MainWindow::onNumberClicked);
    connect(ui->btn_add, &QPushButton::clicked, this, &MainWindow::onOpClicked);
    connect(ui->btn_sub, &QPushButton::clicked, this, &MainWindow::onOpClicked);
    connect(ui->btn_mul, &QPushButton::clicked, this, &MainWindow::onOpClicked);
    connect(ui->btn_div, &QPushButton::clicked, this, &MainWindow::onOpClicked);
    connect(ui->btn_eq, &QPushButton::clicked, this, &MainWindow::onEqClicked);
    connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->btn_back, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->btn_lparen, &QPushButton::clicked, this, &MainWindow::onLParenClicked);
    connect(ui->btn_rparen, &QPushButton::clicked, this, &MainWindow::onRParenClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateExprUI()
{
    ui->label_expression->setText(m_expr);
}

void MainWindow::onNumberClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString text = btn->text();
    if(isCalcDone)
    {
        m_expr = "";
        ui->label_display->setText("=0");
        isCalcDone = false;
    }
    //小数点单独处理，禁止同操作数多个小数点
    if(text == ".")
    {
        int splitIndex = -1;
        for(int i = m_expr.size()-1;i>=0;i--)
        {
            QChar ch = m_expr[i];
            if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(')
            {
                splitIndex = i;
                break;
            }
        }
        QString currentNum = m_expr.mid(splitIndex+1);
        if(currentNum.contains('.'))
        {
            return;
        }
    }
    m_expr += text;
    updateExprUI();
}

void MainWindow::onOpClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString text = btn->text();
    if(isCalcDone)
    {
        QString resText = ui->label_display->text();
        resText.remove(0,1);
        m_expr = resText;
        isCalcDone = false;
    }
    //操作符拦截：末尾已经是运算符，替换最后一个运算符
    if(!m_expr.isEmpty())
    {
        QChar lastCh = m_expr.back();
        if(lastCh == '+' || lastCh == '-' || lastCh == '*' || lastCh == '/')
        {
            m_expr.chop(1);
        }
    }
    m_expr += text;
    updateExprUI();
}

void MainWindow::onLParenClicked()
{
    if(isCalcDone)
    {
        m_expr = "";
        ui->label_display->setText("=0");
        isCalcDone = false;
    }
    m_expr += "(";
    updateExprUI();
}

void MainWindow::onRParenClicked()
{
    if(isCalcDone)
    {
        m_expr = "";
        ui->label_display->setText("=0");
        isCalcDone = false;
    }
    m_expr += ")";
    updateExprUI();
}

void MainWindow::onBackClicked()
{
    if(isCalcDone) return;
    if(!m_expr.isEmpty())
    {
        m_expr.chop(1);
    }
    updateExprUI();
}

void MainWindow::onClearClicked()
{
    m_expr = "";
    ui->label_display->setText("=0");
    isCalcDone = false;
    updateExprUI();
}

void MainWindow::onEqClicked()
{
    QString calcStr = m_expr;
    calcStr.replace("×","*");
    QJSValue res = m_js.evaluate(calcStr);
    if(res.isNumber())
    {
        double val = res.toNumber();
        if(std::isinf(val) || std::isnan(val))
        {
            ui->label_display->setText("=err");
        }
        else
        {
            ui->label_display->setText("=" + QString::number(val));
        }
    }
    else
    {
        ui->label_display->setText("=err");
    }
    isCalcDone = true;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int k = event->key();
    if(k >= Qt::Key_0 && k <= Qt::Key_9)
    {
        if(isCalcDone)
        {
            m_expr = "";
            ui->label_display->setText("=0");
            isCalcDone = false;
        }
        m_expr += QString::number(k-Qt::Key_0);
    }
    else if(k == Qt::Key_Period)
    {
        if(isCalcDone)
        {
            m_expr = "";
            ui->label_display->setText("=0");
            isCalcDone = false;
        }
        //键盘小数点，同样防重复
        int splitIndex = -1;
        for(int i = m_expr.size()-1;i>=0;i--)
        {
            QChar ch = m_expr[i];
            if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(')
            {
                splitIndex = i;
                break;
            }
        }
        QString currentNum = m_expr.mid(splitIndex+1);
        if(currentNum.contains('.'))
        {
            updateExprUI();
            return;
        }
        m_expr += ".";
    }
    else if(k == Qt::Key_Plus)
    {
        if(isCalcDone)
        {
            QString resText = ui->label_display->text();
            resText.remove(0,1);
            m_expr = resText;
            isCalcDone = false;
        }
        //键盘加号拦截
        if(!m_expr.isEmpty())
        {
            QChar lastCh = m_expr.back();
            if(lastCh == '+' || lastCh == '-' || lastCh == '*' || lastCh == '/')
            {
                m_expr.chop(1);
            }
        }
        m_expr += "+";
    }
    else if(k == Qt::Key_Minus)
    {
        if(isCalcDone)
        {
            QString resText = ui->label_display->text();
            resText.remove(0,1);
            m_expr = resText;
            isCalcDone = false;
        }
        if(!m_expr.isEmpty())
        {
            QChar lastCh = m_expr.back();
            if(lastCh == '+' || lastCh == '-' || lastCh == '*' || lastCh == '/')
            {
                m_expr.chop(1);
            }
        }
        m_expr += "-";
    }
    else if(k == Qt::Key_Asterisk)
    {
        if(isCalcDone)
        {
            QString resText = ui->label_display->text();
            resText.remove(0,1);
            m_expr = resText;
            isCalcDone = false;
        }
        if(!m_expr.isEmpty())
        {
            QChar lastCh = m_expr.back();
            if(lastCh == '+' || lastCh == '-' || lastCh == '*' || lastCh == '/')
            {
                m_expr.chop(1);
            }
        }
        m_expr += "*";
    }
    else if(k == Qt::Key_Slash)
    {
        if(isCalcDone)
        {
            QString resText = ui->label_display->text();
            resText.remove(0,1);
            m_expr = resText;
            isCalcDone = false;
        }
        if(!m_expr.isEmpty())
        {
            QChar lastCh = m_expr.back();
            if(lastCh == '+' || lastCh == '-' || lastCh == '*' || lastCh == '/')
            {
                m_expr.chop(1);
            }
        }
        m_expr += "/";
    }
    else if(k == Qt::Key_ParenLeft)
    {
        if(isCalcDone)
        {
            m_expr = "";
            ui->label_display->setText("=0");
            isCalcDone = false;
        }
        m_expr += "(";
    }
    else if(k == Qt::Key_ParenRight)
    {
        if(isCalcDone)
        {
            m_expr = "";
            ui->label_display->setText("=0");
            isCalcDone = false;
        }
        m_expr += ")";
    }
    else if(k == Qt::Key_Backspace)
    {
        if(isCalcDone) return;
        if(!m_expr.isEmpty())
            m_expr.chop(1);
    }
    else if(k == Qt::Key_Escape)
    {
        m_expr = "";
        ui->label_display->setText("=0");
        isCalcDone = false;
    }
    else if(k == Qt::Key_Enter || k == Qt::Key_Return)
    {
        onEqClicked();
        return;
    }
    updateExprUI();
}
