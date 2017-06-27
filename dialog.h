#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "QString"
#include "pixmaptrans.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_openPicture_clicked();

    void on_selecteCorlor_clicked();

private:
    Ui::Dialog *ui;
    QString picturePath;
    QString corlorFilePath;
    pixmapTrans *exampImg;
};

#endif // DIALOG_H
