
#include "mainwindow.h"
#include "pieceslist.h"
#include "futharkwidget.h"

#include <QtWidgets>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenus();
    setupWidgets();

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("old futhark"));
}

void MainWindow::openImage()
{
    const QString directory =
        QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).value(0, QDir::homePath());
    QFileDialog dialog(this, tr("Open Image"), directory);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QStringList mimeTypeFilters;
    for (const QByteArray &mimeTypeName : QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (dialog.exec() == QDialog::Accepted)
        loadImage(dialog.selectedFiles().constFirst());
}

void MainWindow::loadImage(const QString &fileName)
{
    QPixmap newImage;
    if (!newImage.load(fileName)) {
        QMessageBox::warning(this, tr("Open Image"),
                             tr("The image file could not be loaded."),
                             QMessageBox::Close);
        return;
    }
    futharkImage = newImage;
    setupFuthark();
}

void MainWindow::setCompleted()
{
    QMessageBox::information(this, tr("futhark Completed"),
                             tr("Congratulations! You have completed the futhark!\n"
                                "Click OK to start again."),
                             QMessageBox::Ok);

    setupFuthark();
}

void MainWindow::setupFuthark()
{
    int size = qMin(futharkImage.width(), futharkImage.height());
    futharkImage = futharkImage.copy((futharkImage.width() - size) / 2,
        (futharkImage.height() - size) / 2, size, size).scaled(futharkWidget->width(),
            futharkWidget->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    piecesList->clear();

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            int pieceSize = futharkWidget->pieceSize();
            QPixmap pieceImage = futharkImage.copy(x * pieceSize, y * pieceSize, pieceSize, pieceSize);
            piecesList->addPiece(pieceImage, QPoint(x, y));
        }
    }

    for (int i = 0; i < piecesList->count(); ++i) {
        if (QRandomGenerator::global()->bounded(2) == 1) {
            QListWidgetItem *item = piecesList->takeItem(i);
            piecesList->insertItem(0, item);
        }
    }

    futharkWidget->clear();
}

void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = fileMenu->addAction(tr("&Open..."), this, &MainWindow::openImage);
    openAction->setShortcuts(QKeySequence::Open);

    QAction *exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QCoreApplication::quit);
    exitAction->setShortcuts(QKeySequence::Quit);

    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));

    gameMenu->addAction(tr("&Restart"), this, &MainWindow::setupFuthark);
}

void MainWindow::setupWidgets()
{
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    futharkWidget = new FutharkWidget(400);

    piecesList = new PiecesList(futharkWidget->pieceSize(), this);


    connect(futharkWidget, &FutharkWidget::futharkCompleted,
            this, &MainWindow::setCompleted, Qt::QueuedConnection);

    frameLayout->addWidget(piecesList);
    frameLayout->addWidget(futharkWidget);
    setCentralWidget(frame);
}
