#include "writefieldstructure.h"

WriteFieldStructure::WriteFieldStructure()
{

}

WriteFieldStructure::WriteFieldStructure(QString registerName, QString fieldName, QVariant value)
    : m_registerName{registerName},
      m_fieldName{fieldName},
      m_value{value}
{

}

const QString &WriteFieldStructure::registerName() const
{
    return m_registerName;
}

const QString &WriteFieldStructure::fieldName() const
{
    return m_fieldName;
}

const QVariant &WriteFieldStructure::value() const
{
    return m_value;
}
