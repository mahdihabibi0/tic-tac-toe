#ifndef LOSERPAGE_H
#define LOSERPAGE_H

#include <QDialog>

namespace Ui {
class loserPage;
}

class loserPage : public QDialog
{
    Q_OBJECT

public:
    explicit loserPage(QWidget *parent = nullptr);
    ~loserPage();

private:
    Ui::loserPage *ui;
};

#endif // LOSERPAGE_H
