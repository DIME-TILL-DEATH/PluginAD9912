#ifndef WRITEFIELDSTRUCTURE_H
#define WRITEFIELDSTRUCTURE_H

#include <QVariant>

class WriteFieldStructure
{
public:
    WriteFieldStructure();
    WriteFieldStructure(QString registerName, QString fieldName, QVariant value);

    const QString &registerName() const;
    const QString &fieldName() const;
    const QVariant &value() const;

private:
    QString m_registerName;
    QString m_fieldName;
    QVariant m_value;
};

#endif // WRITEFIELDSTRUCTURE_H
