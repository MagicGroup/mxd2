#include "netsetup.h"
netsetui::netsetui()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    QFont wqyFont("WenQuanYi Bitmap Song", 11);
    this->setFont(wqyFont);
    setMinimumSize(500,170);
    setMaximumSize(500,170);
    setWindowTitle(tr("netsetup"));
    QIcon icon = QIcon("/opt/mxd/images/icon_pci.png");
    setWindowIcon(icon);
    this->move(400,200);
    label1=new QLabel(tr("Network IP :"),this);
    label1->setGeometry(15,15,100,22);
    label2=new QLabel(tr("Gateway :"),this);
    label2->setGeometry(15,55,100,22);
    label3=new QLabel(tr("DNS1:"),this);
    label3->setGeometry(15,95,100,22);
    label4=new QLabel(tr("Mask :"),this);
    label4->setGeometry(260,15,100,22);
    label5=new QLabel(tr("Hostname :"),this);
    label5->setGeometry(260,55,100,22);
    label6=new QLabel(tr("DNS2:"),this);
    label6->setGeometry(260,95,100,22);
    edtIP=new QLineEdit(this);
    edtIP->setContextMenuPolicy(Qt::NoContextMenu);
    edtIP->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    edtIP->setGeometry(110,15,132,22);
    edtName=new QLineEdit(this);
    edtName->setContextMenuPolicy(Qt::NoContextMenu);
    edtName->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    edtName->setGeometry(110,55,132,22);
    edtDNS1=new QLineEdit(this);
    edtDNS1->setContextMenuPolicy(Qt::NoContextMenu);
    edtDNS1->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    edtDNS1->setGeometry(110,95,132,22);
    edtMask=new QLineEdit(this);
    edtMask->setContextMenuPolicy(Qt::NoContextMenu);
    edtMask->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    edtMask->setGeometry(355,15,132,22);
    edtName=new QLineEdit(this);
    edtName->setContextMenuPolicy(Qt::NoContextMenu);
    edtName->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    edtName->setGeometry(355,55,132,22);
    edtDNS2=new QLineEdit(this);
    edtDNS2->setContextMenuPolicy(Qt::NoContextMenu);
    edtDNS2->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    edtDNS2->setGeometry(355,95,132,22);
    refreshb=new QPushButton(tr("refresh"),this);
    refreshb->setGeometry(330,135,70,25);
    saveb=new QPushButton(tr("save"),this);
    saveb->setGeometry(415,135,70,25);

    connect(refreshb, SIGNAL(clicked()), this, SLOT(netconf()));
    connect(saveb, SIGNAL(clicked()), this, SLOT(savenet()));
  }

netsetui::~netsetui()
{

}

void netsetui::netconf()
{
QStringList lines;
QFile filenetwork( "/etc/sysconfig/network" );
   if ( filenetwork.open(QIODevice::ReadOnly)) {
       QTextStream stream( &filenetwork );
       QString line;
       while ( !stream.atEnd() ) {
           line = stream.readLine();
           if(line.startsWith("HOSTNAME=")){
               edtName->setText(line.right(line.length()-9));
           }else if(line.startsWith("GATEWAY=")){
               edtGateWay->setText(line.right(line.length()-8));
           }
           lines += line;
       }
       filenetwork.close();
   }
   //dns
   QFile fileresolv( "/etc/resolv.conf" );
   if ( fileresolv.open(QIODevice::ReadOnly)) {
       QTextStream stream( &fileresolv );
       QString line;
       int n = 1;
       while ( !stream.atEnd() ) {
           line = stream.readLine();
           if(line.startsWith("nameserver")){
               if(1 == n){
                   edtDNS1->setText(line.right(line.length()-11));
                   n++;
               }else if(2 == n){
                   edtDNS2->setText(line.right(line.length()-11));
                   n++;
               }else{

               }
           }
           lines += line;
       }
       fileresolv.close();
   }

    //ip netmask
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
   QFile fileifcfg( "/etc/sysconfig/network-scripts/ifcfg-"+eth);
   if ( fileifcfg.open(QIODevice::ReadOnly)) {
       QTextStream stream( &fileifcfg );
       QString line;
       while ( !stream.atEnd() ) {
           line = stream.readLine();
          if(line.startsWith("IPADDR=")){
               edtIP->setText(line.right(line.length()-7));
           }else if(line.startsWith("NETMASK=")){
               edtMask->setText(line.right(line.length()-8));
           }
           lines += line;
       }
       fileifcfg.close();
   }


}

void netsetui::savenet()
{
    QStringList lines;
    QFile filenetwork( "/etc/sysconfig/network" );
        if ( filenetwork.open(QIODevice::ReadOnly)) {
            QTextStream stream( &filenetwork );
            QString line;
            while ( !stream.atEnd() ) {
                line = stream.readLine();
                if(line.startsWith("HOSTNAME=")){
                    line = "HOSTNAME=" +edtName->text();
                }else if(line.startsWith("GATEWAY=")){
                    line = "GATEWAY=" +edtGateWay->text();
                }
                lines += line;
            }
            filenetwork.close();
        }

        if(filenetwork.open(QIODevice::WriteOnly)){
            QTextStream stream( &filenetwork );
            for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
                stream << *it << "\n";
            filenetwork.close();
        }

        lines.clear();
        QFile filehosts( "/etc/hosts" );
        if ( filehosts.open(QIODevice::ReadOnly)) {
            QTextStream stream( &filehosts );
            QString line;
            while ( !stream.atEnd() ) {
                line = stream.readLine();
                if(line.startsWith("127.0.0.1")){
                    line = "127.0.0.1   " +edtName->text() + " localhost.localdomain localhost ";
                }else {

                }
                lines += line;
            }
            filehosts.close();
        }

        if(filehosts.open(QIODevice::WriteOnly)){
            QTextStream stream( &filehosts );
            for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
                stream << *it << "\n";
            filehosts.close();
        }

        lines.clear();



        //dns
        QFile fileresolv( "/etc/ppp/resolv.conf" );
        if ( fileresolv.open(QIODevice::ReadOnly)) {
            QTextStream stream( &fileresolv );
            QString line;
            int n = 1;
            while ( !stream.atEnd() ) {
                line = stream.readLine();
                if(line.startsWith("nameserver")){
                    if(1 == n){
                        line = "nameserver " +edtDNS1->text();
                        n++;
                    }else if(2 == n){
                        line = "nameserver " +edtDNS2->text();
                        n++;
                    }else{

                    }
                }
                lines += line;
            }
            fileresolv.close();
        }
        if(fileresolv.open(QIODevice::WriteOnly)){
            QTextStream stream( &fileresolv );
            for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
                stream << *it << "\n";
            fileresolv.close();
        }
        lines.clear();

         //ip netmask
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
        QFile fileifcfg( "/etc/sysconfig/network-scripts/ifcfg-"+eth);
        if ( fileifcfg.open(QIODevice::ReadOnly)) {
            QTextStream stream( &fileifcfg );
            QString line;
            while ( !stream.atEnd() ) {
               line = stream.readLine();
               if(line.startsWith("IPADDR=")){
                    line = "IPADDR=" +edtIP->text();
                }else if(line.startsWith("NETMASK=")){
                    line = "NETMASK=" +edtMask->text();
                }
                lines += line;
            }
            fileifcfg.close();
        }
        if(fileifcfg.open(QIODevice::WriteOnly)){
            QTextStream stream( &fileifcfg );
            for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
                stream << *it << "\n";
            fileifcfg.close();
        }
        lines.clear();  
}
void netsetui::closeEvent(QCloseEvent *event)
{
        this->hide();
        event->ignore();
}

