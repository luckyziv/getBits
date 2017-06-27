#include "dialog.h"
#include "ui_dialog.h"
#include "QFileDialog"
#include "QDebug"
#include "pixmaptrans.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_selecteCorlor_clicked()
{
    corlorFilePath = QFileDialog::getOpenFileName(this,
                                              tr("选择调色板"),
                                              "../",
                                              tr("调色板文件(*mif)"));
    ui->corlorFilePath->setText(corlorFilePath);
}

void Dialog::on_openPicture_clicked()
{
    picturePath = QFileDialog::getOpenFileName(this,        /* 获取图片路径 */
                                               tr("打开图片"),
                                               "../",
                                               tr("图片文件(*bmp *jpg)"));
    qDebug()<<picturePath;
    ui->piturePath->setText(picturePath);
    QImage img(picturePath);
    ui->label->setPixmap(QPixmap::fromImage(img));

    exampImg = new pixmapTrans(picturePath,corlorFilePath);
    exampImg->pixmapFormatTrans();

}

