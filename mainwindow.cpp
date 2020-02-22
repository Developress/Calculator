#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QString>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->One, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Two, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Three, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Four, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Five, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Six, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Seven, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Nine, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Eight, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Zero, SIGNAL(clicked()), this, SLOT(NewDigit()) );
    connect(ui->Add, SIGNAL(clicked()), this, SLOT(onOperationClicked()) );
    connect(ui->Minus, SIGNAL(clicked()), this, SLOT(onOperationClicked()) );
    connect(ui->Multiply, SIGNAL(clicked()), this, SLOT(onOperationClicked()) );
    connect(ui->divide, SIGNAL(clicked()), this, SLOT(onOperationClicked()) );
    setWindowTitle("Calculator");
}

QString buffer;
int addCounter, diffCounter, multCounter, divideCounter;
double num1, num2, result;
std::string substring, buffCopy;
bool equalClicked = false;

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::NewDigit(){
    if(equalClicked){
        buffer="";
        equalClicked = false;
    }
    if(buffer[0]=='0' && buffer.length()==1){
        buffer="";
    }
    QPushButton *pushBtn = (QPushButton *)sender();
    buffer+=pushBtn->text();
    ui->Field->setText(buffer);
}

void MainWindow::onOperationClicked(){
    if(equalClicked)
        equalClicked = false;
    QPushButton *pushBtn = (QPushButton *)sender();
    if((buffer.length()==0 && !(pushBtn->text()=='-'))||(buffer.length()==1&&buffer[0]=='-'))
        return;
    QString operation = pushBtn->text();
    if(buffer.length()>1&&(buffer[buffer.length()-1] == '+'
       || buffer[buffer.length()-1] == '-'
       || buffer[buffer.length()-1] == '*'
       || buffer[buffer.length()-1] == '/'))
        buffer = buffer.left(buffer.length()-1);
    buffer+=operation;
    ui->Field->setText(buffer);
    if(operation=="+")
        addCounter++;
    else if(operation=="-")
        diffCounter++;
    else if(operation=="*")
        multCounter++;
    else if(operation=="/")
        divideCounter++;
}

void MainWindow::executeOperation(int counter, char operation){
    int startIndex, endIndex;
    buffCopy = buffer.toUtf8().constData();
    for(int i = 0; i<counter; i++){
        for(int j = 0; j<buffer.length(); j++){
            if(buffer[j] == operation){
               num1 = getLeftOperand(j-1, &startIndex);
               num2 = getRightOperand(j+1, &endIndex);
               if(operation=='+')
                   result = num1+num2;
               else if(operation=='-')
                   result = num1-num2;
               else if(operation=='*')
                   result = num1*num2;
               else if(operation=='/')
                   result = num1/num2;
               substring = buffCopy.substr(startIndex, endIndex);
               buffCopy = buffCopy.replace(startIndex, substring.length(), std::to_string(result));
               buffer = QString::fromStdString(buffCopy);
            }
        }
    }
}

double MainWindow::getLeftOperand(int index, int* startIndex){
    for(int i = index; i>=0; i--){
        if(buffer[i] == '+'
           || (buffer[i] == '-' && i!=0)
           || buffer[i] == '*'
           || buffer[i] == '/'){
            *startIndex = i+1;
            buffCopy = buffer.toUtf8().constData();
            substring = buffCopy.substr(*startIndex, index-*startIndex+1);
            return atof(substring.c_str());
    }
}
    *startIndex = 0;
    buffCopy = buffer.toUtf8().constData();
    substring = buffCopy.substr(0, index+1);
    return atof(substring.c_str());
}

double MainWindow::getRightOperand(int index, int* endIndex){
        for(int i = index; i<buffer.length(); i++){
            if(buffer[i] == '+'
               || buffer[i] == '-'
               || buffer[i] == '*'
               || buffer[i] == '/'){
                *endIndex = i-1;
                buffCopy = buffer.toUtf8().constData();
                substring = buffCopy.substr(index,*endIndex-index+1);
                return QString::fromStdString(substring).toDouble();
        }
    }
        *endIndex = buffer.length()-1;
        buffCopy = buffer.toUtf8().constData();
        substring = buffCopy.substr(index, *endIndex);
        return atof(substring.c_str());
}

void MainWindow::on_Dot_clicked()
{
    std::string str = buffer.toUtf8().constData();
    QString qstr = QString::fromStdString(str.substr(getPositionOfOperation(), buffer.length()-getPositionOfOperation()));
    if(buffer.length()>0 && buffer[buffer.length()-1]!='.' && !(qstr.contains('.'))){
        buffer+='.';
        ui->Field->setText(buffer);
    }
}

void MainWindow::on_clear_clicked()
{
    buffer = buffer.left(buffer.length()-1);
    ui->Field->setText(buffer);
}

void MainWindow::on_Equal_clicked()
{
    if(buffer[buffer.length()-1]=='+'||buffer[buffer.length()-1]=='-'
         ||buffer[buffer.length()-1]=='*'||buffer[buffer.length()-1]=='*')
        return;
    executeOperation(multCounter, '*');
    executeOperation(divideCounter, '/');
    executeOperation(addCounter, '+');
    executeOperation(diffCounter, '-');
    ui->Field->setText(buffer);
    equalClicked = true;
}

void MainWindow::on_plusMinus_clicked()
{
    if(!(buffer.contains('+')||buffer.contains('-')
         ||buffer.contains('*')||buffer.contains('/'))){
        if(buffer[0].isDigit()){

            buffer.insert(0, '-');
            ui->Field->setText(buffer);
        }
    } else {
         if(buffer[0]=='-'){
            buffer.remove(0,1);
            ui->Field->setText(buffer);
        }
    }
}

int MainWindow::getPositionOfOperation(){
    for(int i = buffer.length()-1; i>=0; i--){
        if(buffer[i]=='+'||buffer[i]=='-'
                ||buffer[i]=='*'
                ||buffer[i]=='/')
            return i;
    }
    return 0;
}

void MainWindow::on_clear_pressed()
{
    buffer = "";
    ui->Field->setText(buffer);
}

void MainWindow::on_procent_clicked()
{
    if(buffer.length()<1||(buffer[0]=='-'&&buffer.length()==1))
        return;
    double num;
    std::string str;
    str = buffer.toUtf8().constData();
    if(buffer[buffer.length()-1]=='+'
            ||buffer[buffer.length()-1]=='-'
            ||buffer[buffer.length()-1]=='*'
            ||buffer[buffer.length()-1]=='/')
        buffer = buffer.left(buffer.length()-1);
    str = str.substr(getPositionOfOperation()+1, buffer.length()-getPositionOfOperation());
    num = atof(str.c_str());
    num*=0.01;
    str = buffer.toUtf8().constData();
    str.replace(getPositionOfOperation()+1, str.length(), std::to_string(num));
    buffer = QString::fromStdString(str);
    ui->Field->setText(buffer);
}
