#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QObject>
#include <QWidget>

class AbstractView : public QObject
{
    Q_OBJECT
public:
    explicit AbstractView(QObject *parent = 0);
    virtual ~AbstractView();
    virtual QWidget *GetViewsFrame() = 0;

};

#endif // ABSTRACTVIEW_H
