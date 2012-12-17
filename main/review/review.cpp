#include "review.h"

#include <QtGui>
#include "button.h"
#include <fstream>
#include <iostream>

Review::Review(const std::vector<DataPicture>& queuePicture_)
    : queuePicture(queuePicture_)
{
    idLastPicture = 0;

    display = new QLineEdit("0");
    display->setReadOnly(false);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(1);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 4);
    display->setFont(font);
    Button *logButton = createButton(tr("ok"), SLOT(log()));

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    log();

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(display, 0, 0, 1, 20);
    mainLayout->addWidget(logButton, 1, 0, 2, 2);
    mainLayout->addWidget(imageLabel, 1, 2, 2, 18);

    setLayout(mainLayout);
    setWindowTitle(tr("Review"));
    resize(300, 300);
}

void Review::log()
{
    if(idLastPicture > 0)
    {
        std::string inputText = display->text().toStdString();
        queuePicture[idLastPicture - 1].ball = inputText;
    }
    if(idLastPicture >= queuePicture.size())
    {
        std::ofstream config;
        config.open("../updateResult.cfg");
        for(int i = 0; i < queuePicture.size(); i++)
        {
            config << "[input_field]" << std::endl;
            config << "x = " << queuePicture[i].x << std::endl;
            config << "y = " << queuePicture[i].y << std::endl;
            config << "width = " << queuePicture[i].width << std::endl;
            config << "height = " << queuePicture[i].height << std::endl;
            config << "output_file = " << queuePicture[i].path << std::endl;
            config << "recognized_value = " << queuePicture[i].ball << std::endl;
            config << std::endl;
        }
        config.close();
        std::cout << "Config with new results: updateResult.cfg is made." << std::endl;
        exit(0);
    }
    QPixmap *image = new QPixmap(("../" + queuePicture[idLastPicture].path).c_str());
    image->scaled(50,50);
    imageLabel->setPixmap(*image);
    display->setText(queuePicture[idLastPicture].ball.c_str());
    idLastPicture++;
}

Button *Review::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}


