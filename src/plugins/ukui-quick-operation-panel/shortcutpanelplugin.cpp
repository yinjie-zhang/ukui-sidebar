/*
* Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
*
*/
#include "shortcutpanelplugin.h"

shortcutPanelPlugin::shortcutPanelPlugin(QObject *parent)
{
    Q_UNUSED(parent);
    initMemberVariables();         // 初始化插件成员变量
    initShortButtonWidget();       // 初始化8个快捷按钮界面
    initShortcutButtonGsetting();  // 初始化记住上次编辑按钮的gsetting值
    setButtonIcon();               // 设置按钮图标
    initsetShortWidget();          // 布局快捷按钮界面
    setWidget();                   // 将切换按钮和ListView界面set进插件主界面
}

/* 初始化插件成员变量 */
void shortcutPanelPlugin::initMemberVariables()
{
    m_pMainVLayout = new QVBoxLayout;
    m_pMainVLayout->setContentsMargins(0, 0, 0, 0);
    m_pMainVLayout->setSpacing(0);

    m_pButtonHLaout = new QHBoxLayout;
    m_pButtonHLaout->setContentsMargins(0, 0, 0, 0);
    m_pButtonHLaout->setSpacing(0);

    m_pShortGLayout  = new QGridLayout;
    m_pShortGLayout->setContentsMargins(32, 16, 18, 31);
    m_pShortGLayout->setHorizontalSpacing(35);
    m_pShortGLayout->setVerticalSpacing(16);

    m_pMainWidget  = new MainWidget;
    m_pMainWidget->setContentsMargins(0, 0, 0, 0);
    m_pMainWidget->setFixedSize(392, 486);
    m_pButtonWidget = new QWidget;
    m_pButtonWidget->setContentsMargins(0, 0, 0, 0);
    m_pButtonWidget->setFixedHeight(15);
    connect(m_pMainWidget, &MainWidget::EditOptionSignal, this, &shortcutPanelPlugin::ShowEditWidgetSlots);

    /* 账户信息界面 */
    m_PAccountInfoWidget = new AccountInformation();
    m_PAccountInfoWidget->setFixedSize(392, 48);

    /* 快捷操作面板界面 */
    m_pShortWidget   = new QWidget;
    m_pShortWidget->setFixedWidth(380);
    m_pShortWidget->setContentsMargins(0, 0, 0, 0);

    /* 调整音量与屏幕亮度界面 */
    m_pScrollingAreaWidget = new ScrollingAreaWidget();

    /* 显示天气界面 */
    m_pWeatherWidget = new weatherWidget();
    m_pWeatherWidget->setFixedSize(392, 50);

    m_SpreadButtonIconList << SPREAD_BUTTON_NORMAL << SPREAD_BUTTON_HOVER << SPREAD_BUTTON_PRESS;
    m_FoldButtonIconList   << FOLD_BUTTON_NORMAL   << FOLD_BUTTON_HOVER   << FOLD_BUTTON_PRESS;

    m_SpreadButtonIconNameList << SPREAD_BUTTON_NORMAL_NAME << SPREAD_BUTTON_HOVER_NAME << SPREAD_BUTTON_PRESS_NAME;
    m_FoldButtonIconNameList   << FOLD_BUTTON_NORMAL_NAME << FOLD_BUTTON_HOVER_NAME << FOLD_BUTTON_PRESS_NAME;

    m_pSpreadButton  = new PushButton(m_SpreadButtonIconList, m_SpreadButtonIconNameList);
    connect(m_pSpreadButton, &QPushButton::clicked, this, &shortcutPanelPlugin::spreadClikedSlots);
    m_pSpreadButton->setFixedSize(45, 15);

    m_pfoldButton = new PushButton(m_FoldButtonIconList, m_FoldButtonIconNameList);
    connect(m_pfoldButton, &QPushButton::clicked, this, &shortcutPanelPlugin::foldClikedSlots);
    m_pfoldButton->setFixedSize(45, 15);
    return;
}

