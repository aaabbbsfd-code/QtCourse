#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QKeyEvent>
#include <QJSEngine>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void onNumberClicked();
    void onOpClicked();
    void onEqClicked();
    void onClearClicked();
    void onBackClicked();
    void onLParenClicked();
    void onRParenClicked();
private:
    Ui::MainWindow *ui;
    QString m_expr;
    QJSEngine m_js;
    bool isCalcDone;
    void updateExprUI();
};
#endif // MAINWINDOW_H
