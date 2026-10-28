#include "Enums.h"
#include "Constants.h"

#include "MainWindow.h"

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{ parent }
    , m_central{ new QWidget{ this } }
    , m_centralLayout{ new QHBoxLayout{ m_central } }
    , m_centralStack{ new QStackedWidget{ m_central } }

    , m_mainPage{ new QWidget{ m_centralStack } }
    , m_mainLayout{ new QGridLayout{ m_mainPage } }
    , m_singleplayerBtn{ new QPushButton{ "Одиночная игра", m_mainPage } }
    , m_multiplayerBtn{ new QPushButton{ "Совместная игра", m_mainPage } }

    , m_settingsPage{ new QWidget{ m_centralStack } }
    , m_settingsLayout{ new QGridLayout{ m_settingsPage } }
    , m_fieldWidthSettingLabel{ new QLabel{ "Ширина поля:", m_settingsPage } }
    , m_fieldWidthSettingLineEdit{ new QLineEdit{ "10", m_settingsPage } }
    , m_fieldHeightSettingLabel{ new QLabel{ "Высота поля:", m_settingsPage } }
    , m_fieldHeightSettingLineEdit{ new QLineEdit{ "10", m_settingsPage } }
    , m_minesCountSettingLabel{
        new QLabel{ "Количество мин:", m_settingsPage } }
    , m_minesCountSettingLineEdit{ new QLineEdit{ "20", m_settingsPage } }
    , m_acceptSettingsBtn{
        new QPushButton{"Принять настройки", m_settingsPage } }
{
    setupCentral();
    setupMainPage();
    setupSettingsPage();
}

void MainWindow::setupCentral()
{
    setCentralWidget(m_central);
    m_centralLayout->addWidget(m_centralStack);
}

void MainWindow::setupMainPage()
{
    m_centralStack->insertWidget(MainWindowEnums::Main, m_mainPage);

    m_mainLayout->addWidget(m_singleplayerBtn, 0, 0, 1, 1, Qt::AlignCenter);
    m_mainLayout->addWidget(m_multiplayerBtn, 1, 0, 1, 1, Qt::AlignCenter);

    auto singleplayerBtnLambda{
        [this] {
            m_centralStack->setCurrentIndex(MainWindowEnums::Settings);
        }
    };
    connect(m_singleplayerBtn, &QPushButton::clicked, singleplayerBtnLambda);

    auto multiplayerBtnLambda{
        [this] {
            QMessageBox::warning(this, "", DevConstants::OBJECT_IN_DEV_STRING);
        }
    };
    connect(m_multiplayerBtn, &QPushButton::clicked, multiplayerBtnLambda);
}

void MainWindow::setupSettingsPage()
{
    m_centralStack->insertWidget(MainWindowEnums::Settings, m_settingsPage);

    m_settingsLayout->addWidget(m_fieldWidthSettingLabel, 0, 0, 1, 1);
    m_settingsLayout->addWidget(m_fieldWidthSettingLineEdit, 0, 1, 1, 1);
    m_settingsLayout->addWidget(m_fieldHeightSettingLabel, 1, 0, 1, 1);
    m_settingsLayout->addWidget(m_fieldHeightSettingLineEdit, 1, 1, 1, 1);
    m_settingsLayout->addWidget(m_minesCountSettingLabel, 2, 0, 1, 1);
    m_settingsLayout->addWidget(m_minesCountSettingLineEdit, 2, 1, 1, 1);
    m_settingsLayout->addWidget(m_acceptSettingsBtn, 3, 0, 1, 2);
}