#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

PiecesList::PiecesList(int pieceSize, QWidget *parent)
    : QListWidget(parent), m_PieceSize(pieceSize)
{
    setDragEnabled(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(m_PieceSize, m_PieceSize));
    setSpacing(10);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

void PiecesList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::futharkMimeType()))
        event->accept();
    else
        event->ignore();
}

void PiecesList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::futharkMimeType())) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void PiecesList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::futharkMimeType())) {
        QByteArray pieceData = event->mimeData()->data(PiecesList::futharkMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint location;
        dataStream >> pixmap >> location;

        addPiece(pixmap, location);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void PiecesList::addPiece(const QPixmap &pixmap, const QPoint &location)
{
    QListWidgetItem *pieceItem = new QListWidgetItem(this);
    pieceItem->setIcon(QIcon(pixmap));
    pieceItem->setData(Qt::UserRole, QVariant(pixmap));
    pieceItem->setData(Qt::UserRole+1, location);
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

void PiecesList::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    QPoint location = item->data(Qt::UserRole+1).toPoint();

    dataStream << pixmap << location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(PiecesList::futharkMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
        delete takeItem(row(item));
}
