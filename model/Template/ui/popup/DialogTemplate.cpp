#include "DialogTemplate.h"
#include "ui_DialogTemplate.h"

#include <cira_lib_bernoulli/general/GlobalData.hpp>
#include <cira_lib_bernoulli/datatype/FlowData.hpp>
#include <cira_lib_bernoulli/asm_opencv/asmOpenCV.h>

DialogTemplate::DialogTemplate(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogTemplate)
{
  ui->setupUi(this);
  
  //cira_std::connectFlowSlot("<flowName>", this, SLOT(getFlow(std::shared_ptr<FlowData>)));
  
}

DialogTemplate::~DialogTemplate()
{
  delete ui;
}

/*void DialogTemplate::getFlow(std::shared_ptr<FlowData> flow) {
  FlowDataType data = flow->getFlowData();
  QJsonObject payload = data._payload_js_data["payload"].toObject();
  cv::Mat mat_im = data._img;
  
}*/

QJsonObject DialogTemplate::saveState() {
  QJsonObject param_js_data;

  return param_js_data;
}

void DialogTemplate::restoreState(QJsonObject param_js_data) {

}
