#include "PIGAI.h"
#include <QFile>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  PIGAI w;

  //   搞了个中文路径 完蛋玩意
  //   QFile qssFile("lightblue.css"); //资源文件":/css.qss"
  //   qssFile.open(QFile::ReadOnly);
  //   if (qssFile.isOpen()) {
  //     QString qss = QLatin1String(qssFile.readAll()); //读取
  //     qApp->setStyleSheet(qss);                       //设置
  //     qssFile.close();
  //   }

  w.show();
  return a.exec();
}
