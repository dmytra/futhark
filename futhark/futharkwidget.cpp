
#include "futharkwidget.h"
#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>

FutharkWidget::FutharkWidget(int imageSize, QWidget *parent)
    : QWidget(parent), m_ImageSize(imageSize)
{
    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
    setMaximumSize(m_ImageSize, m_ImageSize);
}

void FutharkWidget::clear()
{
    pieces.clear();
    highlightedRect = QRect();
    inPlace = 0;
    update();
}

void FutharkWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::futharkMimeType()))
        event->accept();
    else
        event->ignore();
}

void FutharkWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void FutharkWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QRect updateRect = highlightedRect.united(targetSquare(event->position().toPoint()));

    if (event->mimeData()->hasFormat(PiecesList::futharkMimeType())
        && findPiece(targetSquare(event->position().toPoint())) == -1) {

        highlightedRect = targetSquare(event->position().toPoint());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        highlightedRect = QRect();
        event->ignore();
    }

    update(updateRect);
}

void FutharkWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::futharkMimeType())
        && findPiece(targetSquare(event->position().toPoint())) == -1) {

        QByteArray pieceData = event->mimeData()->data(PiecesList::futharkMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Piece piece;
        piece.rect = targetSquare(event->position().toPoint());
        dataStream >> piece.pixmap >> piece.location;

        pieces.append(piece);

        highlightedRect = QRect();
        update(piece.rect);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        if (piece.location == piece.rect.topLeft() / pieceSize()) {
            inPlace++;
            if (inPlace == 25)
                emit futharkCompleted();
        }
    } else {
        highlightedRect = QRect();
        event->ignore();
    }
}

int FutharkWidget::findPiece(const QRect &pieceRect) const
{
    for (int i = 0, size = pieces.size(); i < size; ++i) {
        if (pieces.at(i).rect == pieceRect)
            return i;
    }
    return -1;
}

void FutharkWidget::mousePressEvent(QMouseEvent *event)
{
    QRect square = targetSquare(event->position().toPoint());
    const int found = findPiece(square);

    if (found == -1)
        return;

    Piece piece = pieces.takeAt(found);

    if (piece.location == square.topLeft() / pieceSize())
        inPlace--;

    update(square);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << piece.pixmap << piece.location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(PiecesList::futharkMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->position().toPoint() - square.topLeft());
    drag->setPixmap(piece.pixmap);

    if (drag->exec(Qt::MoveAction) != Qt::MoveAction) {
        pieces.insert(found, piece);
        update(targetSquare(event->position().toPoint()));

        if (piece.location == square.topLeft() / pieceSize())
            inPlace++;
    }
}

void FutharkWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::white);

    if (highlightedRect.isValid()) {
        painter.setBrush(QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    for (const Piece &piece : pieces)
        painter.drawPixmap(piece.rect, piece.pixmap);
}

const QRect FutharkWidget::targetSquare(const QPoint &position) const
{
    return QRect(position / pieceSize() * pieceSize(),
                 QSize(pieceSize(), pieceSize()));
}

int FutharkWidget::pieceSize() const
{
    return m_ImageSize / 5;
}

int FutharkWidget::imageSize() const
{
    return m_ImageSize;
}
