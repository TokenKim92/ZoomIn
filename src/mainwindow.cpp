#include "mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QClipboard>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pixMapLen = 32;
    m_pixMapHalfLen = m_pixMapLen / 2;
    m_lastPos = {0, 0};

    m_shiftPressed = false;
    m_rgbAreaClicked = false;

    m_curRGB = {255,255,255};
    m_rgbBkColors[0] = {185, 185, 195};
    m_rgbBkColors[1] = {215, 215, 225};
    m_rgbBkColor = m_rgbBkColors[m_rgbAreaClicked];

    m_foundWidget = NULL;

    onInitMainWindow();
}

MainWindow::~MainWindow()
{
    m_timer->stop();
}

void MainWindow::onInitMainWindow()
{
    setFixedSize(MAINWINDOW_LENGTH, MAINWINDOW_LENGTH);
    setWindowFlags(Qt::WindowStaysOnTopHint );
    setFocusPolicy(Qt::WheelFocus);

    m_clientRect = this->rect();
    m_rgbBkRect = {7, m_clientRect.bottom() - 39, 100, 32};
    m_rgbRect = {m_rgbBkRect.left() + 3 , m_rgbBkRect.top() + 3, 26, 26};
    m_rgbTextRect = {m_rgbBkRect.left()+ 28, m_rgbBkRect.top(), m_rgbBkRect.right() - m_rgbBkRect.left() - 30, 32};

    m_rgbBrush.setStyle(Qt::SolidPattern);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onTimer);
    m_timer->start(300);
}

// functions to handle signals
// ------------------------------------------------------------------
void MainWindow::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);

    painter.drawPixmap(m_clientRect, m_pixMap, QRect(0, 0, m_pixMapLen, m_pixMapLen));
    drawRGBArea(&painter);
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if(Qt::Key_Shift == keyEvent->key()) m_shiftPressed = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if(Qt::Key_Shift == keyEvent->key()) m_shiftPressed = false;
}

void MainWindow::mousePressEvent(QMouseEvent *mouseEvent)
{
    getColorAtPixel(mouseEvent->pos());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if(m_rgbAreaClicked) {
        m_rgbAreaClicked = false;
        m_rgbBkColor = m_rgbBkColors[m_rgbAreaClicked];
        update(m_rgbBkRect);
    }
}

void MainWindow::wheelEvent(QWheelEvent *wheelEvent)
{
    if(wheelEvent->angleDelta().y() < 0) {
        if (m_pixMapLen <= MAX_IMAGE_LENGTH / 2) {
            m_pixMapHalfLen = m_pixMapLen;
            m_pixMapLen *= 2;

            captureImageFromScreen(m_lastPos.x(), m_lastPos.y());
        }
    } else {
        if (m_pixMapLen >= MIN_IMAGE_LENGTH * 2) {
            m_pixMapLen = m_pixMapHalfLen;
            m_pixMapHalfLen /= 2;

            captureImageFromScreen(m_lastPos.x(), m_lastPos.y());
        }
    }
}

void MainWindow::focusOutEvent(QFocusEvent* focusEvent)
{
    m_timer->stop();

    QMainWindow::focusOutEvent(focusEvent);
}

void MainWindow::focusInEvent(QFocusEvent* focusEvent)
{
    m_timer->start(300);

    QMainWindow::focusInEvent(focusEvent);
}
// ------------------------------------------------------------------

void MainWindow::drawRGBArea(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    m_rgbBrush.setColor(m_rgbBkColor);
    painter->setBrush(m_rgbBrush);
    painter->drawRoundedRect(m_rgbBkRect, 5, 5);

    m_rgbBrush.setColor(QColor(m_curRGB));
    painter->setBrush(m_rgbBrush);
    painter->drawRoundedRect(m_rgbRect, 5, 5);

    painter->setFont({"Gulim", 10, QFont::Bold});
    painter->setPen(Qt::SolidLine);
    painter->drawText(m_rgbTextRect, Qt::AlignVCenter | Qt::AlignHCenter,
                        (QString("#%1%2%3").arg(m_curRGB.red(), 2, 16, QLatin1Char('0'))
                                           .arg(m_curRGB.green(), 2, 16, QLatin1Char('0'))
                                           .arg(m_curRGB.blue(), 2, 16, QLatin1Char('0'))).toUpper());
}

void MainWindow::onTimer()
{
    if(m_shiftPressed) {
        m_lastPos = QCursor::pos();

        m_foundWidget = QApplication::widgetAt(m_lastPos);
        if(this != m_foundWidget)
            captureImageFromScreen(m_lastPos.x(), m_lastPos.y());
    }
}

void MainWindow::findTargetWidget(QPoint pos)
{
    /* TODO: check which widegt is found with the widgetAt function
     QWidget *foundWidget = QApplication::widgetAt(m_lastPos);
     if(foundWidget != NULL) {
         pos = foundWidget->pos();
     }
     */

}

void MainWindow::captureImageFromScreen(int x, int y)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    m_pixMap = screen->grabWindow(0, x - m_pixMapHalfLen, y - m_pixMapHalfLen, m_pixMapLen, m_pixMapLen);

    update();
}

void MainWindow::getColorAtPixel(QPoint pos)
{
    if(!m_rgbBkRect.contains(pos)) {
        QImage tempImage = m_pixMap.toImage();
        m_curRGB = tempImage.pixel(pos.x() * m_pixMapLen / MAINWINDOW_LENGTH,
                                   pos.y() * m_pixMapLen / MAINWINDOW_LENGTH);
    } else {
        m_rgbAreaClicked = true;
        m_rgbBkColor = m_rgbBkColors[m_rgbAreaClicked];

        QGuiApplication::clipboard()->setText((QString("#%1%2%3").arg(m_curRGB.red(), 2, 16, QLatin1Char('0'))
                                                                 .arg(m_curRGB.green(), 2, 16, QLatin1Char('0'))
                                                                 .arg(m_curRGB.blue(), 2, 16, QLatin1Char('0'))).toUpper());
    }

    update(m_rgbBkRect);
}
