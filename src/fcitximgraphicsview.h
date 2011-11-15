#include "mimgraphicsview.h"

class MAbstractInputMethodHost;
class FcitxImGraphicsView : public MImGraphicsView
{

    Q_OBJECT
public:
    explicit FcitxImGraphicsView(QGraphicsScene* scene, QWidget* parent = 0);
    
    virtual ~FcitxImGraphicsView();
    
    void setInputMethodHost(MAbstractInputMethodHost* host);
    
protected:
    //! \reimp
    virtual void drawBackground(QPainter *painter,
                                const QRectF &rect);
    //! \reimp_end
    
    Q_DISABLE_COPY(FcitxImGraphicsView);
    
private:
    MAbstractInputMethodHost* m_host;

};