/* 初始化8个快捷按钮界面 */
void shortcutPanelPlugin::initShortButtonWidget()
{
    /* 清空链表中的数据，初始化 */
    ShortButtonWidgetList.clear();

    /* 平板模式 */
    m_ppadWidget       = new padWidget();
    ShortButtonWidgetList.append(m_ppadWidget);
    m_pButtonGsettingValue.append("padwidgetvalue");
    connect(m_ppadWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("padwidgetvalue");
    });

    /* 免打扰 */
    m_pnodisturbWidget = new nodisturbWidget();
    ShortButtonWidgetList.append(m_pnodisturbWidget);
    m_pButtonGsettingValue.append("nodisturbwidgetvalue");
    connect(m_pnodisturbWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("nodisturbwidgetvalue");
    });

    /* 节能模式 */
    m_pPowerSavingMode = new powerSavingMode();
    ShortButtonWidgetList.append(m_pPowerSavingMode);
    m_pButtonGsettingValue.append("powersavingmodevalue");
    connect(m_pPowerSavingMode->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("powersavingmodevalue");
    });

    /* 护眼模式 */
    m_pEyeProtectionMode = new eyeProtectionMode();
    ShortButtonWidgetList.append(m_pEyeProtectionMode);
    m_pButtonGsettingValue.append("eyeprotectionmodevalue");
    connect(m_pEyeProtectionMode->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("eyeprotectionmodevalue");
    });

    /* 蓝牙 */
    m_pbluetoothWidget = new bluetoothWidget();
    ShortButtonWidgetList.append(m_pbluetoothWidget);
    m_pButtonGsettingValue.append("bluetoothwidgetvalue");
    connect(m_pbluetoothWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("bluetoothwidgetvalue");
    });

    /* 热点 */
    m_photspotWidget = new hotspotWidget();
    ShortButtonWidgetList.append(m_photspotWidget);
    m_pButtonGsettingValue.append("hotspotwidgetvalue");
    connect(m_photspotWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("hotspotwidgetvalue");
    });

    /* VPN */
    m_pDiturbModeWidget = new diturbModeWidget();
    ShortButtonWidgetList.append(m_pDiturbModeWidget);
    m_pButtonGsettingValue.append("diturbmodewidgetvalue");
    connect(m_pDiturbModeWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("diturbmodewidgetvalue");
    });

    /* 设置 */
    m_psettingWidget = new settingWidget();
    ShortButtonWidgetList.append(m_psettingWidget);
    m_pButtonGsettingValue.append("settingwidgetvalue");
    connect(m_psettingWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("settingwidgetvalue");
    });

    /* 计算器 */
    m_pcalculatorWidget = new calculatorWidget();
    ShortButtonWidgetList.append(m_pcalculatorWidget);
    m_pButtonGsettingValue.append("calculatorwidgetvalue");
    connect(m_pcalculatorWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("calculatorwidgetvalue");
    });

    /* 截图 */
    m_pscreenshotWidget = new screenshotWidget();
    ShortButtonWidgetList.append(m_pscreenshotWidget);
    m_pButtonGsettingValue.append("screenshotwidgetvalue");
    connect(m_pscreenshotWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("screenshotwidgetvalue");
    });

    /* 闹钟 */
    m_pAlarmButtonInterface = new alarmButtonInterface();
    ShortButtonWidgetList.append(m_pAlarmButtonInterface);
    m_pButtonGsettingValue.append("alarmbuttoninterfacevalue");
    connect(m_pAlarmButtonInterface->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("alarmbuttoninterfacevalue");
    });

    /* 便签本 */
    m_pNoteBookButtonWidget = new notebookButtonWidget();
    ShortButtonWidgetList.append(m_pNoteBookButtonWidget);
    m_pButtonGsettingValue.append("notebookbuttonwidgetvalue");
    connect(m_pNoteBookButtonWidget->m_pDeleteButton, &QPushButton::clicked, this, [=](){
        setCanceGsettingButtonValue("notebookbuttonwidgetvalue");
    });

    /* 自动旋转功能 */
//    m_pAutomaticRotationWidget = new AutomaticRotationWidget();
//    ShortButtonWidgetList.append(m_pAutomaticRotationWidget);

    /* Wifi */
//    m_pWifiWidget = new WifiWidget();
//    ShortButtonWidgetList.append(m_pWifiWidget);
    return;
}

/* 初始化快捷操作按钮gsetting值 */
void shortcutPanelPlugin::initShortcutButtonGsetting()
{
    const QByteArray id(SHORTCUT_BUTTON_GSETTING_PATH);
    if(QGSettings::isSchemaInstalled(id))
        m_pGsettingShutcutValue = new QGSettings(id);
    if (m_pGsettingShutcutValue != nullptr)
        connect(m_pGsettingShutcutValue, &QGSettings::changed, this, &shortcutPanelPlugin::resetShortWidget);
    return;
}

/* 布局8个快捷方式的按钮, 初始化 */
void shortcutPanelPlugin::initsetShortWidget()
{
    int tmpx = 0, tmpy = 0;
#ifdef BUILD_TARGET
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 4; k++) {
            if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(4 * j + k)).toBool()) {
                m_pShortGLayout->addWidget(ShortButtonWidgetList.at(4 * j + k), tmpx, tmpy, 1, 1);
                tmpy++;
                if (tmpy == 4) {
                    tmpy = 0;
                    tmpx++;
                }
            }
        }
    }
