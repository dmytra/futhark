#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

class PiecesList;
class FutharkWidget;
QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void loadImage(const QString &path);

public slots:
    void openImage();
    void setupFuthark();

private slots:
    void setCompleted();

private:
    void setupMenus();
    void setupWidgets();

    QPixmap futharkImage;
    PiecesList *piecesList;
    FutharkWidget *futharkWidget;
};

#endif // MAINWINDOW_H
