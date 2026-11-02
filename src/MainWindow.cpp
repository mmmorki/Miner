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

using namespace MainWindowEnums;

void MainWindow::gameOverVictorySlot()
{
    m_infoBar->setCurrentIndex(Victory);
}

void MainWindow::gameOverDefeatSlot()
{
    m_infoBar->setCurrentIndex(Defeat);
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

    , m_gamePage{ new QWidget{ m_centralStack } }
    , m_gameLayout{ new QHBoxLayout{ m_gamePage } }
    , m_gameField{ new Field{ m_gamePage } }
    , m_gameView{ new QGraphicsView{ m_gamePage } }
    , m_infoBar{ new QStackedWidget{ m_gamePage } }

    , m_infoBarStatisticsPage{ new QWidget{ m_infoBar } }
    , m_infoBarStatisticsLayout{ new QGridLayout{ m_infoBarStatisticsPage } }

    , m_infoBarVictoryPage{ new QWidget{ m_infoBar } }
    , m_infoBarVictoryLayout{ new QGridLayout{ m_infoBarVictoryPage } }
    , m_infoBarVictoryBtn{ new QPushButton{ "Заебись", m_infoBarVictoryPage } }

    , m_infoBarDefeatPage{ new QWidget{ m_infoBar } }
    , m_infoBarDefeatLayout{ new QGridLayout{ m_infoBarDefeatPage } }
    , m_infoBarDefeatBtn{ new QPushButton{ "Похуй", m_infoBarDefeatPage } }
{
    setupCentral();
    setupMainPage();
    setupSettingsPage();
    setupGamePage();
    setupInfoBar();
}

void MainWindow::startGame() const
{
    const int fieldWidth{ toInt(m_fieldWidthSettingLineEdit->text()) };
    const int fieldHeight{ toInt(m_fieldHeightSettingLineEdit->text()) };
    const int minesCount{ toInt(m_minesCountSettingLineEdit->text()) };

    m_gameField->setFieldSizeCells(fieldWidth, fieldHeight);
    m_gameField->setMinesCount(minesCount);
    m_gameField->generate();
    m_gameField->unblock();

    m_centralStack->setCurrentIndex(Game);
    m_infoBar->setCurrentIndex(Statistics);
}

void MainWindow::endGame() const
{
    m_gameField->clearData();
    m_centralStack->setCurrentIndex(Main);
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
            startGame();

            m_fieldWidthSettingLineEdit->setText(
                Constants::DEFAULT_FIELD_WIDTH_CELLS_STR);
            m_fieldHeightSettingLineEdit->setText(
                Constants::DEFAULT_FIELD_HEIGHT_CELLS_STR);
            m_minesCountSettingLineEdit->setText(
                Constants::DEFAULT_MINES_COUNT_STR);
        }
    };
    connect(m_acceptSettingsBtn, &QPushButton::clicked, acceptSettingsBtnLambda);
}

void MainWindow::setupGamePage() const
{
    m_centralStack->insertWidget(Game, m_gamePage);

    m_gameLayout->addWidget(m_gameView, 4);
    m_gameView->setMinimumSize(800, 600);
    m_gameView->setScene(m_gameField);

    m_gameLayout->addWidget(m_infoBar, 1);
    m_infoBar->setMinimumSize(200, 600);

    connect(m_gameField, &Field::gameOverVictorySignal,
        this, &MainWindow::gameOverVictorySlot);
    connect(m_gameField, &Field::gameOverDefeatSignal,
        this, &MainWindow::gameOverDefeatSlot);
}

void MainWindow::setupInfoBar() const
{
    m_infoBar->insertWidget(Statistics, m_infoBarStatisticsPage);

    m_infoBar->insertWidget(Victory, m_infoBarVictoryPage);
    m_infoBarVictoryLayout->addWidget(m_infoBarVictoryBtn, 0, 0, 1, 1,
        Qt::AlignCenter);
    connect(m_infoBarVictoryBtn, &QPushButton::clicked,
        this, &MainWindow::endGame);

    m_infoBar->insertWidget(Defeat, m_infoBarDefeatPage);
    m_infoBarDefeatLayout->addWidget(m_infoBarDefeatBtn, 0, 0, 1, 1,
        Qt::AlignCenter);
    connect(m_infoBarDefeatBtn, &QPushButton::clicked,
        this, &MainWindow::endGame);
}