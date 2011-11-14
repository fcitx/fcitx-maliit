#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QDebug>

#include <mabstractinputmethodhost.h>
#include <mimgraphicsview.h>

#include <fcitx/module/dbus/dbusstuff.h>
#include <fcitx/module/ipc/ipc.h>
#include <fcitx/frontend.h>

#include "config.h"
#include "fcitxhost.h"

#define QML_PATH PREFIX "/share/fcitx/maliit/qml/meego-keyboard.qml"

namespace {
    QRect defaultScreenRect(QWidget *w = 0)
    {
        if (not QApplication::desktop()) {
            return QRect();
        }
        
        if (not w) {
            return QApplication::desktop()->screenGeometry();
        }

        return QApplication::desktop()->screenGeometry(w);
    }
}

FcitxHost::FcitxHost(MAbstractInputMethodHost* host, QWidget* mainWindow)
    : MAbstractInputMethod(host, mainWindow),
    m_connection(QDBusConnection::sessionBus()),
    m_scene (new QGraphicsScene( defaultScreenRect(), this )),
    m_view( new MImGraphicsView( m_scene, mainWindow ) ) ,
    m_engine( new QDeclarativeEngine( this ) ),
    m_component(0),
    m_content( 0 ),
    m_appOrientation("0"),
    m_serviceName(QString("%1-%2").arg(FCITX_DBUS_SERVICE).arg(FcitxGetDisplayNumber())),
    m_improxy(new org::fcitx::Fcitx::InputMethod(m_serviceName, FCITX_IM_DBUS_PATH, m_connection, this))
{   
    m_engine->rootContext()->setContextProperty("fcitx", this);
    m_component = new QDeclarativeComponent( m_engine, QUrl( QML_PATH ) );
    m_content = qobject_cast<QGraphicsObject*>( m_component->create() );
    m_scene->addItem( m_content ) ;
    
    
    QWidget* viewport = m_view->viewport() ;

    if ( viewport->nativeParentWidget() )
        viewport = viewport->nativeParentWidget() ;

    const QRect& rect( defaultScreenRect( viewport ) ) ;
    m_view->resize( rect.size() ) ;
    m_view->setMinimumSize(1, 1);
    m_view->setMaximumSize(rect.size());
    m_view->setSceneRect( rect ) ;
    m_view->show() ;
    m_view->setFrameShape( QFrame::NoFrame ) ;
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff ) ;
    m_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff ) ;

    m_dbusproxy = new org::freedesktop::DBus("org.freedesktop.DBus", "/org/freedesktop/DBus", m_connection, this);
    connect(m_dbusproxy, SIGNAL(NameOwnerChanged(QString,QString,QString)), this, SLOT(imChanged(QString,QString,QString)));
    
    qDebug() << rect;
    qDebug() << m_view->isVisible() << mainWindow->isVisible();
}

FcitxHost::~FcitxHost()
{
    
}

FcitxHost* FcitxHost::create(MAbstractInputMethodHost* host, QWidget* mainWindow)
{
    FcitxHost *fcitxHost = new FcitxHost(host, mainWindow);
    fcitxHost->createInputContext();
    return fcitxHost;
}

void FcitxHost::createInputContext()
{

    if (!m_improxy->isValid())
        return;

    QDBusPendingReply< int, uint, uint, uint, uint > result = m_improxy->CreateIC();
    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(result);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(createInputContextFinished(QDBusPendingCallWatcher*)));
}


void FcitxHost::createInputContextFinished(QDBusPendingCallWatcher* watcher)
{
    QDBusPendingReply< int, uint, uint, uint, uint > result = *watcher;
    if (result.isError())
        qWarning() << result.error();
    else
    {
        m_id = qdbus_cast<int>(result.argumentAt(0));
        m_enable = false;
        m_triggerKey[0].sym = (FcitxKeySym) qdbus_cast<uint>(result.argumentAt(1));
        m_triggerKey[0].state = qdbus_cast<uint>(result.argumentAt(2));
        m_triggerKey[1].sym = (FcitxKeySym) qdbus_cast<uint>(result.argumentAt(3));
        m_triggerKey[1].state = qdbus_cast<uint>(result.argumentAt(4));
        m_path = QString(FCITX_IC_DBUS_PATH_QSTRING).arg(m_id);
        m_icproxy = new org::fcitx::Fcitx::InputContext(m_serviceName, m_path, m_connection, this);
        connect(m_icproxy, SIGNAL(CloseIM()), this, SLOT(closeIM()));
        connect(m_icproxy, SIGNAL(CommitString(QString)), this, SLOT(commitString(QString)));
        connect(m_icproxy, SIGNAL(EnableIM()), this, SLOT(enableIM()));
        connect(m_icproxy, SIGNAL(ForwardKey(uint, uint, int)), this, SLOT(forwardKey(uint, uint, int)));
        connect(m_icproxy, SIGNAL(UpdatePreedit(QString,int)), this, SLOT(updatePreedit(QString, int)));

        if (m_icproxy->isValid())
            m_icproxy->FocusIn();

        if (m_icproxy)
            m_icproxy->SetCapacity(CAPACITY_CLIENT_SIDE_UI);
    }
    delete watcher;
}


void FcitxHost::handleFocusChange(bool focusIn)
{
}

void FcitxHost::show()
{
    if (m_content)
        m_content->show();
    
    const QRegion region( m_inputMethodArea ) ;
    inputMethodHost()->setScreenRegion( region ) ;
    inputMethodHost()->setInputMethodArea( region ) ;
}

