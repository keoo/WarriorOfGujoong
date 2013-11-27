#include "constants/ChainConstants.hpp"
/* -- */
#include "core/Perso.hpp"
/* -- */
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

void PersoStatistics::slot_show_view(Perso *perso)
{
    setVisible(true);
    // TODO Bind data from perso
    ui->photo->setPixmap(QPixmap(Constants::IMAGES_FACES_PATH+QString::fromStdString(perso->get_name()+".png")));
    ui->name->setText(QString::fromStdString(perso->get_name()));
    ui->class_lvl->setText("Lvl : " + QString().setNum(perso->get_level()));
    ui->hp_label->setText("HP : " + QString().setNum(perso->get_HP()));
    ui->mp_label->setText("MP : " + QString().setNum(perso->get_MP()));
    ui->weapon->setText(QString::fromStdString(perso->get_weapon().get_name()));
    ui->shield->setText(QString::fromStdString(perso->get_shield().get_name()));

}

void PersoStatistics::slot_hide_view()
{
    setVisible(false);
}

