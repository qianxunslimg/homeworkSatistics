#pragma once

#include "ui_PIGAI.h"
#include <QtWidgets/QWidget>

class PIGAI : public QWidget {
  Q_OBJECT

public:
  PIGAI(QWidget *parent = Q_NULLPTR);

private:
  Ui::PIGAIClass ui;
  QVector<QPair<QString, double>> all_name_score;
  QVector<QPair<QString, double>> blank_name_score;
  QVector<QPair<QString, double>> choice_name_score;
  QString filename;
  QVector<QString> finalNameList;
  void showName(QVector<QString>);

  void sortByOrderAndSub(int, int);
public slots:
  void OnBtOpenClicked();
  void OnCbOrderChanged(int k);
  void OnCbSubChanged(int k);
  void SpinChanged(int k);
};
