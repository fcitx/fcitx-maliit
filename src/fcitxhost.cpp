#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QGraphicsObject>

#include <mabstractinputmethodhost.h>
#include <mimgraphicsview.h>

#include "config.h"

#include "fcitxhost.h"

#define QML_PATH PREFIX "/share/fcitx/maliit/qml/meego-keyboard.qml"

namespace {
    QRect defaultScreenRect(QWidget *w = 0)
    {
        if (not QApplication::desktop() || not w) {
            return QRect();
        }

        return QApplication::desktop()->screenGeometry(w);
    }
}

FcitxHost::FcitxHost(MAbstractInputMethodHost* host, QWidget* mainWindow)
    : MAbstractInputMethod(host, mainWindow),
    m_scene (new QGraphicsScene( defaultScreenRect(), this )),
    m_view( new MImGraphicsView( m_scene, mainWindow ) ) ,
    m_engine( new QDeclarativeEngine( this ) ),
    m_component( new QDeclarativeComponent( m_engine, QUrl( QML_PATH ) ) ),
    m_content( qobject_cast<QGraphicsObject*>( m_component->create() )  )
{
    m_scene->addItem( m_content ) ;
    m_engine->rootContext()->setContextProperty("fcitx", this);
}

FcitxHost::~FcitxHost()
{
    
}

FcitxHost* FcitxHost::create(MAbstractInputMethodHost* host, QWidget* mainWindow)
{
    FcitxHost *fcitxHost = new FcitxHost(host, mainWindow);
    fcitxHost->init();
    return fcitxHost;
}

void FcitxHost::init()
{
    MAbstractInputMethodHost *const host = inputMethodHost();
}


void FcitxHost::handleFocusChange(bool focusIn)
{
}

void FcitxHost::show()
{
}

void FcitxHost::hide()
{
}

void FcitxHost::setPreedit(const QString& preeditString, int cursorPosition)
{
    MAbstractInputMethod::setPreedit(preeditString, cursorPosition);
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
    MAbstractInputMethod::handleAppOrientationChanged(angle);
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
    MAbstractInputMethod::handleClientChange();
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
    return MAbstractInputMethod::activeSubView(state);
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
    return MAbstractInputMethod::subViews(state);
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
