#ifndef QTFRAME_MYWIDGET_H
#define QTFRAME_MYWIDGET_H

#include <QtWidgets/QLabel>
#include <QPainter>

class EventLabel : public QLabel {
public:
    bool draw_rect{false}, flag{false};
    int x0{0}, y0{0}, x1{0}, y1{0};
    int x_min{0}, y_min{0}, x_max{0}, y_max{0};

    // 重写鼠标点击事件
    void mousePressEvent(QMouseEvent *event) {
        if (draw_rect) {
            flag = true;
            x0 = event->x();
            y0 = event->y();
        }
    };

    //重写鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event) {
        flag = false;
    }

    // 重写鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event) {
        if (draw_rect && flag) {
            x1 = event->x();
            y1 = event->y();

            x_min = x0 < x1 ? x0 : x1;
            y_min = y0 < y1 ? y0 : y1;
            x_max = x0 > x1 ? x0 : x1;
            y_max = y0 > y1 ? y0 : y1;
            update();
        }
    }

    //重写绘制事件
    void paintEvent(QPaintEvent *event) {
        QLabel::paintEvent(event);

        QRect rect(x_min, y_min, x_max - x_min, y_max - y_min);
        QPainter painter(this);
        QPen pen(Qt::magenta, 1, Qt::DashLine);

        painter.setPen(pen);
        painter.drawRect(rect);
    }

    // 更新矩形
    void updateRect(int x_1, int y_1, int x_2, int y_2) {
        x_min = x_1;
        y_min = y_1;
        x_max = x_2;
        y_max = y_2;

        update();
    }

    // 设置是否绘制矩形
    void setDrawRect(bool Bool) {
        draw_rect = Bool;
    }


    //获取矩形区域坐标: x1,y1,x2,y2
    void getCoord(int *array) {
        array[0] = x_min;
        array[1] = y_min;
        array[2] = x_max;
        array[3] = y_max;
    }

    EventLabel(QWidget *pWidget) : QLabel(pWidget) { }
};

#endif //QTFRAME_MYWIDGET_H