#else
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 2; k++) {
            if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(2 * j + k)).toBool()) {
                m_pShortGLayout->addWidget(ShortButtonWidgetList.at(2 * j + k), tmpx, tmpy, 1, 1);
                tmpy++;
                if (tmpy == 2) {
                    tmpy = 0;
                    tmpx++;
                }
            }
        }
    }
    m_pShortGLayout->addWidget(m_pScrollingAreaWidget, 0, 3, 3, 2); // 当在通用平板模式下， 滑动条区域所占位置3行2列， 和设计稿大小需要细调
#endif
    return;
}

/* 设置按钮图标 */
void shortcutPanelPlugin::setButtonIcon()
{
    /* 设置展开按钮悬浮和点击状态按钮图片 */
    m_pSpreadButton->setIcon(QIcon(":/image/open-normal.svg"));

    /* 设置折叠按钮悬浮和点击状态按钮图片 */
    m_pfoldButton->setIcon(QIcon(":/image/fold-normal.svg"));
    return;
}

/* 将切换按钮和ListView界面set进插件主界面 */
void shortcutPanelPlugin::setWidget()
{
    m_pButtonHLaout->addItem(new QSpacerItem(20, 173));
    m_pButtonHLaout->addWidget(m_pSpreadButton);
    m_pButtonHLaout->addWidget(m_pfoldButton);
    m_pButtonWidget->setLayout(m_pButtonHLaout);
    m_pSpreadButton->setVisible(false);

    m_pShortWidget->setLayout(m_pShortGLayout);

    m_pMainVLayout->addWidget(m_pButtonWidget);
    m_pMainVLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Fixed));

    m_pMainVLayout->addWidget(m_PAccountInfoWidget);        // 账户信息
    m_pMainVLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Fixed));
    m_pLinelabel_1 = new QLabel();
    m_pLinelabel_1->setFixedSize(392, 1);
    m_pLinelabel_1->setStyleSheet("QLabel{border: 1px solid rgba(246, 246, 246, 1)};");
    m_pMainVLayout->addWidget(m_pLinelabel_1);

    m_pMainVLayout->addWidget(m_pShortWidget);
#ifdef BUILD_TARGET
    m_pLinelabel_2 = new QLabel();
    m_pLinelabel_2->setFixedSize(392, 1);
    m_pLinelabel_2->setStyleSheet("QLabel{border: 1px solid rgba(246, 246, 246, 1)};");
    m_pMainVLayout->addWidget(m_pLinelabel_2);
    m_pMainVLayout->addWidget(m_pScrollingAreaWidget);      // 滚动条区域
#else

#endif

    m_pLinelabel_3 = new QLabel();
    m_pLinelabel_3->setFixedSize(392, 1);
    m_pLinelabel_3->setStyleSheet("QLabel{border: 1px solid rgba(246, 246, 246, 1)};");
    m_pMainVLayout->addWidget(m_pLinelabel_3);

    m_pMainVLayout->addWidget(m_pWeatherWidget);            // 天气界面
    m_pMainVLayout->setSpacing(0);
    m_pMainWidget->setLayout(m_pMainVLayout);
    return;
}

/* 获取快捷操作面板界面 */
QWidget* shortcutPanelPlugin::getShortCutPanelWidget()
{
    if (m_pMainWidget == nullptr) {
        return nullptr;
    }
    return m_pMainWidget;
}

/* 获取接口代号 */
InterfaceEnum shortcutPanelPlugin::getInterfaceMark(QString key)
{
    if ("" == key) {
        qDebug() << "获取接口代号失败， 传入参数有误";
        return InterfaceEnum::Else;
    }
    if (m_InterfaceHash.contains(key)) {
        return m_InterfaceHash.value(key);
    } else
        return InterfaceEnum::Else;
}

/* 移除Hash中接口 */
void shortcutPanelPlugin::removeInterfaceMark(QString key)
{
    if ("" == key) {
        qDebug() << "移除Hash中接口失败， 传入参数有误";
        return;
    }
    if (m_InterfaceHash.contains(key))
        m_InterfaceHash.remove(key);
    return;
}

/* 插入接口和代号往Hash表中 */
void shortcutPanelPlugin::insertInterfaceMark(QString key, InterfaceEnum value)
{
    if ("" == key || value < 0) {
        qDebug() << "插入数据失败， 传入参数有误";
        return;
    }
    if (m_InterfaceHash.contains(key)) {
        qDebug() << "此Key值已存在";
        return;
    }
    m_InterfaceHash.insert(key, value);
    return;
}

void shortcutPanelPlugin::setGridLayoutWidgetHide()
{
    for (int i = 1; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (m_pShortGLayout->itemAtPosition(i, j) == nullptr) {
                return;
            } else {
                m_pShortGLayout->itemAtPosition(i, j)->widget()->setVisible(false);
            }
        }
    }
}

