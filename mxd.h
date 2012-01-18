#include<sys/file.h>
#include<sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "netsetup.h"
class myclass: public QWidget
{
        Q_OBJECT
 public:
        myclass();
        ~myclass();
        netsetui *netui;
        void useargv(char *uarg);
	int rx_bytes,tx_bytes;
        QStringList pppoe;
        QStringList papline;
        QStringList _dns;
        QString eth;
private:
	void createActions();
        void createTrayIcon();
        QLineEdit *lee1;
	QLineEdit *lee2;
	QLineEdit *lee3;	
	QPushButton *b1;
	QPushButton *savebutton;
	QSystemTrayIcon *trayIcon;
        char *msg;
	QLabel *label1;
	QLabel *label2;
	QLabel *label3;
        QComboBox *ncard;
	QCheckBox *dnscheck;
        QAction *minimizeAction;
        QAction *conn1;
        QAction *conn2;
        QAction *quitAction;
        QAction *netsetup;
        QMenu *trayIconMenu;
	uid_t uid;
	QString x1,x2;
	QTimer timer;
        int nostart;
protected:
        void closeEvent(QCloseEvent *event);
private slots:
	void pppoeStart();
        void pppoeStop();
        void Netset();
        void linkStart();
        void showM();
	void saveconf();
        void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void updateIcon();
        void hideme();
}; 
