#include "sinetab.h"

SineTab::SineTab(QWidget *parent)
    : QWidget(parent),
      mainGrid{new QGridLayout()}
{
    this->setLayout(mainGrid);

    quint16 rowNum = 0;

    mainGrid->addWidget(&clkFrqEdit, rowNum, 1);
    mainGrid->addWidget(new QLabel(tr("Тактовая частота")), rowNum++, 0);

    QLabel* modeLabel = new QLabel{tr("Режим:")};
    modeLabel->setFixedHeight(20);
    mainGrid->addWidget(modeLabel, rowNum++, 0, 1, 2, Qt::AlignHCenter);

    mainGrid->addWidget(&modeDivider, rowNum, 0);
    mainGrid->addWidget(&divEdit, rowNum++, 1);

    mainGrid->addWidget(&modeFreq, rowNum, 0);
    mainGrid->addWidget(&outFrqEdit, rowNum++, 1);

    mainGrid->addWidget(new QLabel(tr("Коэффициент деления")), rowNum++, 0);

    clkFrqEdit.setText("800e6");
    divEdit.setText("4");
    outFrqEdit.setText("200e6");
    modeDivider.setChecked(true);

    mainGrid->addWidget(&btnSetReg, rowNum, 0);
    mainGrid->addWidget(&checkDeviceWrite, rowNum++, 1);
    checkDeviceWrite.setChecked(true);

    mainGrid->addWidget(&listView, rowNum++, 0, 1, 2, Qt::AlignCenter);

    connect(&btnSetReg, &QPushButton::clicked, this, &SineTab::btnSetFields);
}

QMap<QString, QString> SineTab::getSaveData()
{
    QMap<QString, QString> dataToSave;

    dataToSave["Синус_частота"] = clkFrqEdit.text();
    dataToSave["Синус_деление"] = divEdit.text();
    dataToSave["Синус_выходная_частота"] = outFrqEdit.text();

    return dataToSave;
}

void SineTab::setLoadData(const QMap<QString, QString>& dataToLoad)
{
    QString loadedData;


    clkFrqEdit.setText(dataToLoad["Синус_частота"]);
    divEdit.setText(dataToLoad["Синус_деление"]);

    loadedData = dataToLoad["Синус_выходная_частота"];
    if(loadedData != "") outFrqEdit.setText(loadedData);
}

void SineTab::btnSetFields()
{
    listView.clear();

    QStringList list;
    list << QDateTime::currentDateTime().toString();
    list << "";

    bool convResult;
    double clkFrq = clkFrqEdit.text().toDouble(&convResult);

    if(!convResult)
    {
        list << "Частота тактирования задана неверно";
        listView.addItems(list);
        return;
    }

    double divider = divEdit.text().toDouble(&convResult);

    if(!convResult)
    {
        list << "Делитель задан неверно";
        listView.addItems(list);
        return;
    }

    double resultFrequency;
    if(modeDivider.isChecked())
    {
        resultFrequency = clkFrq/divider;
        outFrqEdit.setText(QString::number(resultFrequency, 'g', 8));
    }
    else
    {
        resultFrequency = outFrqEdit.text().toDouble(&convResult);
        if(!convResult)
        {
            list << "Выходная частота задана неверно";
            listView.addItems(list);
            return;
        }
        divider = clkFrq/resultFrequency;
        divEdit.setText(QString::number(divider, 'g', 8));
    }
    list << "Результирующая частота: " + QString().setNum(resultFrequency) + " Гц";

    qulonglong controlWord = (resultFrequency/clkFrq) * pow(2, 48);

    list << "Управляющее слово: 0x" + QString().setNum(controlWord, 16);
    list << "";

    std::vector<WriteFieldStructure> dataToSend;

    dataToSend.push_back({"FTW0", "Frequency Tuning Word", controlWord&0xFFFFFFFFFFFF});
    dataToSend.push_back({"Register update", "Update Mirrored regs", 0x1});

    list << "Установлено в регистр FTW0";


    listView.addItems(list);
    listView.scrollToBottom();

    emit setSineFields(dataToSend, checkDeviceWrite.checkState());
}
