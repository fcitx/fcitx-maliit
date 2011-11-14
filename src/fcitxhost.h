
#include <mabstractinputmethod.h>
#include <QRect>
#include <QRegion>

#include "org.freedesktop.DBus.h"
#include "org.fcitx.Fcitx.InputMethod.h"
#include "org.fcitx.Fcitx.InputContext.h"
#include <fcitx-config/hotkey.h>

class QGraphicsObject;
class QDeclarativeComponent;
class QDeclarativeEngine;
class MImGraphicsView;
class QGraphicsScene;
//! Logic class for virtual keyboard
class FcitxHost: public MAbstractInputMethod
{
    Q_OBJECT
    Q_PROPERTY( int screenWidth READ screenWidth NOTIFY screenWidthChanged )
    Q_PROPERTY( int screenHeight READ screenHeight NOTIFY screenHeightChanged )
    Q_PROPERTY( QRect cursorRect READ cursorRect NOTIFY cursorRectChanged )
    Q_PROPERTY( QString appOrientation READ appOrientation NOTIFY appOrientationChanged )
public:

    FcitxHost(MAbstractInputMethodHost *host,
                  QWidget *mainWindow);
    virtual ~FcitxHost();

    //! Creates MKeyboardHost instance and init()'s it.
    static FcitxHost * create(MAbstractInputMethodHost *host,
                              QWidget *mainWindow);
    void createInputContext();
    

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
    
    int screenWidth();
    int screenHeight();
    QRect cursorRect();
    const QString& appOrientation();
    
    Q_INVOKABLE void setScreenRegion( const QRect &area ) ;
    Q_INVOKABLE void setInputMethodArea( const QRect &area ) ;
    Q_INVOKABLE void sendCommit( const QString& text ) ;
    Q_INVOKABLE void sendPreedit( const QString& text ) ;
signals:
    void screenWidthChanged( int width ) ;
    void screenHeightChanged( int height ) ;
    void cursorRectChanged( QRect& cursorRect ) ;
    void appOrientationChanged ( QString& appOrientation );
    
private slots:    
    void createInputContextFinished(QDBusPendingCallWatcher* watcher);
    void commitString(const QString& text);
    void closeIM();
    void enableIM();
    void updatePreedit(const QString& str, int cursorPos);
    void forwardKey(uint keyval, uint state, int type);
    void imChanged(const QString& service, const QString& oldowner, const QString& newowner);
    
private:
    QDBusConnection m_connection;
    QGraphicsScene* m_scene;
    MImGraphicsView* m_view;
    QDeclarativeEngine* m_engine;
    QDeclarativeComponent* m_component;
    QGraphicsObject* m_content;
    QRect m_cursorRect;
    QRegion m_inputMethodArea;
    QString m_appOrientation;
    QString m_serviceName;
    org::freedesktop::DBus* m_dbusproxy;
    org::fcitx::Fcitx::InputMethod* m_improxy;
    org::fcitx::Fcitx::InputContext* m_icproxy;
    int m_id;
    bool m_enable;
    QString m_path;
    HOTKEYS m_triggerKey[2];
};