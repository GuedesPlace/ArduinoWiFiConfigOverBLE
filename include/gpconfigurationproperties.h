#ifndef GpConfigurationProperties_h
#define GpConfigurationProperties_h
#include <WString.h>

typedef struct {
    String key;
    String label;
    String value;
} gpConfigProperty;

class GPConfigurationProperties
{
    public:
        GPConfigurationProperties(int capcity);
        int getCapacity();
        gpConfigProperty getProperty(int element);
        void setProperty(gpConfigProperty property, int element);
        ~GPConfigurationProperties();
    private:
        int m_capacity;
        gpConfigProperty* m_propertiesArray;
};
#endif