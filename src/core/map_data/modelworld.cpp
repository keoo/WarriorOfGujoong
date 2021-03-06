#include <QFile>
#include <qdom.h>
#include <QPixmap>
//#include <qapplication.h>
#include "constants/ChainConstants.hpp"
#include "core/map_data/tiledata.hpp"
#include "modelarea.h"
#include "modelworld.h"

#include <iostream>
using namespace std;

ModelWorld::ModelWorld(const QString & fileSource)
{
    // Initializing xml document class
    QDomDocument doc;
    {
        QFile f(fileSource);
        f.open(QIODevice::ReadOnly);
        if(!f.isOpen()) {
            throw("file " + fileSource + " not opened/found");
        }
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
                        if ( tag_child.hasAttribute("id") && tag_child.hasAttribute("path")  && tag_child.hasAttribute("canWalk"))
                        {
                            const QString id (tag_child.attribute("id"));
                            if ( this->_tiles_map.count(id) == 0 )
                            {
                                QSharedPointer<TileData> pix (new TileData(new QPixmap(Constants::IMAGES_PATH + tag_child.attribute("path")), tag_child.attribute("canWalk") == "1" ? true : false ));
                                this->_tiles_map.insert(pair<QString,QSharedPointer<TileData> >(id,pix));
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




const map < QString, QSharedPointer<TileData> > & ModelWorld::get_tiles_map() const
{
    return this->_tiles_map;
}




const map < QString, QSharedPointer<ModelArea> > & ModelWorld::get_modelarea_map() const
{
    return this->_modelarea_map;
}
