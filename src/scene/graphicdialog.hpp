#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QTimer>
#include <QSharedPointer>
#include <QGraphicsProxyWidget>

#include "util/direction.hpp"

class QGraphicsScene;
class QLabel;
class QTextEdit;
class DialogText;

struct DialogPosition {
    Direction _horizontal_dir;
    Direction _vertical_dir;

    DialogPosition(Direction h_dir=CENTER,
                   Direction v_dir=BOTTOM) : _horizontal_dir(h_dir), _vertical_dir(v_dir) {}
};

class GraphicDialog : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    explicit GraphicDialog(QGraphicsScene *scene, const QList<QSharedPointer<DialogText> > &dialogs, const DialogPosition &dialog_pos);
    
private:
    QTimer _timer;

    QList<QSharedPointer <DialogText> > _texts;

    QLabel *_icon_perso;
    QTextEdit *_text_widget;

    int _cur_text;
    int _cur_letter;

    void set_position(QGraphicsScene *scene, const DialogPosition &pos);

public:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void keyPressEvent(QKeyEvent *event);

signals:
    void signal_end_of_dialogs();
    
public slots:
    void next_char();

    void next_text();
};

#endif // DIALOG_HPP
