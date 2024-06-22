#ifndef FormTemplate_H
#define FormTemplate_H

#include <QWidget>
#include <iostream>
#include <QMovie>
#include <QScreen>

#include "ThreadTemplate.hpp"
#include <cira_lib_bernoulli/general/GlobalData.hpp>
#include "popup/DialogTemplate.h"

namespace Ui {
class FormTemplate;
}

class FormTemplate : public QWidget
{
  Q_OBJECT

public:
  explicit FormTemplate(QWidget *parent = 0);
  ~FormTemplate();

  bool nodeStatus_enable = true;
  bool nodeStatus_ready = true;
  bool nodeStatus_complete = true;

  qint64 timestamp_base = 0;
  qint64 timestamp_input = -1;

  bool isHaveError = false;

  QString style_nodeDisable = "background-color: transparent; border-image: url(:/cira_bernoulli_opencv/icon/node_disable.png); background: none; border: none; background-repeat: none;";
  QString style_nodeEnable = "background-color: transparent; border-image: url(:/cira_bernoulli_opencv/icon/node_enable.png); background: none; border: none; background-repeat: none;";

  QMovie *mv_node_run;

  ThreadTemplate *workerThread;
  DialogTemplate *dialog;
  
  QString scene_namespace;

public slots:
  void update_ui();

private slots:
  void stop_node_process();
  void on_pushButton_nodeEnable_clicked();
  void on_pushButton_prop_clicked();

private:
  Ui::FormTemplate *ui;
};

#endif // FormTemplate_H
