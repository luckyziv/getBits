#include "pixmaptrans.h"
#include "QDebug"
#include "QPainter"
#include "QImage"
#include "QByteArray"
#include "QFile"
#include "QColor"
#include "QRgb"
#include "QtMath"

pixmapTrans::pixmapTrans()
{

}

pixmapTrans::pixmapTrans(QString picturepath)
{
    picturePath = picturepath;
}

pixmapTrans::pixmapTrans(QString picturepath,QString corlorfilepath)
{
    picturePath = picturepath;
    corlorFilePath  = corlorfilepath;
    file = new QFile(corlorFilePath);
    img.load(picturePath);
}

void pixmapTrans::pixmapFormatTrans()
{
    /* colorPallet is to make a Qlist! */
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){     /* read file OK? */
        qDebug()<<"Can not open the file!";
    }

    int i=0;
    qDebug()<<"正在读取调色板文件,请稍等...";
    while(!file->atEnd()) {
        QByteArray line = file->readLine();
        QString str(line);
//        qDebug()<<"str.length="<<str.length();
//        qDebug()<<str;

        if(str.length()==10||str.length()==11){
            if(str.length()==10){
                /*                qDebug()<<str.mid(0,1)<<"  "<<str.mid(2,6);
                colorPallet.insert(i,QByteArray::fromHex(str.mid(2,6)));
                hexData[i].append(str.mid(2,2));
                qDebug()<<hexData[i].toInt();*/
                paletteValue[i].index = QString::number(i,16).toUpper();
                paletteValue[i].rValue = str.mid(2,2);
                paletteValue[i].gValue = str.mid(4,2);
                paletteValue[i].bValue = str.mid(6,2);
                /*                qDebug()<<paletteValue[i].index
                                       <<paletteValue[i].rValue.toInt(0,16)
                                       <<paletteValue[i].gValue.toInt(0,16)
                                     <<paletteValue[i].bValue.toInt(0,16);*/
                i++;
            }
            else{
                /*qDebug()<<str.mid(0,2)<<"  "<<str.mid(3,6);
                colorPallet.insert(i,str.mid(3,6));
                hexData[i].append(str.mid(3,2));
                qDebug()<<hexData[i].toInt();*/
                paletteValue[i].index = QString::number(i,16).toUpper();
                paletteValue[i].rValue = str.mid(3,2);
                paletteValue[i].gValue = str.mid(5,2);
                paletteValue[i].bValue = str.mid(7,2);
                /*qDebug()<<paletteValue[i].index
                                       <<paletteValue[i].rValue.toInt(0,16)
                                      <<paletteValue[i].gValue.toInt(0,16)
                                     <<paletteValue[i].bValue.toInt(0,16);*/
                i++;
            }
        }
    }
    file->close();
//    qDebug()<<"file close!";
    qDebug()<<"调色板读取完毕!";
    /*for(int i = 0;i<256;i++){
            qDebug()<<paletteValue[i].index<<paletteValue[i].rValue<<paletteValue[i].gValue<<paletteValue[i].bValue;
        }*/

    int x;
    int y;
    QColor color;
    color = img.pixel(0,1);

    qDebug()<<"正在生成新文件,请稍等...";
    /* 生成新文件 */
    QFile f("./hello.mif");
    QTextStream txtOutput(&f);
    if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"Open failed."<<endl;
    }else{
        txtOutput << "WIDTH = 8;" << endl;
        txtOutput << "DEPTH = 307200;" << endl << endl;
        txtOutput << "ADDRESS_RADIX = HEX;" <<endl;
        txtOutput << "DATA_RADIX = HEX;" <<endl <<endl;
        txtOutput << "CONTENT BEGIN" <<endl;

    }

    int count = 0;
    for(x=0;x<img.width();x++)
        for(y=0;y<img.height();y++){
            color = img.pixelColor(x,y);
            //qDebug()<<color.red()<<color.green()<<color.blue();

            /* 求方差（相似度） */
            for(int i=0;i<256;i++){
                dValueRecord.append(sqrt((color.red()-paletteValue[i].rValue.toInt(0,16))*(color.red()-paletteValue[i].rValue.toInt(0,16))+
                                         (color.green()-paletteValue[i].gValue.toInt(0,16))*(color.green()-paletteValue[i].gValue.toInt(0,16))+
                                         (color.blue()-paletteValue[i].bValue.toInt(0,16))*(color.blue()-paletteValue[i].bValue.toInt(0,16))));
            }
            int sum = 0;
            /* 获取方差（相似度）的最小值 */
            double temp = dValueRecord.value(0);
            for(int i=0;i<256;i++){
                if(temp >= dValueRecord.value(i)){
                    temp = dValueRecord.value(i);
                    /* 记录方差最小值有几个 */
                    ++sum;
//                    qDebug()<<temp;
                }
            }
//            qDebug()<<"相似值数目:"<<sum;
            /* 输出方差最小值 */
//            qDebug()<<"最小值为:"<<temp;

            /* 找到相似点坐标 */
            sum = 0;
            for(int i=0;i<256;i++){
                if(temp == dValueRecord.value(i)){
                    ++sum;
                    rLikePixpleIndex = i;
                    //txtOutput << QString::number(count,16).toUpper();
                }
            }
            //qDebug()<<"方差数组中相似的个数:"<<sum;
            //qDebug()<<++count;


            txtOutput<<QString::number(count,16).toUpper();
            count++;
            txtOutput<<":";
            if(rLikePixpleIndex<16){
                txtOutput<<"0"<<paletteValue[rLikePixpleIndex].index;
            }
            else
                txtOutput<<paletteValue[rLikePixpleIndex].index;
            txtOutput<<";";
            txtOutput<<endl;

            /* dValueRecord是用来记录方差的，一个像素点记录完就要清空！ */
            dValueRecord.clear();
        }
    txtOutput<<"END;";
    f.close();
    qDebug()<<"新文件生成成功!";
}
