#include "wt_projectwidget.h"
#include "ui_wt_projectwidget.h"
#include "newprojectdialog.h"
#include "modelparameter.h" // 引入全局参数类

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QPalette>

WT_ProjectWidget::WT_ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WT_ProjectWidget)
{
    ui->setupUi(this);
    init();
}

WT_ProjectWidget::~WT_ProjectWidget()
{
    delete ui;
}

void WT_ProjectWidget::init()
{
    qDebug() << "初始化项目管理界面...";

    // 设置一个空的顶部样式
    QString topPicStyle = "";
    QString topName = "  ";

    // 设置整体背景为透明
    this->setStyleSheet("background-color: transparent;");
    ui->widget_5->setStyleSheet("background-color: transparent;");

    // 调整按钮之间的间距
    ui->gridLayout_3->setHorizontalSpacing(30);
    ui->gridLayout_3->setVerticalSpacing(10);

    // 创建大字体
    QFont bigFont;
    bigFont.setPointSize(16);
    bigFont.setBold(true);

    // 创建背景颜色
    QColor backgroundColor(148, 226, 255);

    // 按钮样式表
    QString forceStyle = QString(
        "MonitoStateW { "
        "background-color: rgb(148, 226, 255); "
        "border-radius: 10px; "
        "padding: 10px; "
        "} "
        "MonitoStateW * { "
        "background-color: transparent; "
        "} "
        "MonitoStateW:hover { "
        "background-color: rgb(120, 200, 240); "
        "} "
        "QLabel { "
        "color: #333333; "
        "font-weight: bold; "
        "margin-top: 5px; "
        "background-color: transparent; "
        "}"
        );

    // 1. "新建"按钮设置
    QString centerPicStyle = "border-image: url(:/new/prefix1/Resource/Mon1.png);";
    QString bottomName = "新建";
    ui->MonitState1->setTextInfo(centerPicStyle, topPicStyle, topName, bottomName);
    ui->MonitState1->setFixedSize(128, 160);
    ui->MonitState1->setStyleSheet(forceStyle);
    ui->MonitState1->setAutoFillBackground(true);
    QPalette palette1 = ui->MonitState1->palette();
    palette1.setColor(QPalette::Window, backgroundColor);
    ui->MonitState1->setPalette(palette1);
    ui->MonitState1->setFont(bigFont);
    ui->MonitState1->setMouseTracking(true);
    connect(ui->MonitState1, SIGNAL(sigClicked()), this, SLOT(onNewProjectClicked()));

    // 2. "打开"按钮设置
    centerPicStyle = "border-image: url(:/new/prefix1/Resource/Mon2.png);";
    bottomName = "打开";
    ui->MonitState2->setTextInfo(centerPicStyle, topPicStyle, topName, bottomName);
    ui->MonitState2->setFixedSize(128, 160);
    ui->MonitState2->setStyleSheet(forceStyle);
    ui->MonitState2->setAutoFillBackground(true);
    QPalette palette2 = ui->MonitState2->palette();
    palette2.setColor(QPalette::Window, backgroundColor);
    ui->MonitState2->setPalette(palette2);
    ui->MonitState2->setFont(bigFont);
    ui->MonitState2->setMouseTracking(true);
    connect(ui->MonitState2, SIGNAL(sigClicked()), this, SLOT(onOpenProjectClicked()));

    // 3. "关闭"按钮设置 (原读取按钮)
    // 建议替换为关闭图标，若无则暂用 Mon3.png
    centerPicStyle = "border-image: url(:/new/prefix1/Resource/Mon3.png);";
    bottomName = "关闭";
    ui->MonitState3->setTextInfo(centerPicStyle, topPicStyle, topName, bottomName);
    ui->MonitState3->setFixedSize(128, 160);
    ui->MonitState3->setStyleSheet(forceStyle);
    ui->MonitState3->setAutoFillBackground(true);
    QPalette palette3 = ui->MonitState3->palette();
    palette3.setColor(QPalette::Window, backgroundColor);
    ui->MonitState3->setPalette(palette3);
    ui->MonitState3->setFont(bigFont);
    ui->MonitState3->setMouseTracking(true);
    // 断开旧连接并连接到关闭槽函数
    ui->MonitState3->disconnect();
    connect(ui->MonitState3, SIGNAL(sigClicked()), this, SLOT(onCloseProjectClicked()));

    // 4. "退出"按钮设置
    centerPicStyle = "border-image: url(:/new/prefix1/Resource/Mon4.png);";
    bottomName = "退出";
    ui->MonitState4->setTextInfo(centerPicStyle, topPicStyle, topName, bottomName);
    ui->MonitState4->setFixedSize(128, 160);
    ui->MonitState4->setStyleSheet(forceStyle);
    ui->MonitState4->setAutoFillBackground(true);
    QPalette palette4 = ui->MonitState4->palette();
    palette4.setColor(QPalette::Window, backgroundColor);
    ui->MonitState4->setPalette(palette4);
    ui->MonitState4->setFont(bigFont);
    ui->MonitState4->setMouseTracking(true);
    connect(ui->MonitState4, &MonitoStateW::sigClicked, this, [=]() {
        QApplication::quit();
    });
}

