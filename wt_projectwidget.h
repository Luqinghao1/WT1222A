#ifndef WT_PROJECTWIDGET_H
#define WT_PROJECTWIDGET_H

#include <QWidget>
#include <QString>
#include <QMessageBox> // [新增] 引入 QMessageBox
#include "newprojectdialog.h"

// 项目管理界面
namespace Ui {
class WT_ProjectWidget;
}

class WT_ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WT_ProjectWidget(QWidget *parent = nullptr);
    ~WT_ProjectWidget();

    // 初始化界面
    void init();

    // 创建新项目（供外部调用）
    void createNewProject();

signals:
    // 新项目创建或打开成功信号 (用于通知 MainWindow 解锁功能)
    void newProjectCreated();

    // 项目已关闭信号 (用于通知 MainWindow 禁用其他功能)
    void projectClosed();

    // (保留兼容) 文件读取成功信号
    void fileLoaded(const QString& filePath, const QString& fileType);

private slots:
    // 点击"新建"按钮的槽函数
    void onNewProjectClicked();

    // 点击"打开"按钮的槽函数
    void onOpenProjectClicked();

    // 点击"关闭"按钮的槽函数
    void onCloseProjectClicked();

private:
    Ui::WT_ProjectWidget *ui;

    // [新增] 辅助函数：显示带有统一白色背景样式的消息框
    // 参数:
    //   icon: 图标类型 (Information, Question, Warning, Critical)
    //   title: 窗口标题
    //   text: 消息内容
    //   buttons: 按钮组合 (默认只有一个 Ok 按钮)
    // 返回值: 用户点击的按钮 StandardButton 值
    int showMessageBox(QMessageBox::Icon icon, const QString& title, const QString& text,
                       QMessageBox::StandardButtons buttons = QMessageBox::Ok);
};

#endif // WT_PROJECTWIDGET_H
