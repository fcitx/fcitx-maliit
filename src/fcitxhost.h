
#include <mabstractinputmethod.h>

class QGraphicsObject;
class QDeclarativeComponent;
class QDeclarativeEngine;
class MImGraphicsView;
class QGraphicsScene;
//! Logic class for virtual keyboard
class FcitxHost: public MAbstractInputMethod
{
    Q_OBJECT
public:

    FcitxHost(MAbstractInputMethodHost *host,
                  QWidget *mainWindow);
    virtual ~FcitxHost();

    //! Creates MKeyboardHost instance and init()'s it.
    static FcitxHost * create(MAbstractInputMethodHost *host,
                              QWidget *mainWindow);
    void init();
    

    //! reimp
    virtual void handleFocusChange(bool focusIn);
    virtual void show();
    virtual void hide();
    // Note: application call this setPreedit() to tell keyboard about the preedit information.
    // If keyboard wants to update preedit, it should not call the method but call the lower
    // localSetPreedit().
    virtual void setPreedit(const QString &preeditString, int cursorPosition);
    virtual void update();
    virtual void reset();
    virtual void handleMouseClickOnPreedit(const QPoint &mousePos, const QRect &preeditRect);
    virtual void handleVisualizationPriorityChange(bool priority);
    virtual void handleAppOrientationAboutToChange(int angle);
    virtual void handleAppOrientationChanged(int angle);
    virtual void setToolbar(QSharedPointer<const MToolbarData> toolbar);
    virtual void setState(const QSet<MInputMethod::HandlerState> &state);
    virtual void processKeyEvent(QEvent::Type keyType, Qt::Key keyCode,
                                 Qt::KeyboardModifiers modifiers,
                                 const QString &text, bool autoRepeat, int count,
                                 quint32 nativeScanCode, quint32 nativeModifiers, unsigned long time);
    virtual void handleClientChange();
    virtual void switchContext(MInputMethod::SwitchDirection direction, bool enableAnimation);
    virtual QList<MAbstractInputMethod::MInputMethodSubView> subViews(MInputMethod::HandlerState state
                                                                  = MInputMethod::OnScreen) const;
    virtual void setActiveSubView(const QString &subViewId,
                                  MInputMethod::HandlerState state = MInputMethod::OnScreen);
    virtual QString activeSubView(MInputMethod::HandlerState state = MInputMethod::OnScreen) const;
    virtual void showLanguageNotification();
    virtual void setKeyOverrides(const QMap<QString, QSharedPointer<MKeyOverride> > &newOverrides);
    //! reimp_end
    
    void resetVirtualKeyboardShiftState();
    void resetVirtualKeyboardLatchedShiftState();
    
private:
    QGraphicsScene* m_scene;
    MImGraphicsView* m_view;
    QDeclarativeEngine* m_engine;
    QDeclarativeComponent* m_component;
    QGraphicsObject* m_content;
};