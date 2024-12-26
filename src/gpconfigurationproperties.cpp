#include "./GPConfigurationProperties.h"
GPConfigurationProperties::GPConfigurationProperties(int capacity)
{
    m_capacity = capacity;
    //m_propertiesArray = (gpConfigProperty *)malloc(capacity * sizeof(gpConfigProperty));
    m_propertiesArray = new gpConfigProperty[capacity];
}
int GPConfigurationProperties::getCapacity()
{
    return m_capacity;
}
gpConfigProperty GPConfigurationProperties::getProperty(int element)
{
    return m_propertiesArray[element];
}

void GPConfigurationProperties::setProperty(gpConfigProperty property, int element)
{
    m_propertiesArray[element] = property;
}

GPConfigurationProperties::~GPConfigurationProperties()
{
    free(m_propertiesArray);
}
