#include <QDebug>

#include <mabstractinputmethodhost.h>

#include "fcitximgraphicsview.h"


FcitxImGraphicsView::FcitxImGraphicsView(QGraphicsScene* scene, QWidget* parent ):
    MImGraphicsView(scene, parent),
    m_host(0)
{

}

FcitxImGraphicsView::~FcitxImGraphicsView()
{

}

void FcitxImGraphicsView::setInputMethodHost(MAbstractInputMethodHost* host)
{
    m_host = host;
}


void FcitxImGraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
    if (rect.isEmpty()) {
        return;
    }
    
    QPixmap bg; 
    
    if (m_host)
        bg = m_host->background();
    if (not bg.isNull())
        painter->drawPixmap(rect.toRect(), bg, rect.toRect());
    else
    {
        painter->save();
        painter->setCompositionMode(QPainter::CompositionMode_Source);
        painter->fillRect(rect, Qt::transparent);
        painter->restore();
    }
}
