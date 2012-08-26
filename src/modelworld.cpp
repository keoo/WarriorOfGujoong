#include "modelworld.h"
#include "modelarea.h"

#include <qfile.h>
#include <qdom.h>
//#include <qapplication.h>
#include <QPixmap>

#include <iostream>
using namespace std;

ModelWorld::ModelWorld(const QString & fileSource)
{
    // Initializing xml document class
    QDomDocument doc;
    {
        QFile f(fileSource);
        f.open(QIODevice::ReadOnly);
        doc.setContent(&f);
        f.close();
    }

    // Filling tiles_map
    {
        QDomElement root=doc.documentElement();
        QDomElement child=root.firstChild().toElement();
        while(!child.isNull())
        {
            if (child.tagName() == "tiles")
            {
                QDomElement tag_child = child.firstChild().toElement();
                while(!tag_child.isNull())
                {
                    if (tag_child.tagName() == "tile")
                    {
                        if ( tag_child.hasAttribute("id") && tag_child.hasAttribute("path") )
                        {
                            const QString id (tag_child.attribute("id"));
                            if ( this->_tiles_map.count(id) == 0 )
                            {
                                QSharedPointer<QPixmap> pix (new QPixmap(tag_child.attribute("path") ));
                                this->_tiles_map.insert(pair<QString,QSharedPointer<QPixmap> >(id,pix));
                            }
                        }
                    }
                    tag_child = tag_child.nextSibling().toElement();
                }
            }
            child = child.nextSibling().toElement();
        }
    }

    // Filling modelarea map
    {
        QDomElement root=doc.documentElement();
        QDomElement child=root.firstChild().toElement();
        while(!child.isNull())
        {
            if (child.tagName() == "area")
            {
                if ( child.hasAttribute("id") )
                {
                    const QString id (child.attribute("id"));
                    QSharedPointer<ModelArea> ma ( new ModelArea(child,this->_tiles_map) );
                    this->_modelarea_map.insert(pair<QString,QSharedPointer<ModelArea> >(id,ma));
                }
            }
            child = child.nextSibling().toElement();
        }
    }
}




const map < QString, QSharedPointer<QPixmap> > & ModelWorld::get_tiles_map() const
{
    return this->_tiles_map;
}




const map < QString, QSharedPointer<ModelArea> > & ModelWorld::get_modelarea_map() const
{
    return this->_modelarea_map;
}
