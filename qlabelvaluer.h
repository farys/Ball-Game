#ifndef QLABELVALUER_H
#define QLABELVALUER_H
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>

#define LETTER_SIZE 8

class QValuer : public QWidget{
public:
    QValuer(QString txt, QWidget * parent = 0) : QWidget(parent){
        label = new QLabel(txt, this);
        label->show();

        lineEdit = new QLineEdit(this);
        lineEdit->show();
        this->setMinimumSize((txt.length()*LETTER_SIZE)+200, 500);
        this->setAttribute(Qt::WA_InvalidSize, true);
    }

    void setValue(int i){
        this->lineEdit->setText(QString::number(i));
    }

    int getValue(){
        return this->lineEdit->text().toInt();
    }

    void move(int x, int y){
        this->lineEdit->move(x + (label->text().length()*LETTER_SIZE), y);
        this->label->move(x, y);
    }

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QString prepend;

};
#endif // QLABELVALUER_H
