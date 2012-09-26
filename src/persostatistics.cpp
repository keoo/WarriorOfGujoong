#include "persostatistics.hpp"
#include "ui_persostatistics.h"

PersoStatistics::PersoStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersoStatistics)
{
    ui->setupUi(this);
}

PersoStatistics::~PersoStatistics()
{
    delete ui;
}

void PersoStatistics::slot_show_view(/**/)
{
    setVisible(true);
    // TODO Bind data from perso
}

void PersoStatistics::slot_hide_view()
{
    setVisible(false);
}

