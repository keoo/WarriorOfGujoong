#ifndef CHAINCONSTANTS_HPP
#define CHAINCONSTANTS_HPP

#include <QString>
#include <QObject>

namespace Constants {
static QString ATTACK = QObject::tr("Attack");
static QString MAGIC = QObject::tr("Magic");
static QString FINISH_ACTION = QObject::tr("Finish");
static QString ASK_END_TURN = QObject::tr("Have you finished your turn ?");
static QString TITLE_ASK_END_TURN = QObject::tr("End of turn ?");
static QString RUN = QObject::tr("Run");

// Image paths : TODO do better than this

static const QString IMAGES_PATH = "../resources/WarriorOfGujoong-tiles/";
static const QString IMAGES_FACES_PATH = IMAGES_PATH + "faces/";
static const QString IMAGES_DATA_PATH = IMAGES_PATH + "fight/";
static const QString IMAGES_PERSOS_PATH = IMAGES_PATH + "persos/";

};

#endif // CHAINCONSTANTS_HPP
