#include "modelworld.h"

#include <qfile.h>
#include <qdom.h>
#include <qapplication.h>

#include <QString>

#include <iostream>
using namespace std;

ModelWorld::ModelWorld(const QString & fileSource)
{
    QDomDocument doc;
    {
        QFile f(fileSource);
        f.open(QIODevice::ReadOnly);
        doc.setContent(&f);
        f.close();
    }

}
