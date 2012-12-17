#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <QApplication>
#include "review.h"

#include "DataPicture.h"
#include <assert.h>
#include "../../config/config.hpp"
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    Config config = Config::parse("../result.cfg");
    Config updateConfig;
    std::vector<DataPicture> queuePicture;
    for (Config::Section section: config["input_field"]) {
            DataPicture newElement;
            assert(section["output_file"].size() == 1);
            for (std::string property: section["output_file"]) {
                newElement.path = property;
            }
            assert(section["recognized_value"].size() == 1);
            for (std::string property: section["recognized_value"]) {
                newElement.ball = property;
            }
            assert(section["x"].size() == 1);
            for (std::string property: section["x"]) {
                newElement.x = property;
            }
            assert(section["y"].size() == 1);
            for (std::string property: section["y"]) {
                newElement.y = property;
            }
            assert(section["width"].size() == 1);
            for (std::string property: section["width"]) {
                newElement.width = property;
            }
            assert(section["height"].size() == 1);
            for (std::string property: section["height"]) {
                newElement.height = property;
            }
            queuePicture.push_back(newElement);
    }
    QApplication app(argc, argv);
    Review review(queuePicture);
    review.show();
    return app.exec();
}
