#include "mxd.h"
myclass::myclass()
{   char buf[10];
    FILE *pp;
    if((pp=popen("ps -A|awk '{print $4}'|grep ^mxd$|wc -l","r"))!=NULL)
    {
        while(fgets(buf,sizeof(buf),pp))
              {
                this->nostart=atoi(buf);
            }}
    if(nostart>1)
    {
        int fd;
        fd=open("/var/run/mxd_status.pid", O_WRONLY|O_CREAT);
        if(flock(fd,LOCK_EX)==0)
            {
               exit(0);
            }
    }
    QFile filestatus("/var/run/mxd_status.pid");
    if(filestatus.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&filestatus);
        stream<<"running"<<"\n";
        filestatus.close();
    }
       uid=getuid();
       netui=new netsetui();
       QFile filepap("/etc/ppp/pap-secrets");
        if(filepap.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&filepap);
            QString line;
            while(!stream.atEnd())
            {
                line=stream.readLine();
                if(line.startsWith("mxd_connect"))
                    {
                        line.remove("\t");
                        papline=line.split("*");
                     }               
              }            
            filepap.close();
        }
        else
            {
                   qWarning("Can't find the file /etc/ppp/pap-secrets");
                   papline.append("mxd_connect");
                   papline.append("");
                   papline.append("");
             }
    if(papline.size()<1)
     {
        qWarning("Some thing is wrong in /etc/ppp/pap-secrets");
        papline.append("mxd_connect");
        papline.append("");
        papline.append("");
     }
         QFile resolv("/etc/resolv.conf");
         if(resolv.open(QIODevice::ReadOnly))
         {
             QTextStream stream(&resolv);
             QString line;
             while(!stream.atEnd())
             {
                 line=stream.readLine();
                 if(line.startsWith("nameserver"))
                 {
                    _dns.append(line.remove("nameserver "));
                 }             
             }
             resolv.close();
         }else
         {
             qWarning("Can't find the file /etc/ppp/resolv.conf");
             _dns.append("");
         }
         if(_dns.size()<1)
         {
             qWarning("Some thing is wrong in /etc/ppp/resolv.conf");
             _dns.append("");
         }
         QFile feth("/etc/ppp/pppoe.conf");
         if(feth.open(QIODevice::ReadOnly))
         {
             QTextStream stream(&feth);
             QString line;
             while(!stream.atEnd())
             {
                 line=stream.readLine();
                 if(line.startsWith("ETH="))
                 {
                    eth=line.remove("ETH=");
                 }
             }
             feth.close();
         }

        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
        QFont wqyFont("WenQuanYi Bitmap Song",11);
        this->setFont(wqyFont);
        setMinimumSize(260,140);
	setMaximumSize(260,140);
	this->move(400,200);
        label1=new QLabel(tr("Name :"),this);
        label1->setGeometry(15,15,80,22);
	label2=new QLabel(this);
        label2->setText(tr("Passwd :"));
        label2->setGeometry(15,47,80,22);
        lee1=new QLineEdit(this);
	lee1->setContextMenuPolicy(Qt::NoContextMenu);
	lee1->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        lee1->setGeometry(70,15,180,22);
        lee1->setText(papline.at(1));
	lee2=new QLineEdit(this);
	lee2->setContextMenuPolicy(Qt::NoContextMenu);
	lee2->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        lee2->setGeometry(70,47,180,22);
	lee3=new QLineEdit(this);
	lee3->setContextMenuPolicy(Qt::NoContextMenu);
	lee3->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	lee3->setEnabled(false);
        lee3->setGeometry(70,79,180,22);
        dnscheck=new QCheckBox(tr("DNS"),this);
        dnscheck->setGeometry(15,79,80,22);
	dnscheck->setChecked(false);
        lee2->setText(papline.at(2));
	lee2->setEchoMode(QLineEdit::Password);
        lee3->setText(_dns.at(0));
        ncard=new QComboBox(this);
        ncard->setGeometry(15,107,80,22);
        ncard->addItem(eth);
        ncard->addItem("eth0");
        ncard->addItem("eth1");
        ncard->addItem("eth2");        
        b1=new QPushButton(tr("Dialup"),this);
        b1->setGeometry(155,107,40,25);
        savebutton=new QPushButton(tr("Save"),this);
        savebutton->setGeometry(205,107,40,25);
	QIcon icon = QIcon("/opt/mxd/images/icon_pci.png");
	setWindowIcon(icon);
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(icon);
        trayIcon->setToolTip(tr("Dialer"));
	createActions();
	createTrayIcon();
        trayIcon->show();
        setWindowTitle(tr("Dialer"));
