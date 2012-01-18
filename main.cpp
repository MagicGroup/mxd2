#include "mxd.h"
int main(int argc,char **argv)
{
        QApplication testc(argc,argv);
        QTranslator translator;
        translator.load("/opt/mxd/mxd_cn");
        testc.installTranslator(&translator);
        myclass newc;
        newc.show();
        newc.useargv(argv[1]);
        return testc.exec();
}

