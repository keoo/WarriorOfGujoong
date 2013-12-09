#ifndef DIALOGTEXT_HPP
#define DIALOGTEXT_HPP

#include <QString>

struct DialogText {
    DialogText(const QString &perso, const QString &text);

    QString _perso;
    QString _text;
};

#endif // DIALOGTEXT_HPP
