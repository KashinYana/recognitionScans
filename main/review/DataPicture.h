#ifndef DATAPICTURE_H
#define DATAPICTURE_H

class DataPicture
{
public:
    DataPicture(std::string path_, std::string ball_, std::string x_,
                 std::string y_, std::string width_, std::string height_)
    {
        path = path_;
        ball = ball_;
        x = x_;
        y = y_;
        width = width_;
        height = height_;
    }
    DataPicture()
    {

    }
    std::string path;
    std::string ball;
    std::string x;
    std::string y;
    std::string width;
    std::string height;
};

#endif // DATAPICTURE_H
