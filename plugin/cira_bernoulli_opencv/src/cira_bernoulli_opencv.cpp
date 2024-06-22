#include <cira_bernoulli_opencv/cira_bernoulli_opencv.hpp>
#include <cira_lib_bernoulli/CiraBernoulliWidget.h>

cira_bernoulli_opencv::cira_bernoulli_opencv() {

}

std::shared_ptr<DataModelRegistry> cira_bernoulli_opencv::registerDataModels(std::shared_ptr<DataModelRegistry> ret) {


  /**********************
  regist model here
  example:

    ret->registerModel<Some1Model>();
    ret->registerModel<Some2Model>();

  ******************/

  setTreeWidget();
  return ret;

}

void cira_bernoulli_opencv::setTreeWidget() {

  QStringList strListNodeDataModels;
  QString category;


  /**********************
  regist model name here for drag&drop
  example:

    category = "SomeCategory";
    strListNodeDataModels << category + ",Some1"      + ",null";
    strListNodeDataModels << category + ",Some2"      + ",null";

    CiraBernoulliWidget::pluginTreeWidget->addNodeDataModels(strListNodeDataModels);

  ******************/

}
