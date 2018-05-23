#include "mergewidget.h"
#include <QApplication>

mergeWidget::mergeWidget(QWidget *parent)
    : QWidget(parent)
{
    //style sheet
    qssDefiniation();
    setWindowIcon(QIcon(":/images/merge.ico"));

    file1Button = new QPushButton(tr("bootloader"));
    file2Button = new QPushButton(tr("app")); //代开文件1和2按钮
    mergeButton = new QPushButton(tr("合并")); //合并按钮
    helpButton  = new QPushButton(tr("帮助")); //帮助按钮

    fp1LineEdit = new QLineEdit(this);
    fp2LineEdit = new QLineEdit(this);
    fp1LineEdit->setReadOnly(true);
    fp2LineEdit->setReadOnly(true);//路径为只读

    fp1Label    = new QLabel(tr("路径"));
    fp2Label    = new QLabel(tr("路径"));
    titleLabel  = new QLabel(tr("Hex文件合并工具"));
    linkLabel   = new QLabel(this);
    linkLabel->setOpenExternalLinks(true);
    linkLabel->setText(tr("<a href=\"http://www.openedv.com\">联系作者"));

    titleLabel->setStyleSheet(qssLabel);//设置标题字体方式

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);//固定大小

    mainLayout->addWidget(titleLabel,0,0,1,10,Qt::Alignment(Qt::AlignCenter));
    mainLayout->addWidget(fp1Label,1,0,Qt::AlignRight);//1行0列开始，占据1行1列
    mainLayout->addWidget(fp1LineEdit,1,1,1,5);//1行1列开始，占据1行2列
    mainLayout->addWidget(file1Button,1,6,1,2);//1行3列开始，占据1行1列

    mainLayout->addWidget(fp2Label,2,0,Qt::AlignRight);
    mainLayout->addWidget(fp2LineEdit,2,1,1,5);
    mainLayout->addWidget(file2Button,2,6,1,2);


    mainLayout->addWidget(mergeButton,3,3,1,1);
    mainLayout->addWidget(helpButton,3,5,1,1);
    mainLayout->addWidget(linkLabel,4,0,1,8,Qt::AlignCenter);

    mainLayout->setMargin(30);
    mainLayout->setSpacing(15); //设置控件间像素15pix
    setLayout(mainLayout);

    connect(file1Button,&QPushButton::clicked,this,&mergeWidget::file1Clicked);//file1button
    connect(file2Button,&QPushButton::clicked,this,&mergeWidget::file2Clicked);
    connect(helpButton,&QPushButton::clicked,this,&mergeWidget::helpClicked); //help
    connect(mergeButton,&QPushButton::clicked,this,&mergeWidget::mergeClicked);//merge

}
//deconstructor
mergeWidget::~mergeWidget()
{

}
void mergeWidget::qssDefiniation()
{
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
            QBrush(QPixmap(":/images/bg4.jpg").scaled(// 缩放背景图.
                this->size(),
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation)));
    this->setPalette(palette);
    qssPushButton="QPushButton{}";
    qssLabel="QLabel{font: 75 12pt 黑体;"
                    "}";
}

/*
**name:open a hex file
**
**retval: true-open success false-open fail
*/
bool mergeWidget::openHEXFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,tr("打开错误！"),
                             tr("无法读取文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd()){
         line1[number1++]= in.readLine();
    }
    QApplication::restoreOverrideCursor();
    return true;
}
//SLOTS
bool mergeWidget::file1Clicked() //打开文件1.hex，并将内容存放到缓冲区
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                      tr("打开hex文件"),"",tr("Text file (*.hex)"));

    if(!fileName.isEmpty())
    {
        fp1LineEdit->setText(fileName);
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this,tr("打开错误！"),
                                 tr("无法读取文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
            return false;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        while(!in.atEnd()){
             line1[number1++]= in.readLine();
        }
        QApplication::restoreOverrideCursor();
        return true;
    }
    return false;
}
bool mergeWidget::file2Clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                      tr("打开hex文件"),"",tr("Text file (*.hex)"));

    if(!fileName.isEmpty())
    {
        fp2LineEdit->setText(fileName);
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this,tr("打开错误！"),
                                 tr("无法读取文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
            return false;
        }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        while(!in.atEnd()){
             line2[number2++]= in.readLine();
        }
        QApplication::restoreOverrideCursor();
        return true;
    }
    return false;
}
//merge handle
void mergeWidget::mergeHandle()
{
    QString mergeFile[number1+number2-2];
    int i=0;
    for(i=0;i<number1-2;i++)
        mergeFile[i]=line1[i];
    for(int m=1;m<number2;m++)
        mergeFile[i+m-1]=line2[m]; //将两文件保存到开辟的缓存区

    QString fileName = QFileDialog::getSaveFileName(this,tr("保存文件"),tr(""),tr("Text file (*.hex)"));
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法写入文件 %1：/n %2")
                             .arg(fileName).arg(file.errorString()));
    }
    QTextStream out(&file);
    for(i=0;i<number1+number2-2;i++)
     out<<mergeFile[i]<<"\n";

    QMessageBox msgbox(this);
    msgbox.setText(tr("合并成功"));
    if(msgbox.exec()==QMessageBox::Ok)
    {
        number1 = 0; //成功保存后才会清除
        number2 = 0;
        fp1LineEdit->clear();
        fp2LineEdit->clear();
    }
}

bool mergeWidget::mergeClicked()
{
    if(number1>2&&number2>2)
    {
        if(number1==number2)
        {
            QMessageBox equalMsgBox(this);
            equalMsgBox.setText(tr("请确认打开文件是否正确?"));
            equalMsgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
            equalMsgBox.setDefaultButton(QMessageBox::Yes);
            int ret = equalMsgBox.exec();
            if(ret == QMessageBox::Yes){ //确认文件打开正确,不是同一文件
                mergeHandle();
            }else{
                number1 = 0; //重新打开文件
                number2 = 0;
                fp1LineEdit->clear();
                fp2LineEdit->clear();
                return false;
            }
        }
        else{
            mergeHandle();
        }
    }
    else{
        QMessageBox::warning(this,tr("警告"),tr("请打开两个要合并的文件！"));
        return false;
    }
    return true;
}

//help
void mergeWidget::helpClicked()
{
    QMessageBox msgBox(this);
    msgBox.setText(tr("如有需要，请联系作者。"));
    msgBox.resize(200,400);
    msgBox.exec();
}
