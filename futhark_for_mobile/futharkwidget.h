#ifndef FUTHARKWIDGET_H
#define FUTHARKWIDGET_H

#include <QPoint>
#include <QPixmap>
#include <QList>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class FutharkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FutharkWidget(int imageSize, QWidget *parent = nullptr);
    void clear();

    int pieceSize() const;
    int imageSize() const;

signals:
    void futharkCompleted();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    struct Piece {
        QPixmap pixmap;
        QRect rect;
        QPoint location;
    };

    int findPiece(const QRect &pieceRect) const;
    const QRect targetSquare(const QPoint &position) const;

    QList<Piece> pieces;
    QRect highlightedRect;
    int inPlace;
    int m_ImageSize;
};

#endif // FUTHARKWIDGET_H
