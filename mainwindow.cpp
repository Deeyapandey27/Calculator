#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    waitingForOperand = true;
    expression = "";
    fixedPrefix = "";
    currentNumberStr = "";

    ui->display->setText("0");
    connect(ui->btn0, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn1, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn2, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn3, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn4, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn5, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn7, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn8, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btn9, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->btnPlus, &QPushButton::clicked, this, &MainWindow::operationClicked);
    connect(ui->btnMinus, &QPushButton::clicked, this, &MainWindow::operationClicked);
    connect(ui->btnmultiply, &QPushButton::clicked, this, &MainWindow::operationClicked);
    connect(ui->btnDivide, &QPushButton::clicked, this, &MainWindow::operationClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateDisplay()
{
    expression = fixedPrefix + currentNumberStr;

    if (expression.isEmpty())
        ui->display->setText("0");
    else
        ui->display->setText(expression);
}
void MainWindow::resetCalculation()
{
    numbers.clear();
    ops.clear();

    fixedPrefix = "";
    currentNumberStr = "";
    expression = "";

    waitingForOperand = true;
}
void MainWindow::digitClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());

    if (!button)
        return;

    QString digit = button->text();

    if (waitingForOperand)
    {
        currentNumberStr = digit;
        waitingForOperand = false;
    }
    else
    {
        if (currentNumberStr == "0")
            currentNumberStr = digit;
        else
            currentNumberStr += digit;
    }

    updateDisplay();
}


void MainWindow::operationClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());

    if (!button)
        return;

    if (currentNumberStr.isEmpty())
        return;

    QString opSymbol = button->text();

    numbers.append(currentNumberStr.toDouble());

    ops.append(opSymbol);

    fixedPrefix += currentNumberStr + " " + opSymbol + " ";

    currentNumberStr = "";
    waitingForOperand = true;

    updateDisplay();
}


void MainWindow::on_btnEqual_clicked()
{
    if (currentNumberStr.isEmpty() && numbers.isEmpty())
        return;

    if (currentNumberStr.isEmpty())
        return;

    numbers.append(currentNumberStr.toDouble());

    double result = numbers[0];

    bool divideByZero = false;

    for (int i = 0; i < ops.size(); i++)
    {
        double next = numbers[i + 1];

        if (ops[i] == "+")
        {
            result += next;
        }
        else if (ops[i] == "-")
        {
            result -= next;
        }
        else if (ops[i] == "*")
        {
            result *= next;
        }
        else if (ops[i] == "/")
        {
            if (next == 0)
            {
                divideByZero = true;
                break;
            }

            result /= next;
        }
    }
    if (divideByZero)
    {
        ui->display->setText("Error");
        resetCalculation();
        return;
    }
    QString fullExpression =
        fixedPrefix + currentNumberStr + " = " + QString::number(result);

    ui->display->setText(fullExpression);
    resetCalculation();
}
void MainWindow::on_btnClear_clicked()
{
    resetCalculation();

    ui->display->setText("0");
}
void MainWindow::on_btnBack_clicked()
{
    if (!currentNumberStr.isEmpty())
    {
        currentNumberStr.chop(1);

        if (currentNumberStr.isEmpty())
            waitingForOperand = true;
    }

    updateDisplay();
}