#ifndef PERSOSTATISTICS_HPP
#define PERSOSTATISTICS_HPP

#include <QWidget>

namespace Ui {
class PersoStatistics;
}

class PersoStatistics : public QWidget
{
    Q_OBJECT
    
public:
    explicit PersoStatistics(QWidget *parent = 0);
    ~PersoStatistics();

private:
    Ui::PersoStatistics *ui;

public slots:
    void slot_show_view(/*Perso*/);
    void slot_hide_view();
};

#endif // PERSOSTATISTICS_HPP
