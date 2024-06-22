#include "TemplateModel.hpp"
#include <cira_lib_bernoulli/datatype/FlowData.hpp>

#include <QtConcurrent/QtConcurrent>

TemplateModel::
TemplateModel()
{
  if(!GlobalData::isInitialized) return;

  form = new FormTemplate();
  _requireConfirmToDelete = false;

  if(GlobalData::isLoadingFlow) {
    timerLoadFromButton = new QTimer(this);
    connect(timerLoadFromButton, SIGNAL(timeout()), this, SLOT(loopTimerLoadFromButton()));
    timerLoadFromButton->start();
  }

  FlowDataType flowdata_;
  _flowDataOut.reset();
  _flowDataOut = std::make_shared<FlowData>(flowdata_);
  emit dataUpdated(0);

}

void TemplateModel::runProcess(std::shared_ptr<FlowData> _flowDataIn) {

  FlowDataType flowdata = _flowDataIn->getFlowData();

  form->workerThread->payload_js_data = flowdata._payload_js_data["payload"].toObject();
  if(form->workerThread->payload_js_data.isEmpty())
  {
    form->workerThread->payload_js_data = QJsonObject();
  }
  if(form->workerThread->isUseImage) {
    form->workerThread->mat_im.release();
    form->workerThread->mat_im = flowdata._img.clone();
  }
  form->workerThread->param_js_data = form->dialog->saveState();
  form->workerThread->run();

  form->isHaveError = form->workerThread->isHaveError;

  if(!GlobalData::isAllSceneCanRun) {
    form->isHaveError = true;
  }

  form->timestamp_base = GlobalData::timer.nsecsElapsed();

  QJsonObject jsdata;
  jsdata["datetime"] = GlobalData::datatime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
  if(GlobalData::isAllSceneCanRun && !form->workerThread->isHaveError) {
    jsdata["timestamp"] = QString::number(GlobalData::timer.nsecsElapsed());
  }
  jsdata["payload"] = form->workerThread->payload_js_data;
  flowdata._payload_js_data = jsdata;
  if(form->workerThread->isUseImage) {
    flowdata._img = form->workerThread->mat_im;
  }
  flowdata._output_js_data = form->workerThread->output_js_data;

  _flowDataOut = std::make_shared<FlowData>(flowdata);
  form->nodeStatus_complete = true;
  emit dataUpdated(0);

}


void TemplateModel::loopTimerLoadFromButton() {
  if(!GlobalData::isLoadingFlow) {
    timerLoadFromButton->stop();
    delete timerLoadFromButton;
    return;
  }

  FlowDataType flowdata_;
  flowdata_._payload_js_data["load_complete"] = true;
  _flowDataOut = std::make_shared<FlowData>(flowdata_);
  emit dataUpdated(0);
}


QJsonObject
TemplateModel::
save() const
{

  QJsonObject modelJson = NodeDataModel::save();
  modelJson["data"] = form->dialog->saveState();
  modelJson["node_enable"] = form->nodeStatus_enable;
  if(this->resizable()) {
    modelJson["width"] = form->width();
    modelJson["height"] = form->height();
  }
  return modelJson;
}


void
TemplateModel::
restore(QJsonObject const &p)
{

  if(!p["data"].isUndefined()) {
    form->dialog->restoreState(p["data"].toObject());
  }

  if(!p["node_enable"].isUndefined()) {
    form->nodeStatus_enable = p["node_enable"].toBool();
  }

  if(this->resizable()) {
    if(!p["width"].isUndefined() && !p["height"].isUndefined()) {
       form->setFixedSize(p["width"].toInt(), p["height"].toInt());
    }
  }

  form->update_ui();

}

unsigned int
TemplateModel::
nPorts(PortType portType) const
{
  unsigned int result = 1;

  switch (portType)
  {
    case PortType::In:
      result = 1;
      break;

    case PortType::Out:
      result = 1;

    default:
      break;
  }

  return result;
}

NodeDataType
TemplateModel::
dataType(PortType portType, PortIndex portIndex) const
{
  switch (portType)
  {
    case PortType::In:
      switch (portIndex)
      {
        case 0:
          return FlowData().type();
      }
      break;

    case PortType::Out:
      switch (portIndex)
      {
        case 0:
          return FlowData().type();
      }
      break;

    case PortType::None:
      break;
  }

}

void TemplateModel::setInData(std::shared_ptr<QtNodes::NodeData> data, PortIndex portIndex) {

  if(isBusy || !form->nodeStatus_complete) {
    form->timestamp_base = GlobalData::timer.nsecsElapsed();
    return;
  }
  isBusy = true;

  std::shared_ptr<FlowData> _flowDataIn = std::dynamic_pointer_cast<FlowData>(data);

  if(_flowDataIn) {
    if(_flowDataIn->getFlowData()._payload_js_data.contains("timestamp")) {
      form->timestamp_input = _flowDataIn->getFlowData()._payload_js_data["timestamp"].toString().toLongLong();
    }

    portInStatus[0]++;
    if(portInStatus[0] >= PORTSTATUS::CONNECTED) {
      portInStatus[0] = PORTSTATUS::CONNECTED;
    }
    else if (portInStatus[0] == PORTSTATUS::FIRSTCONNECT) {
      form->timestamp_base = GlobalData::timer.nsecsElapsed();
    }

  } else {
    //not connect
    portInStatus[0] = PORTSTATUS::DISCONNECTED;
    form->timestamp_base = GlobalData::timer.nsecsElapsed();
  }

  if(!GlobalData::isAllSceneCanRun) {
    form->timestamp_base = GlobalData::timer.nsecsElapsed();
  }

  if(GlobalData::isAllSceneCanRun && form->nodeStatus_enable
     && portInStatus[0] == PORTSTATUS::CONNECTED
     && form->timestamp_input >= form->timestamp_base
     && form->nodeStatus_complete && form->nodeStatus_ready) {

    form->nodeStatus_complete = false;
    form->isHaveError = false;
    form->update_ui();
    QtConcurrent::run(this, &TemplateModel::runProcess, _flowDataIn);

  }

  isBusy = false;

}

std::shared_ptr<NodeData>
TemplateModel::
outData(PortIndex port)
{
  form->update_ui();
  switch (port)
  {
    case 0:
      return _flowDataOut;;
  }
  return nullptr;
}
