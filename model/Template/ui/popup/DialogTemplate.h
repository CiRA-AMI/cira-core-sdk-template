#ifndef DialogTemplate_H
#define DialogTemplate_H

#include <QDialog>
#include <QJsonObject>
#include <QEvent>

#include <iostream>

//#include <cira_std_lib/cira_std_lib.hpp>

namespace Ui {
class DialogTemplate;
}

class DialogTemplate : public QDialog
{
  Q_OBJECT

public:
  explicit DialogTemplate(QWidget *parent = 0);
  ~DialogTemplate();

  void changeEvent(QEvent *ev) {
    if (ev->type() == QEvent::ActivationChange)
    {
        if(this->isActiveWindow())
        {

        }
        else
        {
          if(!isPin) {
            this->close();
          }
        }
    }
  }

  bool isPin = true;

  QString style_unpin = "background-color: transparent; border-image: url(:/cira_bernoulli_opencv/icon/unpin.png); background: none; border: none; background-repeat: none;";
  QString style_pin = "background-color: transparent; border-image: url(:/cira_bernoulli_opencv/icon/pin.png); background: none; border: none; background-repeat: none;";

  QJsonObject saveState();
  void restoreState(QJsonObject param_js_data);

//private slots:
//  void getFlow(std::shared_ptr<FlowData> flow);
  
private:
  Ui::DialogTemplate *ui;
};

#endif // DialogTemplate_H
