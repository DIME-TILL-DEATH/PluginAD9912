#ifndef PLUGINAD9912_H
#define PLUGINAD9912_H

#include <QtWidgets>
#include <QMainWindow>

#include <QtPlugin>
#include <QObject>
#include <QDebug>

#include "controlpanelinterface.h"

#include "sinetab.h"
#include "writefieldstructure.h"

class PluginAD9912 : public QMainWindow, public ControlPanelInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "com.vniirt.controlpanel")
    Q_INTERFACES(ControlPanelInterface)

public:
    explicit PluginAD9912(QWidget *parent = nullptr);
    ~PluginAD9912() {};

    void showPanel() override;
    void closePanel() override;

    void setTargetDeviceNumber(quint16 number) override;
    quint16 targetDeviceNumber() override;

signals:
    void setFieldValue(QString registerName, QString fieldName, QVariant value) override;
    void getFieldValue(QString registerName, QString fieldName, QVariant& value) override;
    void writeRegisterSequence(QStringList registerNames) override;
    void writeRegisterSequence() override;

public slots:

    void setFields(std::vector<WriteFieldStructure> dataToSet, bool writeToDevice);

    void saveRequest(QString plugInName, QMap<QString, QString>& dataToSave) override;
    void loadRequest(QString plugInName, const QMap<QString, QString>& dataToLoad) override;

private:
    QString m_name = "Контрольная панель AD9912";
    quint16 m_pluginNumber;

    QTabWidget tabWidget;

    SineTab* sineTab;
};

#endif // PLUGINAD9912_H
