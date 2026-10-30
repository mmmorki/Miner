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

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    void setupCentral();
    void setupMainPage();
    void setupSettingsPage();
    void setupGameScene() const;

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

    Field* m_gameField{ nullptr };
    QGraphicsView* m_gameView{ nullptr };
};

#endif //MAINWINDOW_H