// [新增] 辅助函数：统一显示白底黑字的弹窗
int WT_ProjectWidget::showMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text, QMessageBox::StandardButtons buttons)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(icon);
    msgBox.setStandardButtons(buttons);

    // 设置白底黑字样式
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #ffffff; color: #000000; }"
        "QLabel { color: #000000; background-color: transparent; }"
        "QPushButton { "
        "   color: #000000; "
        "   background-color: #f0f0f0; "
        "   border: 1px solid #c0c0c0; "
        "   border-radius: 4px; "
        "   padding: 5px 15px; "
        "   min-width: 60px; "
        "}"
        "QPushButton:hover { background-color: #e0e0e0; }"
        "QPushButton:pressed { background-color: #d0d0d0; }"
        );

    return msgBox.exec();
}

void WT_ProjectWidget::onNewProjectClicked()
{
    // 如果当前已有打开的项目，提示用户先关闭
    if (ModelParameter::instance()->hasLoadedProject()) {
        int res = showMessageBox(QMessageBox::Question, "提示",
                                 "当前已有打开的项目，创建新项目将关闭当前项目。\n是否继续？",
                                 QMessageBox::Yes | QMessageBox::No);

        if (res == QMessageBox::No) return;

        // 执行关闭逻辑
        ModelParameter::instance()->closeProject();
        emit projectClosed();
    }

    qDebug() << "新建按钮被点击...";
    NewProjectDialog* dialog = new NewProjectDialog(this);
    int result = dialog->exec();

    if (result == QDialog::Accepted) {
        qDebug() << "新项目创建成功";

        // 获取新项目数据并更新全局参数
        ProjectData data = dialog->getProjectData();
        ModelParameter::instance()->setParameters(
            data.porosity, data.thickness, data.viscosity,
            data.volumeFactor, data.compressibility, data.productionRate,
            data.wellRadius, data.fullFilePath
            );

        emit newProjectCreated();
    }
    delete dialog;
}

void WT_ProjectWidget::onOpenProjectClicked()
{
    // 如果当前已有打开的项目，提示用户先关闭
    if (ModelParameter::instance()->hasLoadedProject()) {
        int res = showMessageBox(QMessageBox::Question, "提示",
                                 "当前已有打开的项目，打开新项目将关闭当前项目。\n是否继续？",
                                 QMessageBox::Yes | QMessageBox::No);

        if (res == QMessageBox::No) return;

        // 执行关闭逻辑
        ModelParameter::instance()->closeProject();
        emit projectClosed();
    }

    qDebug() << "打开按钮被点击...";
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("打开项目"),
        "",
        tr("WellTest Project (*.pwt)")
        );

    if (filePath.isEmpty()) return;

    // 使用全局参数类加载项目
    if (ModelParameter::instance()->loadProject(filePath)) {
        // 使用统一样式显示成功信息
        showMessageBox(QMessageBox::Information, "成功", "项目已成功打开！");

        // 发送信号通知 MainWindow 解锁其他功能
        emit newProjectCreated();
    } else {
        // 使用统一样式显示错误信息
        showMessageBox(QMessageBox::Critical, "错误", "无法打开或解析项目文件。");
    }
}

void WT_ProjectWidget::onCloseProjectClicked()
{
    qDebug() << "关闭按钮被点击...";

    // 1. 检查是否有项目打开
    if (!ModelParameter::instance()->hasLoadedProject()) {
        showMessageBox(QMessageBox::Information, "提示", "当前没有打开的项目。");
        return;
    }

    // 2. 询问是否保存
    int res = showMessageBox(QMessageBox::Question, "关闭项目",
                             "是否在关闭前保存当前项目更改？\n选择'Yes'保存并关闭，'No'直接关闭，'Cancel'放弃操作。",
                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (res == QMessageBox::Cancel) return;

    if (res == QMessageBox::Yes) {
        if (ModelParameter::instance()->saveProject()) {
            showMessageBox(QMessageBox::Information, "保存成功", "项目已成功保存。");
        } else {
            showMessageBox(QMessageBox::Critical, "保存失败", "无法保存项目文件。");
            return; // 保存失败则中止关闭
        }
    }

    // 3. 执行关闭操作
    ModelParameter::instance()->closeProject();

    // 4. 发送信号通知主界面重置状态
    emit projectClosed();

    showMessageBox(QMessageBox::Information, "已关闭", "项目已关闭，数据已重置。");
}
