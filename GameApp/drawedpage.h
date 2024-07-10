#ifndef DRAWEDPAGE_H
#define DRAWEDPAGE_H

#include <QDialog>

namespace Ui {
class drawedPage;
}

class drawedPage : public QDialog
{
    Q_OBJECT

public:
    explicit drawedPage(QWidget *parent = nullptr);
    ~drawedPage();

private:
    Ui::drawedPage *ui;
};

#endif // DRAWEDPAGE_H
