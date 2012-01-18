#ifndef NETSETUP_H
#define NETSETUP_H

#endif // NETSETUP_H
#include <QtGui>
class netsetui: public QWidget
{
     Q_OBJECT
public:
        netsetui();
        ~netsetui();
        QLabel *label1;
        QLabel *label2;
        QLabel *label3;
        QLabel *label4;
        QLabel *label5;
        QLabel *label6;
        QLineEdit *edtDNS1;
        QLineEdit *edtDNS2;
        QLineEdit *edtGateWay;        
        QLineEdit *edtIP;
        QLineEdit *edtName;
        QLineEdit *edtMask;
        QPushButton *refreshb;
        QPushButton *saveb;        
protected:
        void closeEvent(QCloseEvent *event);
private:
        QString eth;
public slots:
        void netconf();
        void savenet();
    };
