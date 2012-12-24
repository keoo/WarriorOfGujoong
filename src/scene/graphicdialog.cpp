#include <iostream>
/* -- */
#include <QApplication>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QTextEdit>
#include <QPixmap>
#include <QHBoxLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QSharedPointer>
/* -- */
#include "core/map_data/dialogtext.hpp"
/* -- */
#include "graphicdialog.hpp"

static const QString IMAGES_FACES_PATH = "/tmp/WarriorOfGujoong-tiles/faces/";

GraphicDialog::GraphicDialog(QGraphicsScene *scene, const QList<QSharedPointer<DialogText> > &dialogs, const DialogPosition &dialog_pos) :
    QGraphicsProxyWidget()
{
    _timer.setInterval(100);

    _cur_text = 0;
    _cur_letter = 0;

    _texts = dialogs;

    _icon_perso = new QLabel("");
    QWidget *widget = new QWidget;
    widget->setStyleSheet("background: rgba(30%, 30%, 30%, 40%); border: 1px solid gray; border-radius: 15px;");
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    widget->setLayout(horizontalLayout);
    horizontalLayout->addWidget(_icon_perso);
    _text_widget = new QTextEdit;
    _text_widget->setReadOnly(true);
    _text_widget->setTextInteractionFlags(Qt::NoTextInteraction);
    _text_widget->setStyleSheet("border: 2px solid gray; border-radius: 10px; padding: 0 8px; \
                                background: transparent;");

    QFontMetrics metrics(QApplication::font());
    _text_widget->setFixedHeight(4*metrics.height());
    horizontalLayout->addWidget(_text_widget);

    setWidget(widget);

    scene->addItem(this);

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    setMinimumWidth(scene->sceneRect().width()/2);

    set_position(scene, dialog_pos);

   // setScale(1.5);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(next_char()));
}

void GraphicDialog::set_position(QGraphicsScene *scene, const DialogPosition &dialog_pos)
{
    // Set position
   double pos_x = 0;
   switch(dialog_pos._horizontal_dir) {
   case LEFT:
       pos_x = 0;
       break;
   case RIGHT:
       pos_x = scene->sceneRect().width()-size().width();
       break;
   default:
       pos_x = (scene->sceneRect().width()-size().width()) / 2.;
   break;
   }
   double pos_y = 0;
   switch(dialog_pos._vertical_dir) {
   case TOP:
       pos_y = 0;
       break;
   case BOTTOM:
       pos_y = scene->sceneRect().height()-size().height();
       break;
   default:
       pos_y = (scene->sceneRect().height()-size().height()) / 2.;
   break;
   }
    setPos(pos_x, pos_y);


}

void GraphicDialog::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    next_text();
}

void GraphicDialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        emit signal_end_of_dialogs();
    }
}

void GraphicDialog::next_char()
{
    _cur_letter ++;
    if(_cur_letter <= _texts[_cur_text].data()->_text.size()) {
        _text_widget->setHtml(_texts[_cur_text].data()->_text.left(_cur_letter));
    }
    else {
        _timer.stop();
        _cur_text ++;
    }
}

void GraphicDialog::next_text()
{
    if(_timer.isActive()) {
        // Stop current text
        _text_widget->setHtml(_texts[_cur_text].data()->_text);
        _timer.stop();
        _cur_text ++;
    }
    else {
        // Don't go too far
        if(_cur_text >= _texts.size()) {
            _cur_text = 0;
            emit signal_end_of_dialogs();
        }
        else {
        _cur_letter = 0;

        // Change the icon
        _icon_perso->setPixmap(QPixmap(IMAGES_FACES_PATH+_texts[_cur_text].data()->_perso + ".png"));
        // Start timer
        _timer.start();
        }
    }
}