void FcitxHost::hide()
{
    if (m_content)
        m_content->hide();
    
    const QRegion region( m_inputMethodArea ) ;
    inputMethodHost()->setScreenRegion( region ) ;
    inputMethodHost()->setInputMethodArea( region ) ;
}

void FcitxHost::setPreedit(const QString& preeditString, int cursorPosition)
{
    Q_UNUSED(preeditString);
    Q_UNUSED(cursorPosition);
}

void FcitxHost::update()
{
    bool flag ;
    const QRect cursorRect( inputMethodHost()->cursorRectangle( flag ) ) ;
    if ( flag && m_cursorRect != cursorRect ) {
        m_cursorRect = cursorRect ;
        emit cursorRectChanged( m_cursorRect ) ;
    }
}

void FcitxHost::reset()
{
    MAbstractInputMethod::reset();
}

void FcitxHost::handleMouseClickOnPreedit(const QPoint& mousePos, const QRect& preeditRect)
{
    MAbstractInputMethod::handleMouseClickOnPreedit(mousePos, preeditRect);
}

void FcitxHost::handleVisualizationPriorityChange(bool priority)
{
    MAbstractInputMethod::handleVisualizationPriorityChange(priority);
}

void FcitxHost::handleAppOrientationAboutToChange(int angle)
{
    MAbstractInputMethod::handleAppOrientationAboutToChange(angle);
}

void FcitxHost::handleAppOrientationChanged(int angle)
{
    if (m_appOrientation.toInt() != angle)
    {
        m_appOrientation = QString::number(angle);
        emit appOrientationChanged(m_appOrientation);
    }
}

void FcitxHost::setToolbar(QSharedPointer< const MToolbarData > toolbar)
{
    MAbstractInputMethod::setToolbar(toolbar);
}

void FcitxHost::setState(const QSet< MInputMethod::HandlerState >& state)
{
    MAbstractInputMethod::setState(state);
}

void FcitxHost::processKeyEvent(QEvent::Type keyType, Qt::Key keyCode, Qt::KeyboardModifiers modifiers, const QString& text, bool autoRepeat, int count, quint32 nativeScanCode, quint32 nativeModifiers, long unsigned int time)
{
    MAbstractInputMethod::processKeyEvent(keyType, keyCode, modifiers, text, autoRepeat, count, nativeScanCode, nativeModifiers, time);
}

void FcitxHost::handleClientChange()
{
    if (m_icproxy->isValid())
        m_icproxy->Reset();
}

void FcitxHost::switchContext(MInputMethod::SwitchDirection direction, bool enableAnimation)
{
    MAbstractInputMethod::switchContext(direction, enableAnimation);
}

void FcitxHost::setActiveSubView(const QString& subViewId, MInputMethod::HandlerState state)
{
    MAbstractInputMethod::setActiveSubView(subViewId, state);
}

QString FcitxHost::activeSubView(MInputMethod::HandlerState state) const
{
    return "" ;
}

void FcitxHost::showLanguageNotification()
{
    MAbstractInputMethod::showLanguageNotification();
}

void FcitxHost::setKeyOverrides(const QMap< QString, QSharedPointer< MKeyOverride > >& newOverrides)
{
    MAbstractInputMethod::setKeyOverrides(newOverrides);
}

QList< MAbstractInputMethod::MInputMethodSubView > FcitxHost::subViews(MInputMethod::HandlerState state) const
{
    return QList<MAbstractInputMethod::MInputMethodSubView>() ;
}

int FcitxHost::screenWidth()
{
    return defaultScreenRect().width();
}

int FcitxHost::screenHeight()
{
    return defaultScreenRect().height();
}

QRect FcitxHost::cursorRect()
{
    return m_cursorRect;
}

const QString& FcitxHost::appOrientation()
{
    qDebug() << m_appOrientation;
    return m_appOrientation;
}

void FcitxHost::setScreenRegion(const QRect& area)
{

}

void FcitxHost::setInputMethodArea(const QRect& area)
{
    if ( m_inputMethodArea != area ) {
        m_inputMethodArea = area;
    }
    qDebug() << m_inputMethodArea;
}

void FcitxHost::sendCommit(const QString& text)
{
    inputMethodHost()->sendCommitString( text ) ;
}

void FcitxHost::commitString(const QString& text)
{
    inputMethodHost()->sendCommitString( text ) ;
}

void FcitxHost::sendPreedit(const QString& text)
{

}

void FcitxHost::enableIM()
{
    m_enable = true;
}

void FcitxHost::closeIM()
{
    m_enable = false;
}

void FcitxHost::forwardKey(uint keyval, uint state, int type)
{

}

void FcitxHost::updatePreedit(const QString& str, int cursorPos)
{

}

void FcitxHost::imChanged(const QString& service, const QString& oldowner, const QString& newowner)
{
    if (service == m_serviceName)
    {
        /* old die */
        if (oldowner.length() > 0 || newowner.length() > 0)
        {
            if (m_improxy)
            {
                delete m_improxy;
                m_improxy = NULL;
            }

            if (m_icproxy)
            {
                delete m_icproxy;
                m_icproxy = NULL;
            }
            m_enable = false;
            m_triggerKey[0].sym = m_triggerKey[1].sym = (FcitxKeySym) 0;
            m_triggerKey[0].state = m_triggerKey[1].state = 0;
        }

        /* new rise */
        if (newowner.length() > 0)
            createInputContext();
    }
}