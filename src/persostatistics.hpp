#ifndef PERSOSTATISTICS_HPP
#define PERSOSTATISTICS_HPP

#include <QWidget>

class Perso;

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
    void slot_show_view(Perso *perso);
    void slot_hide_view();
};

#endif // PERSOSTATISTICS_HPP