connect(dnscheck,SIGNAL(clicked(bool)),lee3,SLOT(setEnabled(bool)));
connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
connect(b1, SIGNAL(clicked()), this, SLOT(linkStart()));
connect(savebutton, SIGNAL(clicked()), this, SLOT(saveconf()));
QTimer *timer = new QTimer(this);
connect(timer, SIGNAL(timeout()), this, SLOT(updateIcon()));
timer->start(700);
}
void myclass::showM()
{
	if(access("/etc/ppp/rp-pppoe-gui/conf.mxd_connect",0)==0)
	{
        QString titlec=tr("waiting...");
        QString textc=tr("PPPoE Connected");
        trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);
	this->hide();
         }
	else
	{
        QString titlec=tr("save config");
        QString textc=tr("please save conf by root");
        trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);
        this->saveconf();
	}
}
               
void myclass::saveconf()
{
QString titlec,textc;
if(uid==0)
{
        titlec=tr("waiting...");
        textc=tr("save account");
        //setup 1
        QFile filepap("/etc/ppp/mxd-secrets");
        if(filepap.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&filepap);
            QStringList lines;
            QString line;
            line="mxd_connect\t*\t";
            line+=lee1->text();
            line+="\t*\t";
            line+=lee2->text();
            lines.append(line);
            line=lee1->text();
            line+="\t*\t";
            line+=lee2->text();
            lines.append(line);
            for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
            stream << *it << "\n";
            filepap.close();
            lines.clear();
            line.clear();
        }
        unlink("/etc/ppp/pap-secrets");
        unlink("/etc/ppp/chap-secrets");
        system("cp /etc/ppp/mxd-secrets /etc/ppp/pap-secrets");
        system("cp /etc/ppp/mxd-secrets /etc/ppp/chap-secrets");
        
//setup 2

if(access("/etc/ppp/pppoe.conf",0)!=0)
       	 {
        printf("Can't find the file /etc/ppp/pppoe.conf\n");
         }

QFile pppoe("/etc/ppp/pppoe.conf");
if(pppoe.open(QIODevice::WriteOnly))
{
    QTextStream stream(&pppoe);
    QStringList lines;
    QString line;
    line="ETH="+ncard->currentText();
    lines.append(line);
    line="USER="+lee1->text();
    lines.append(line);
    lines.append("DEMAND=no");
    if (dnscheck->checkState()==2)
    {
        lines.append("DNSTYPE=SPECIFY");
        lines.append("PEERDNS=no");
        line="DNS1="+lee3->text();
        lines.append(line);
        line="DNS2="+_dns.at(0);
        lines.append(line);
    }
    else
    {
        lines.append("DNSTYPE=SERVER");
        lines.append("PEERDNS=no");
        lines.append("DNS1=");
        lines.append("DNS2=");
    }
    lines.append("DEFAULTROUTE=yes");
    lines.append("CONNECT_TIMEOUT=30");
    lines.append("CONNECT_POLL=2");
    lines.append("ACNAME=");
    lines.append("SERVICENAME=");
    lines.append("FORCEPING=\".\"");
    lines.append("CF_BASE=`basename $CONFIG`");
    lines.append("PIDFILE=\"/var/run/$CF_BASE-pppoe.pid\"");
    lines.append("SYNCHRONOUS=no");
    lines.append("CLAMPMSS=1412");
    lines.append("LCP_INTERVAL=20");
    lines.append("LCP_FAILURE=3");
    lines.append("PPPOE_TIMEOUT=80");
    lines.append("FIREWALL=NONE");
    lines.append("LINUX_PLUGIN=");
    lines.append("PPPOE_EXTRA=\"\"");
    lines.append("PPPD_EXTRA=\"\"");
    for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
    stream << *it << "\n";
    pppoe.close();
    lines.clear();
    line.clear();
}
//setup 3

if (dnscheck->checkState()==2)
{
QFile dns("/etc/ppp/resolv.conf");
if(dns.open(QIODevice::WriteOnly))
{
    QTextStream stream(&dns);
    QStringList lines;
    QString line;
    line="nameserver "+lee3->text();
    lines.append(line);
    line="nameserver "+_dns.at(0);
    lines.append(line);
    for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
    stream << *it << "\n";
    dns.close();
    lines.clear();
    line.clear();
}

}
}
else
    {
    titlec=tr("wrong operate");
    textc=tr("Please save by root");
     }

trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);
}
               
