
#ifndef PIECESLIST_H
#define PIECESLIST_H

#include <QListWidget>

class PiecesList : public QListWidget
{
    Q_OBJECT

public:
    explicit PiecesList(int pieceSize, QWidget *parent = nullptr);
    void addPiece(const QPixmap &pixmap, const QPoint &location);

    static QString futharkMimeType() { return QStringLiteral("image/x-futhark-piece"); }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;

    int m_PieceSize;
};

#endif // PIECESLIST_H
