#ifndef ACTIONMENUWINDOW_HPP
#define ACTIONMENUWINDOW_HPP

#include <QDialog>
#include <QStringList>
#include <QList>
#include <QButtonGroup>

class QRadioButton;
class QKeyEvent;

class ActionMenuWindow : public QDialog
{
    Q_OBJECT

private:
    QList <QRadioButton *> _buttons;

    QString _action_chosen;

    QButtonGroup _group;

public:
    explicit ActionMenuWindow(QWidget *parent = 0);
    
    void set_actions(const QStringList& actions);

    const QString &get_action() const;

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    
public slots:
    
};

#endif // ACTIONMENUWINDOW_HPP
