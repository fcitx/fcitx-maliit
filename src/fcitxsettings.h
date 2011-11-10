
#include <mabstractinputmethodsettings.h>
#include <QObject>

class FcitxSettings: public QObject, public MAbstractInputMethodSettings
{
    Q_OBJECT
    Q_DISABLE_COPY(FcitxSettings)
public:
    FcitxSettings();
    
    //! \reimp
    virtual QString title();
    virtual QString icon();
    virtual QGraphicsWidget* createContentWidget(QGraphicsWidget* parent = 0);
    //! \reimp_end
    
};