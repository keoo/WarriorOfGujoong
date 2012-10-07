#include <QKeyEvent>
#include <QRadioButton>
#include <QVBoxLayout>
/* -- */
#include "actionmenuwindow.hpp"

ActionMenuWindow::ActionMenuWindow(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    QVBoxLayout *new_layout = new QVBoxLayout;
    setLayout(new_layout);

    setStyleSheet("background-color: \
                  qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E0E0E0, stop: 1 #FFFFFF);border: 2px solid gray;\
    border-radius: 5px;");
}

void ActionMenuWindow::set_actions(const QStringList& actions) {

    foreach(QRadioButton *butt, _buttons) {
        layout()->removeWidget(butt);
        _group.removeButton(butt);
    }

    _buttons.clear();

    foreach(QString act, actions) {
        QRadioButton *button = new QRadioButton(act);
        layout()->addWidget(button);
        button->setStyleSheet(" QRadioButton::indicator::checked  {image: url(/tmp/WarriorOfGujoong-tiles/tile-mountain.png);} \
                              QRadioButton::indicator::unchecked  {image: url(/tmp/WarriorOfGujoong-tiles/tile-hill-001.png);}");
        _buttons.push_back(button);
        _group.addButton(button);
    }
    _buttons.last()->setChecked(true);
}

const QString &ActionMenuWindow::get_action() const
{
    return _action_chosen;
}

void ActionMenuWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        reject();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
       _action_chosen = _group.checkedButton()->text();
        accept();
        break;
    default:
        QDialog::keyPressEvent(event);
        break;
    }
}
