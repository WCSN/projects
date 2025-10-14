#ifndef PLOTTERBOX_H
#define PLOTTERBOX_H

#include <QWidget>

namespace Ui 
{
	class PlotterBox;
}

class PlotterBox : public QWidget
{
    Q_OBJECT

public:
    explicit PlotterBox(QWidget *parent = nullptr);
    ~PlotterBox();
    Ui::PlotterBox *ui;

};

#endif // PLOTTERBOX_H
