#ifndef GETIPPAGE_H
#define GETIPPAGE_H

#include <QDialog>

namespace Ui {
class GetIPpage;
}

class GetIPpage : public QDialog
{
    Q_OBJECT

public:
    explicit GetIPpage(QWidget *parent = nullptr);
    ~GetIPpage();

private slots:
    void on_ipInput_editingFinished();

    void on_submitBtn_clicked();

signals:
    bool ipAddress(QString ip);
private:
    Ui::GetIPpage *ui;
};

#endif // GETIPPAGE_H
