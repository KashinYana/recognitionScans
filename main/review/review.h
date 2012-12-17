#ifndef REVIEW_H
#define REVIEW_H

#include <QWidget>
#include <QMainWindow>

#include "DataPicture.h"

class QLineEdit;
class Button;
class QLabel;
class QScrollArea;

class Review : public QWidget
{
    Q_OBJECT

public:
    Review(const std::vector<DataPicture>&);

private slots:
    void log();

private:
    Button *createButton(const QString &text, const char *member);
    QLineEdit *display;
    Button *logButton;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    std::vector<DataPicture> queuePicture;
    int idLastPicture;
};

#endif // REVIEW_H
