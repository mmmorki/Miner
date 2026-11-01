#include "Enums.h"
#include "Constants.h"
#include "Functions.h"

#include "MainWindow.h"

#include "Field.h"

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>

void MainWindow::endGameSlot()
{
    m_centralStack->setCurrentIndex(Main);
}

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
    , m_fieldWidthSettingLineEdit{
        new QLineEdit{ Constants::DEFAULT_FIELD_WIDTH_CELLS_STR,
                       m_settingsPage } }
    , m_fieldHeightSettingLabel{ new QLabel{ "Высота поля:", m_settingsPage } }
    , m_fieldHeightSettingLineEdit{
        new QLineEdit{ Constants::DEFAULT_FIELD_HEIGHT_CELLS_STR,
                       m_settingsPage } }
    , m_minesCountSettingLabel{
        new QLabel{ "Количество мин:", m_settingsPage } }
    , m_minesCountSettingLineEdit{
        new QLineEdit{ Constants::DEFAULT_MINES_COUNT_STR, m_settingsPage } }
    , m_acceptSettingsBtn{
        new QPushButton{"Принять настройки", m_settingsPage } }

    , m_gameField{ new Field{ m_centralStack } }
    , m_gameView{ new QGraphicsView{ m_centralStack } }
{
    setupCentral();
    setupMainPage();
    setupSettingsPage();
    setupGameScene();
}

void MainWindow::setupCentral()
{
    setCentralWidget(m_central);
    m_centralLayout->addWidget(m_centralStack);
}

void MainWindow::setupMainPage()
{
    m_centralStack->insertWidget(Main, m_mainPage);

    m_mainLayout->addWidget(m_singleplayerBtn, 0, 0, 1, 1, Qt::AlignCenter);
    m_mainLayout->addWidget(m_multiplayerBtn, 1, 0, 1, 1, Qt::AlignCenter);

    auto singleplayerBtnLambda{
        [this] {
            m_centralStack->setCurrentIndex(Settings);
        }
    };
    connect(m_singleplayerBtn, &QPushButton::clicked, singleplayerBtnLambda);

    auto multiplayerBtnLambda{
        [this] {
            QMessageBox::warning(this, "", Constants::OBJECT_IN_DEV_STRING);
        }
    };
    connect(m_multiplayerBtn, &QPushButton::clicked, multiplayerBtnLambda);
}

void MainWindow::setupSettingsPage()
{
    m_centralStack->insertWidget(Settings, m_settingsPage);

    m_settingsLayout->addWidget(m_fieldWidthSettingLabel, 0, 0, 1, 1);
    m_settingsLayout->addWidget(m_fieldWidthSettingLineEdit, 0, 1, 1, 1);
    m_settingsLayout->addWidget(m_fieldHeightSettingLabel, 1, 0, 1, 1);
    m_settingsLayout->addWidget(m_fieldHeightSettingLineEdit, 1, 1, 1, 1);
    m_settingsLayout->addWidget(m_minesCountSettingLabel, 2, 0, 1, 1);
    m_settingsLayout->addWidget(m_minesCountSettingLineEdit, 2, 1, 1, 1);
    m_settingsLayout->addWidget(m_acceptSettingsBtn, 3, 0, 1, 2);

    auto acceptSettingsBtnLambda{
        [this] {
            const int fieldWidth{ toInt(m_fieldWidthSettingLineEdit->text()) };
            const int fieldHeight{ toInt(m_fieldHeightSettingLineEdit->text()) };
            const int minesCount{ toInt(m_minesCountSettingLineEdit->text()) };

            m_fieldWidthSettingLineEdit->setText(
                Constants::DEFAULT_FIELD_WIDTH_CELLS_STR);
            m_fieldHeightSettingLineEdit->setText(
                Constants::DEFAULT_FIELD_HEIGHT_CELLS_STR);
            m_minesCountSettingLineEdit->setText(
                Constants::DEFAULT_MINES_COUNT_STR);

            m_gameField->setFieldSizeCells(fieldWidth, fieldHeight);
            m_gameField->setMinesCount(minesCount);
            m_gameField->generate();

            m_centralStack->setCurrentIndex(Game);
        }
    };
    connect(m_acceptSettingsBtn, &QPushButton::clicked, acceptSettingsBtnLambda);
}

void MainWindow::setupGameScene() const
{
    m_centralStack->insertWidget(Game, m_gameView);

    m_gameView->setScene(m_gameField);

    connect(m_gameField, &Field::endGameSignal, this, &MainWindow::endGameSlot);
}