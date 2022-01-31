#include "PIGAI.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPair>
#include <algorithm>
#include <qsettings.h>

using namespace std;
#pragma execution_character_set("utf-8")

PIGAI::PIGAI(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);
  setWindowTitle("llm作业统计");
  setWindowIcon(QIcon(":/PIGAI/akngx-8hzee-001.ico"));
  setAttribute(Qt::WA_QuitOnClose, true);

  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnBtOpenClicked()));
  connect(ui.m_cbSub, SIGNAL(currentIndexChanged(int)), this,
          SLOT(OnCbSubChanged(int)));
  connect(ui.m_cbOrder, SIGNAL(currentIndexChanged(int)), this,
          SLOT(OnCbOrderChanged(int)));
  connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(SpinChanged(int)));
}

void PIGAI::OnBtOpenClicked() {
  QSettings settings("./setting.ini", QSettings::IniFormat);
  settings.beginGroup("path");
  QString defaultPath = settings.value("txtPath").toString();

  filename = QFileDialog::getOpenFileName(nullptr, tr("输入结果文件"),
                                          defaultPath, tr("txt结果 (*.txt)"));
  if (!filename.isEmpty())
    ui.textEdit->setText(filename);
  else
    return;
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Truncate)) {
    QMessageBox::information(nullptr, tr("错误"),
                             QString("加载文件" + file.fileName() + "失败"));
    return;
  }
  all_name_score.clear();
  blank_name_score.clear();
  choice_name_score.clear();
  while (!file.atEnd()) {
    QString name;
    double allCore;
    double blankCore;
    double choiceCore;
    QByteArray line = file.readLine();
    QString str(line);

    str.replace('\t', ',');
    str.replace(' ', ',');
    QStringList list = str.split(',');
    if (list.size() == 9) {
      int aa = 0;
      name = list[2];
      allCore = list[5].left(list[5].size() - 1).toInt();
      choiceCore = list[6].left(list[6].size() - 1).toInt();
      blankCore = list[7].left(list[7].size() - 1).toInt();
      all_name_score.push_back(qMakePair(name, allCore));
      choice_name_score.push_back(qMakePair(name, choiceCore));
      blank_name_score.push_back(qMakePair(name, blankCore));
    }
  }
  ui.spinBox->setMaximum(all_name_score.size());
  QString titleTxt = "作业人数:";
  titleTxt += QString::number(all_name_score.size());
  setWindowTitle(titleTxt);
  qDebug() << "load all finished..." << endl;
  int order = ui.m_cbOrder->currentIndex();
  int sub = ui.m_cbSub->currentIndex();
  sortByOrderAndSub(order, sub);

  defaultPath = QFileInfo(filename).absoluteDir().absolutePath();
  settings.setValue("txtPath", defaultPath);
  settings.endGroup();
  settings.sync();
}

void PIGAI::showName(QVector<QString> s) {
  QString final_name;
  for (int i = 0; i < s.size(); i++) {
    final_name += s[i];
    final_name += "、";
  }
  final_name.chop(1);
  ui.textBrowser->setText(final_name);
}

void PIGAI::sortByOrderAndSub(int order, int sub) {
  if (all_name_score.empty() || blank_name_score.empty() ||
      choice_name_score.empty()) {
    QMessageBox::information(nullptr, tr("错误"), QString("加载文件失败"));
    return;
  }
  if (order == 0) { //好-》坏
    sort(all_name_score.begin(), all_name_score.end(),
         [](QPair<QString, double> a, QPair<QString, double> b) -> bool {
           return a.second > b.second;
         });
    sort(blank_name_score.begin(), blank_name_score.end(),
         [](QPair<QString, double> a, QPair<QString, double> b) -> bool {
           return a.second > b.second;
         });
    sort(choice_name_score.begin(), choice_name_score.end(),
         [](QPair<QString, double> a, QPair<QString, double> b) -> bool {
           return a.second > b.second;
         });
  } else {
    sort(all_name_score.begin(), all_name_score.end(),
         [](QPair<QString, double> a, QPair<QString, double> b) -> bool {
           return a.second < b.second;
         });
    sort(blank_name_score.begin(), blank_name_score.end(),
         [](QPair<QString, double> a, QPair<QString, double> b) -> bool {
           return a.second < b.second;
         });
    sort(choice_name_score.begin(), choice_name_score.end(),
         [](QPair<QString, double> a, QPair<QString, double> b) -> bool {
           return a.second < b.second;
         });
  }

  finalNameList.clear();
  int size = ui.spinBox->value();
  switch (sub) {
  case 0:
    for (int i = 0; i < size; i++) {
      finalNameList.push_back(all_name_score[i].first);
    }
    showName(finalNameList);
    break;
  case 1:
    for (int i = 0; i < size; i++) {
      finalNameList.push_back(choice_name_score[i].first);
    }
    showName(finalNameList);
    break;
  case 2:
    for (int i = 0; i < size; i++) {
      finalNameList.push_back(blank_name_score[i].first);
    }
    showName(finalNameList);
    break;
  default:
    break;
  }
}

void PIGAI::OnCbOrderChanged(int k) {
  int sub = ui.m_cbSub->currentIndex();
  sortByOrderAndSub(k, sub);
}

void PIGAI::OnCbSubChanged(int k) {
  int order = ui.m_cbOrder->currentIndex();
  sortByOrderAndSub(order, k);
}

void PIGAI::SpinChanged(int k) {
  int order = ui.m_cbOrder->currentIndex();
  int sub = ui.m_cbSub->currentIndex();
  sortByOrderAndSub(order, sub);
}