void shortcutPanelPlugin::setGridLayoutWidgetShow()
{
    for (int i = 1; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (m_pShortGLayout->itemAtPosition(i, j) == nullptr) {
                return;
            } else {
                m_pShortGLayout->itemAtPosition(i, j)->widget()->setVisible(true);
            }
        }
    }
}

/* 展开按钮槽函数 */
void shortcutPanelPlugin::spreadClikedSlots()
{
    m_pSpreadButton->setVisible(false);
    m_pfoldButton->setVisible(true);
    setGridLayoutWidgetShow();
    m_pScrollingAreaWidget->setVisible(true);
    m_pWeatherWidget->setVisible(true);
    m_pShortWidget->setFixedHeight(297);
    m_pMainWidget->setFixedSize(392, 486);
    m_pLinelabel_2->setVisible(true);
    m_pLinelabel_3->setVisible(true);
    m_pMainWidget->update();
    return;
}

/* 折叠按钮槽函数 */
void shortcutPanelPlugin::foldClikedSlots()
{
    m_pSpreadButton->setVisible(true);
    m_pfoldButton->setVisible(false);
    int height = m_pMainWidget->height();
    int width  = m_pMainWidget->width();
    setGridLayoutWidgetHide();
    m_pShortWidget->setFixedHeight(102);
    m_pMainWidget->setFixedSize(width, 184);
    m_pScrollingAreaWidget->setVisible(false);
    m_pWeatherWidget->setVisible(false);
    m_pLinelabel_2->setVisible(false);
    m_pLinelabel_3->setVisible(false);
    m_pMainWidget->update();
    return;
}

/* 点击编辑按钮时出现编辑界面 */
void shortcutPanelPlugin::ShowEditWidgetSlots()
{
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(0)).toBool()) {
        padWidget *w = dynamic_cast<padWidget *> (ShortButtonWidgetList.at(0));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(1)).toBool()) {
        nodisturbWidget *w = dynamic_cast<nodisturbWidget *> (ShortButtonWidgetList.at(1));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(2)).toBool()) {
        powerSavingMode *w = dynamic_cast<powerSavingMode *> (ShortButtonWidgetList.at(2));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(3)).toBool()) {
        eyeProtectionMode *w = dynamic_cast<eyeProtectionMode *> (ShortButtonWidgetList.at(3));
        w->m_pDeleteButton->setVisible(true);
    }

    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(4)).toBool()) {
        bluetoothWidget *w = dynamic_cast<bluetoothWidget *> (ShortButtonWidgetList.at(4));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(5)).toBool()) {
        hotspotWidget *w = dynamic_cast<hotspotWidget *> (ShortButtonWidgetList.at(5));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(6)).toBool()) {
        diturbModeWidget *w = dynamic_cast<diturbModeWidget *> (ShortButtonWidgetList.at(6));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(7)).toBool()) {
        settingWidget *w = dynamic_cast<settingWidget *> (ShortButtonWidgetList.at(7));
        w->m_pDeleteButton->setVisible(true);
    }

    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(8)).toBool()) {
        calculatorWidget *w = dynamic_cast<calculatorWidget *> (ShortButtonWidgetList.at(8));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(9)).toBool()) {
        screenshotWidget *w = dynamic_cast<screenshotWidget *> (ShortButtonWidgetList.at(9));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(10)).toBool()) {
        alarmButtonInterface *w = dynamic_cast<alarmButtonInterface *> (ShortButtonWidgetList.at(10));
        w->m_pDeleteButton->setVisible(true);
    }
    if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(11)).toBool()) {
        notebookButtonWidget *w = dynamic_cast<notebookButtonWidget *> (ShortButtonWidgetList.at(11));
        w->m_pDeleteButton->setVisible(true);
    }
    return;
}

/* 当收到控制面板发出的gsetting信号值变化时，重新布局按钮界面 */
void shortcutPanelPlugin::resetShortWidget()
{
    int tmpx = 0, tmpy = 0;
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 4; k++) {
            if (m_pGsettingShutcutValue->get(m_pButtonGsettingValue.at(4 * j + k)).toBool()) {
                m_pShortGLayout->addWidget(ShortButtonWidgetList.at(4 * j + k), tmpx, tmpy, 1, 1);
                tmpy++;
                if (tmpy == 4) {
                    tmpy = 0;
                    tmpx++;
                }
            }
        }
    }
    m_pShortWidget->update();
    return;
}

/* 点击取消固定按钮时，修改其gsetting值 */
void shortcutPanelPlugin::setCanceGsettingButtonValue(QString key)
{
    if (m_pGsettingShutcutValue != nullptr && m_pGsettingShutcutValue->keys().contains(key)) {
        m_pGsettingShutcutValue->set(key, false);
    }
    return;
}
