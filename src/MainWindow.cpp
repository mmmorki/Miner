#include "Enums.h"
#include "Constants.h"
#include "Functions.h"

#include "MainWindow.h"

#include "Field.h"
#include "Server.h"
#include "Client.h"

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>

void MainWindow::gameOverVictorySlot()
{
    m_infoBar->setCurrentIndex(MainWindowEnums::Victory);
}

void MainWindow::gameOverDefeatSlot()
{
    m_infoBar->setCurrentIndex(MainWindowEnums::Defeat);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{ parent }
    , m_server{ new Server{ this } }
    , m_client{ new Client{ this } }

    , m_central{ new QWidget{ this } }
    , m_centralLayout{ new QHBoxLayout{ m_central } }
    , m_centralStack{ new QStackedWidget{ m_central } }

    , m_mainPage{ new QWidget{ m_centralStack } }
    , m_mainLayout{ new QVBoxLayout{ m_mainPage } }
    , m_singleplayerBtn{ new QPushButton{ "Одиночная игра", m_mainPage } }
    , m_multiplayerHostBtn{ new QPushButton{ "Создать игру", m_mainPage } }
    , m_multiplayerClientBtn{
        new QPushButton{"Присоединиться к игре", m_mainPage } }

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

    , m_connectionPage{ new QWidget{ m_centralStack } }
    , m_connectionLayout{ new QGridLayout{ m_connectionPage } }
    , m_connectionIpLabel{ new QLabel{ "IP-адрес хоста:", m_connectionPage } }
    , m_connectionIpLineEdit{ new QLineEdit{ m_connectionPage } }
    , m_connectionAcceptBtn{ new QPushButton{ "Подключиться", m_connectionPage } }

    , m_gamePage{ new QWidget{ m_centralStack } }
    , m_gameLayout{ new QHBoxLayout{ m_gamePage } }
    , m_gameField{ new Field{ m_gamePage } }
    , m_gameView{ new QGraphicsView{ m_gamePage } }
    , m_infoBar{ new QStackedWidget{ m_gamePage } }

    , m_infoBarStatisticsPage{ new QWidget{ m_infoBar } }
    , m_infoBarStatisticsLayout{ new QGridLayout{ m_infoBarStatisticsPage } }

    , m_infoBarVictoryPage{ new QWidget{ m_infoBar } }
    , m_infoBarVictoryLayout{ new QGridLayout{ m_infoBarVictoryPage } }
    , m_infoBarVictoryToMenuBtn{ new QPushButton{ "Меню", m_infoBarVictoryPage } }
    , m_infoBarVictoryRestartBtn{ new QPushButton{ "Играть снова", m_infoBarVictoryPage } }

    , m_infoBarDefeatPage{ new QWidget{ m_infoBar } }
    , m_infoBarDefeatLayout{ new QGridLayout{ m_infoBarDefeatPage } }
    , m_infoBarDefeatToMenuBtn{ new QPushButton{ "Меню", m_infoBarDefeatPage } }
    , m_infoBarDefeatRestartBtn{ new QPushButton{ "Играть снова", m_infoBarDefeatPage } }
{
    setupCentral();
    setupMainPage();
    setupSettingsPage();
    setupConnectionPage();
    setupGamePage();
    setupInfoBar();
}

void MainWindow::startGame()
{
    if (m_mode == MainWindowEnums::Single || m_mode == MainWindowEnums::Host)
    {
        const int fieldWidth{ toInt(m_fieldWidthSettingLineEdit->text()) };
        const int fieldHeight{ toInt(m_fieldHeightSettingLineEdit->text()) };
        const int minesCount{ toInt(m_minesCountSettingLineEdit->text()) };

        m_gameField->setFieldSizeCells(fieldWidth, fieldHeight);
        m_gameField->setMinesCount(minesCount);
        m_gameField->generate();
        m_gameField->unblock();

        m_centralStack->setCurrentIndex(MainWindowEnums::Game);
        m_infoBar->setCurrentIndex(MainWindowEnums::Statistics);

        if (m_mode == MainWindowEnums::Host)
        {
            connectServer();
            m_server->listen(QHostAddress::Any, Constants::NETWORK_PORT);
        }
    }

    else if (m_mode == MainWindowEnums::Client)
    {
        connectClient();
        m_client->connectToHost(m_connectionIpLineEdit->text(),
            Constants::NETWORK_PORT);
    }
}

void MainWindow::endGame()
{
    m_gameField->clearData();
    m_centralStack->setCurrentIndex(MainWindowEnums::Main);

    switch (m_mode)
    {
    case MainWindowEnums::Host:
        disconnectServer();
        break;

    case MainWindowEnums::Client:
        disconnectClient();

    default:
        break;
    }

    m_mode = MainWindowEnums::None;
}

void MainWindow::restartGame()
{
    m_gameField->generate();
    m_gameField->unblock();

    m_infoBar->setCurrentIndex(MainWindowEnums::Statistics);
}

void MainWindow::setupCentral()
{
    setCentralWidget(m_central);
    m_centralLayout->addWidget(m_centralStack);
}

void MainWindow::setupMainPage()
{
    m_centralStack->insertWidget(MainWindowEnums::Main, m_mainPage);

    m_mainLayout->addWidget(m_singleplayerBtn, Qt::AlignCenter);
    auto singleplayerBtnLambda{
        [this] {
            m_mode = MainWindowEnums::Single;
            m_centralStack->setCurrentIndex(MainWindowEnums::Settings);
        }
    };
    connect(m_singleplayerBtn, &QPushButton::clicked, singleplayerBtnLambda);

    m_mainLayout->addWidget(m_multiplayerHostBtn, Qt::AlignCenter);
    auto multiplayerHostBtnLambda{
        [this] {
            m_mode = MainWindowEnums::Host;
            m_centralStack->setCurrentIndex(MainWindowEnums::Settings);
        }
    };
    connect(m_multiplayerHostBtn, &QPushButton::clicked,
        multiplayerHostBtnLambda);

    m_mainLayout->addWidget(m_multiplayerClientBtn, Qt::AlignCenter);
    auto multiplayerClientBtnLambda{
        [this] {
            m_mode = MainWindowEnums::Client;
            m_centralStack->setCurrentIndex(MainWindowEnums::Connection);
        }
    };
    connect(m_multiplayerClientBtn, &QPushButton::clicked,
        multiplayerClientBtnLambda);
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

void MainWindow::setupConnectionPage()
{
    m_centralStack->insertWidget(MainWindowEnums::Connection, m_connectionPage);

    m_connectionLayout->addWidget(m_connectionIpLabel, 0, 0, 1, 1);
    m_connectionLayout->addWidget(m_connectionIpLineEdit, 0, 1, 1, 1);
    m_connectionLayout->addWidget(m_connectionAcceptBtn, 1, 0, 1, 2,
        Qt::AlignCenter);

    auto connectionAcceptBtnLambda{
        [this] {
            startGame();

            m_connectionIpLineEdit->clear();
        }
    };
    connect(m_connectionAcceptBtn, &QPushButton::clicked,
        connectionAcceptBtnLambda);
}

void MainWindow::setupGamePage()
{
    m_centralStack->insertWidget(MainWindowEnums::Game, m_gamePage);

    m_gameLayout->addWidget(m_gameView, 4);
    m_gameLayout->addWidget(m_infoBar, 1);

    m_gameView->setMinimumSize(800, 600);
    m_gameView->setScene(m_gameField);

    m_infoBar->setMinimumSize(200, 600);

    connect(m_gameField, &Field::gameOverVictorySignal,
        this, &MainWindow::gameOverVictorySlot);
    connect(m_gameField, &Field::gameOverDefeatSignal,
        this, &MainWindow::gameOverDefeatSlot);
}

void MainWindow::setupInfoBar()
{
    m_infoBar->insertWidget(MainWindowEnums::Statistics, m_infoBarStatisticsPage);

    m_infoBar->insertWidget(MainWindowEnums::Victory, m_infoBarVictoryPage);
    m_infoBarVictoryLayout->addWidget(m_infoBarVictoryToMenuBtn, 0, 0, 1, 1,
        Qt::AlignCenter);
    connect(m_infoBarVictoryToMenuBtn, &QPushButton::clicked,
        this, &MainWindow::endGame);
    m_infoBarVictoryLayout->addWidget(m_infoBarVictoryRestartBtn, 1, 0, 1, 1,
        Qt::AlignCenter);
    connect(m_infoBarVictoryRestartBtn, &QPushButton::clicked,
        this, &MainWindow::restartGame);

    m_infoBar->insertWidget(MainWindowEnums::Defeat, m_infoBarDefeatPage);
    m_infoBarDefeatLayout->addWidget(m_infoBarDefeatToMenuBtn, 0, 0, 1, 1,
        Qt::AlignCenter);
    connect(m_infoBarDefeatToMenuBtn, &QPushButton::clicked,
        this, &MainWindow::endGame);
    m_infoBarDefeatLayout->addWidget(m_infoBarDefeatRestartBtn, 1, 0, 1, 1,
        Qt::AlignCenter);
    connect(m_infoBarDefeatRestartBtn, &QPushButton::clicked,
        this, &MainWindow::restartGame);
}

void MainWindow::connectClient()
{
    connect(m_gameField, &Field::toOnlineLeftClickSignal,
        m_client, &Client::toOnlineLeftClickSlot);
    connect(m_gameField, &Field::toOnlineRightClickSignal,
        m_client, &Client::toOnlineRightClickSlot);

    connect(m_client, &Client::fromOnlineLeftClickSignal,
        m_gameField, &Field::fromOnlineLeftClickSlot);
    connect(m_client, &Client::fromOnlineRightClickSignal,
        m_gameField, &Field::fromOnlineRightClickSlot);
}

void MainWindow::connectServer()
{
    connect(m_gameField, &Field::toOnlineLeftClickSignal,
        m_server, &Server::toOnlineLeftClickSlot);
    connect(m_gameField, &Field::toOnlineRightClickSignal,
        m_server, &Server::toOnlineRightClickSlot);

    connect(m_server, &Server::fromOnlineLeftClickSignal,
        m_gameField, &Field::fromOnlineLeftClickSlot);
    connect(m_server, &Server::fromOnlineRightClickSignal,
        m_gameField, &Field::fromOnlineRightClickSlot);
}

void MainWindow::disconnectClient()
{
    disconnect(m_gameField, &Field::toOnlineLeftClickSignal,
        m_client, &Client::toOnlineLeftClickSlot);
    disconnect(m_gameField, &Field::toOnlineRightClickSignal,
        m_client, &Client::toOnlineRightClickSlot);

    disconnect(m_client, &Client::fromOnlineLeftClickSignal,
        m_gameField, &Field::fromOnlineLeftClickSlot);
    disconnect(m_client, &Client::fromOnlineRightClickSignal,
        m_gameField, &Field::fromOnlineRightClickSlot);
}

void MainWindow::disconnectServer()
{
    disconnect(m_gameField, &Field::toOnlineLeftClickSignal,
        m_server, &Server::toOnlineLeftClickSlot);
    disconnect(m_gameField, &Field::toOnlineRightClickSignal,
        m_server, &Server::toOnlineRightClickSlot);

    disconnect(m_server, &Server::fromOnlineLeftClickSignal,
        m_gameField, &Field::fromOnlineLeftClickSlot);
    disconnect(m_server, &Server::fromOnlineRightClickSignal,
        m_gameField, &Field::fromOnlineRightClickSlot);
}