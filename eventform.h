#ifndef EVENTFORM_H
#define EVENTFORM_H

#include <QWidget>

namespace Ui {
class eventForm;
}

class eventForm : public QWidget
{
    Q_OBJECT

public:
    explicit eventForm(QWidget *parent = 0);
    void clear();
    ~eventForm();

private:
    Ui::eventForm *ui;

private slots:
    void on_pushButton_clicked();

public slots:
    void append(QString str);

signals:
    void acceptSignal();

};

#endif // EVENTFORM_H
