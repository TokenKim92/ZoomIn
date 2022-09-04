#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define MAX_IMAGE_LENGTH		256
#define MIN_IMAGE_LENGTH		16
#define MAINWINDOW_LENGTH       300

class MainWindow : public QMainWindow
{    
    Q_OBJECT
private:
    QPixmap m_pixMap;
    unsigned short m_pixMapLen;
    unsigned short m_pixMapHalfLen;

    QRect m_clientRect;
    QRect m_rgbBkRect;
    QRect m_rgbRect;
    QRect m_rgbTextRect;

    QPoint m_lastPos;

    bool m_shiftPressed;
    bool m_rgbAreaClicked;

    QBrush m_rgbBrush;
    QColor m_curRGB;
    QColor m_rgbBkColors[2];
    QColor m_rgbBkColor;

    QWidget *m_foundWidget;
    QTimer *m_timer;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onInitMainWindow();

    virtual void paintEvent(QPaintEvent *paintEvent);
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void keyReleaseEvent(QKeyEvent *keyEvent);
    virtual void mousePressEvent(QMouseEvent *mousEevent);
    virtual void mouseReleaseEvent(QMouseEvent *mouseEvent);
    virtual void wheelEvent(QWheelEvent *wheelEvent);
    virtual void focusOutEvent(QFocusEvent* focusEvent);
    virtual void focusInEvent(QFocusEvent* focusEvent);

    void drawRGBArea(QPainter *painter);
    void findTargetWidget(QPoint pos);
    void onTimer();
    void captureImageFromScreen(int x, int y);
    void getColorAtPixel(QPoint pos);
};
#endif // MAINWINDOW_H
