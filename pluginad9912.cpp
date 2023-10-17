#include "pluginad9912.h"

PluginAD9912::PluginAD9912(QWidget *parent)
    : QMainWindow{parent}
{
    setCentralWidget(&tabWidget);

    setWindowFlags(Qt::WindowStaysOnTopHint);

    sineTab = new SineTab();
    tabWidget.addTab(sineTab, tr("Синус"));

    connect(sineTab, &SineTab::setSineFields, this, &PluginAD9912::setFields);
}

void PluginAD9912::showPanel()
{
    show();
}

void PluginAD9912::closePanel()
{
    close();
}

void PluginAD9912::setTargetDeviceNumber(quint16 number)
{
    m_pluginNumber = number;
}

quint16 PluginAD9912::targetDeviceNumber()
{
    return m_pluginNumber;
}

void PluginAD9912::setFields(std::vector<WriteFieldStructure> dataToSet, bool writeToDevice)
{
    QStringList registerNames;
    foreach(WriteFieldStructure fieldData, dataToSet)
    {
        registerNames << fieldData.registerName();
        emit setFieldValue(fieldData.registerName(), fieldData.fieldName(), fieldData.value());
    }

    if(writeToDevice)
    {
        registerNames.removeDuplicates();
        emit writeRegisterSequence(registerNames);
    }
}

void PluginAD9912::saveRequest(QString plugInName, QMap<QString, QString> &dataToSave)
{
    if(plugInName == m_name)
    {
        dataToSave.clear();
        dataToSave.insert(sineTab->getSaveData());
    }
}

void PluginAD9912::loadRequest(QString plugInName, const QMap<QString, QString> &dataToLoad)
{
    if(plugInName == m_name)
    {
        sineTab->setLoadData(dataToLoad);
    }
}
