#ifndef IMAGETAB_H
#define IMAGETAB_H

#include <QWidget>

namespace Ui {
class imageTab;
}

class imageTab : public QWidget
{
    Q_OBJECT

public:
    explicit imageTab(QWidget *parent = nullptr);
    ~imageTab();

private:
    Ui::imageTab *ui;
};

#endif // IMAGETAB_H
