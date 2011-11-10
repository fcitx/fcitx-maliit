#ifndef MKEYBOARDPLUGIN_H
#define MKEYBOARDPLUGIN_H

#include <QObject>
#include <QWidget>

#include <minputmethodplugin.h>


/*!
 * \class FcitxPlugin
 *
 * \brief Implements plugin for Fcitx
 */
class FcitxPlugin: public QObject, public MInputMethodPlugin
{
    Q_OBJECT
    Q_INTERFACES(MInputMethodPlugin)

public:
    FcitxPlugin();
    virtual ~FcitxPlugin();

    //! \reimp
    virtual QString name() const;
    virtual QStringList languages() const;
    virtual MAbstractInputMethod *createInputMethod(MAbstractInputMethodHost *host,
                                                    QWidget *mainWindow);
    virtual MAbstractInputMethodSettings *createInputMethodSettings();
    virtual QSet<MInputMethod::HandlerState> supportedStates() const;
    //! \reimp_end
};

#endif