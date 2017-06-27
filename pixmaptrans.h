#ifndef PIXMAPTRANS_H
#define PIXMAPTRANS_H
#include "QString"
#include "QImage"
#include "QFile"
#include "QList"


struct rgbValue{
    QString index = NULL;
    QString rValue = NULL;
    QString gValue = NULL;
    QString bValue = NULL;
};
class pixmapTrans
{
public:
    pixmapTrans();
    pixmapTrans(QString);
    pixmapTrans(QString ,QString);

    void pixmapFormatTrans();
private:
    rgbValue paletteValue[256];
    float minDValue = 0;
//    double dValueRecord[256];
    QList <double> dValueRecord;

    int rLikePixpleIndex = 0;
    QImage img;
    QImage *examImg;

    QString picturePath;    /* 图片路径 */
    QString corlorFilePath;     /* 调色板路径 */
    QFile   *file;
    QList <QString> colorPallet;
    QByteArray hexData[256];
};

#endif // PIXMAPTRANS_H
