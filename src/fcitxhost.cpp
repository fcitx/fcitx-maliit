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
    m_scene (new QGraphicsScene( defaultScreenRect(), this )),
    m_view( new MImGraphicsView( m_scene, mainWindow ) ) ,
    m_engine( new QDeclarativeEngine( this ) ),
    m_component(0),
    m_content( 0 ),
    m_appOrientation("0")
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
    
    qDebug() << rect;
    qDebug() << m_view->isVisible() << mainWindow->isVisible();
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

}

void FcitxHost::sendPreedit(const QString& text)
{

}
