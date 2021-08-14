#include "mainwindow.h"

#include <QApplication>
#ifdef defined(Q_OS_ANDROID)
#include <QtAndroidExtras/QtAndroid>
bool checkPermission() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.WRITE_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
             return false;
        }
   }
   return true;
}
#endif
int main(int argc, char *argv[])
{
    //checkPermission();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