void myclass::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
     switch (reason) {
        case QSystemTrayIcon::DoubleClick:
if(this->isHidden())
{	
this->showNormal();
}else{this->hideme();}
         break;
     default:
         ;
     }
}

void myclass::createActions()
{
     minimizeAction = new QAction(tr("Hidden (&I)"), this);
     minimizeAction->setIcon(QIcon("/opt/kde4/share/icons/default.kde4/32x32/status/task-recurring.png"));
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hideme()));

     conn1 = new QAction(tr("Dial up (&X)"), this);
     conn1->setIcon(QIcon("/opt/kde4/share/icons/default.kde4/32x32/apps/internet-telephony.png"));
     connect(conn1, SIGNAL(triggered()), this, SLOT(pppoeStart()));

     conn2 = new QAction(tr("Cut off (&R)"), this); 
     conn2->setIcon(QIcon("/opt/kde4/share/icons/default.kde4/32x32/apps/xorg.png"));
     connect(conn2, SIGNAL(triggered()), this, SLOT(pppoeStop()));

     netsetup=new QAction(tr("Net setup(&S)"),this);
     netsetup->setIcon(QIcon("/opt/kde4/share/icons/default.kde4/32x32/devices/audio-card.png"));
     connect(netsetup,SIGNAL(triggered()),this,SLOT(Netset()));

     quitAction = new QAction(tr("Exit (&Q)"), this);
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
     quitAction->setIcon(QIcon("/opt/kde4/share/icons/default.kde4/32x32/status/dialog-error.png"));
}

void myclass::createTrayIcon()
{
     trayIconMenu = new QMenu(this);
     trayIconMenu->addAction(minimizeAction);
     trayIconMenu->addAction(conn1);
     trayIconMenu->addAction(conn2);
     trayIconMenu->addAction(netsetup);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(quitAction);
     trayIcon->setContextMenu(trayIconMenu);
}

void myclass::pppoeStart()
{
  if(access("/sys/class/net/ppp0/statistics/rx_bytes",0)!=0)
     {
        system("/usr/bin/mxd2c 1");
	this->hide();
	   }
   if(access("/sys/class/net/ppp0/statistics/rx_bytes",0)==0)
       {
            showM();
                }
}
void myclass::pppoeStop()
{
 system("/usr/bin/mxd2c 2");
}

void myclass::Netset()
{
    netui->netconf();
    netui->show();
}

void myclass::linkStart()
{
    if(access("/sys/class/net/ppp0/statistics/rx_bytes",0)!=0)
    {
        pppoeStart();
                }
    else
        pppoeStop();
}
void myclass::updateIcon()
{	
	if(access("/sys/class/net/ppp0/statistics/rx_bytes",0)==0)
       	 {	
	    int r,t;
	    FILE *in;
	    char block[128];		
		in=fopen("/sys/class/net/ppp0/statistics/rx_bytes","r");
		fgets(block,128,in);
		r=atoi(block);
		fclose(in);
		in=fopen("/sys/class/net/ppp0/statistics/tx_bytes","r");
		fgets(block,128,in);
		t=atoi(block);
		fclose(in);
		if(rx_bytes==r && tx_bytes==t)
			{
			QIcon  icon  =  QIcon("/opt/mxd/images/status_none.png");        
            			this->trayIcon->setIcon(icon);
				}
		if(rx_bytes<r && tx_bytes<t)
			{
			QIcon  icon  =  QIcon("/opt/mxd/images/status_both.png");        
         			this->trayIcon->setIcon(icon);
					}
		if(rx_bytes<r && tx_bytes==t)
			{
			QIcon  icon  =  QIcon("/opt/mxd/images/status_rx.png");        
         			this->trayIcon->setIcon(icon);
					}
		if(rx_bytes==r && tx_bytes<t)
			{
			QIcon  icon  =  QIcon("/opt/mxd/images/status_tx.png");        
         			this->trayIcon->setIcon(icon);
					}
		rx_bytes=r;
		tx_bytes=t;
                b1->setText(tr("Cutoff"));
		}
		else
		{QIcon  icon  =  QIcon("/opt/mxd/images/status_error.png");        
                this->trayIcon->setIcon(icon);
                b1->setText(tr("Dialup"));
            }
}

void myclass::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible())
    {

        this->hide();
        event->ignore();
    }

}

void myclass::hideme()
{
    this->hide();
    netui->hide();
}

myclass::~myclass()
{
    pppoeStop();
}
void myclass::useargv(char *uarg)
{
    if(qstrcmp(uarg,"up")==0)
        {
        this->pppoeStart();
        }
}
