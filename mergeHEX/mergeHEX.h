
#ifndef MERGEWIDGET_H
#define MERGEWIDGET_H

#include <QWidget>
#include <QString>
/*user's header files*/
#include <QPushButton> //widgets
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QToolButton>

#include <QMessageBox>
#include <QDialog>

#include <QFileDialog>
#include <QTextStream>

#include <QGridLayout> //layout
#include <QLayout>
#include <QStyle>
#include <QPalette>

class mergeWidget : public QWidget
{
    Q_OBJECT

public:
    mergeWidget(QWidget *parent = 0);
    ~mergeWidget();
private:
    //data
    int number1=0;
    int number2=0;
    QString line1[4096];  //boot文件总行数--150KB
    QString line2[8192];  //app文件总行数--300KB
    //如果文件过大，会导致程序崩溃！可根据需要调整此处大小，或者修改程序逻辑。
    
    //buttons
    QPushButton *file1Button;
    QPushButton *file2Button;
    QPushButton *mergeButton;
    QPushButton *helpButton;
    //labels
    QLabel *fp1Label;
    QLabel *fp2Label;
    QLabel *titleLabel;
    QLabel *linkLabel;
    //lineEdits
    QLineEdit *fp1LineEdit;
    QLineEdit *fp2LineEdit;
    //methods
    bool openHEXFile(const QString&fileName);
    void mergeHandle();
    //QSS
    QString qssPushButton;
    QString qssLineEdit;
    QString qssLabel;
    void qssDefiniation();
private slots:
    bool file1Clicked();
    bool file2Clicked();
    bool mergeClicked();
    void helpClicked();
};

#endif // MERGEWIDGET_H
