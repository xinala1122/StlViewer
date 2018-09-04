#include "AbstractView.h"
#include <QWidget>

AbstractView::AbstractView(QObject *parent) :
    QObject(parent)
{
}

AbstractView::~AbstractView()
{
}

