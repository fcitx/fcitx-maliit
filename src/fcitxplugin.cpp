#include "fcitxplugin.h"
#include "fcitxhost.h"
#include "fcitxsettings.h"

FcitxPlugin::FcitxPlugin()
{

}

FcitxPlugin::~FcitxPlugin()
{

}

QString FcitxPlugin::name() const
{
    return "Fcitx";
}

QStringList FcitxPlugin::languages() const
{
    return QStringList("en");
}

MAbstractInputMethod* FcitxPlugin::createInputMethod(MAbstractInputMethodHost* host, QWidget* mainWindow)
{
    FcitxHost *inputMethod = FcitxHost::create(host, mainWindow);
    return inputMethod;
}

MAbstractInputMethodSettings* FcitxPlugin::createInputMethodSettings()
{
    MAbstractInputMethodSettings *inputMethodSettings = new FcitxSettings;
    return inputMethodSettings;
}

QSet< MInputMethod::HandlerState > FcitxPlugin::supportedStates() const
{
    QSet<MInputMethod::HandlerState> result;

    result << MInputMethod::OnScreen << MInputMethod::Hardware;
    return result;
}

Q_EXPORT_PLUGIN2(meego-fcitx, FcitxPlugin)