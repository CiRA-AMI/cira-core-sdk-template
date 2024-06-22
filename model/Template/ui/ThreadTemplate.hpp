#ifndef ThreadTemplate_HPP
#define ThreadTemplate_HPP

#include <ros/ros.h>
#include <cira_lib_bernoulli/general/GlobalData.hpp>
#include <QThread>
#include <QJsonObject>

#include <opencv2/opencv.hpp>

class ThreadTemplate : public QThread
{
  Q_OBJECT
public:

  QString name = "Template";

  QJsonObject payload_js_data;
  QJsonObject output_js_data;
  QJsonObject param_js_data;

  bool isUseImage = $$is_use_img$$;
  cv::Mat mat_im;

  bool isHaveError = false;

  ThreadTemplate() { }

  void run() {

    isHaveError = false;
    output_js_data = QJsonObject();
    QJsonObject jso;
    if(isUseImage) {
      if(mat_im.empty()) {
        jso["error"] = "No image";
        output_js_data[name] = jso;
        isHaveError = true;
        return;
      }
    }

    //********* run here **************


    jso["msg"] = "Hello World";


    payload_js_data[name] = jso;

  }


};

#endif // ThreadTemplate_HPP
