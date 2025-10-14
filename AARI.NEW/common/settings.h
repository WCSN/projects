#ifndef SETTINGS_H
#define SETTINGS_H
#pragma once

#include <QSettings>

class Settings
{
public:
    Settings();
    ~Settings();

    static bool contains(const QString &key);
    static QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    static void setValue(const QString &key, const QVariant &value);
    static void clear(void);

private:
    static Settings *s_instance;
    QSettings m_settings;
};

#endif // SETTINGS_H
