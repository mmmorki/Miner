#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Field;

class QWidget;
class QStackedWidget;
class QGridLayout;
class QPushButton;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QGraphicsView;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

private slots:
    void gameOverVictorySlot();
    void gameOverDefeatSlot();

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

    void startGame() const;
    void endGame() const;
    void restartGame() const;

protected:
    void setupCentral();
    void setupMainPage();
    void setupSettingsPage();
    void setupGamePage();
    void setupInfoBar();

private:
    QWidget* m_central{ nullptr };
    QHBoxLayout* m_centralLayout{ nullptr };
    QStackedWidget* m_centralStack{ nullptr };

    QWidget* m_mainPage{ nullptr };
    QGridLayout* m_mainLayout{ nullptr };
    QPushButton* m_singleplayerBtn{ nullptr };
    QPushButton* m_multiplayerBtn{ nullptr };

    QWidget* m_settingsPage{ nullptr };
    QGridLayout* m_settingsLayout{ nullptr };
    QLabel* m_fieldWidthSettingLabel{ nullptr };
    QLineEdit* m_fieldWidthSettingLineEdit{ nullptr };
    QLabel* m_fieldHeightSettingLabel{ nullptr };
    QLineEdit* m_fieldHeightSettingLineEdit{ nullptr };
    QLabel* m_minesCountSettingLabel{ nullptr };
    QLineEdit* m_minesCountSettingLineEdit{ nullptr };
    QPushButton* m_acceptSettingsBtn{ nullptr };

    QWidget* m_gamePage{ nullptr };
    QHBoxLayout* m_gameLayout{ nullptr };
    Field* m_gameField{ nullptr };
    QGraphicsView* m_gameView{ nullptr };
    QStackedWidget* m_infoBar{ nullptr };

    QWidget* m_infoBarStatisticsPage{ nullptr };
    QGridLayout* m_infoBarStatisticsLayout{ nullptr };

    QWidget* m_infoBarVictoryPage{ nullptr };
    QGridLayout* m_infoBarVictoryLayout{ nullptr };
    QPushButton* m_infoBarVictoryToMenuBtn{ nullptr };
    QPushButton* m_infoBarVictoryRestartBtn{ nullptr };

    QWidget* m_infoBarDefeatPage{ nullptr };
    QGridLayout* m_infoBarDefeatLayout{ nullptr };
    QPushButton* m_infoBarDefeatToMenuBtn{ nullptr };
    QPushButton* m_infoBarDefeatRestartBtn{ nullptr };
};

#endif //MAINWINDOW_H