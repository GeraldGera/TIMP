#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_apiClient(new ApiClient(this))
    , m_isAuthenticated(false)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    m_stackedWidget = new QStackedWidget(this);
    layout->addWidget(m_stackedWidget);

    setupPages();
    setupMenu();

    if (!m_isAuthenticated) {
        m_stackedWidget->setCurrentWidget(m_authRegForm);
    }

    connect(m_authRegForm, &AuthRegForm::userLoggedIn, this, &MainWindow::onUserLoggedIn);
    connect(m_authRegForm, &AuthRegForm::userLoggedIn, m_tasksPage, &TasksPage::onUserChanged);
    connect(m_authRegForm, &AuthRegForm::userLoggedIn, m_profilePage, &ProfilePage::onUserChanged);
}

void MainWindow::setupPages()
{
    m_authRegForm = new AuthRegForm(m_apiClient, this);
    m_tasksPage = new TasksPage(m_apiClient, this);
    m_profilePage = new ProfilePage(m_apiClient, this);

    m_stackedWidget->addWidget(m_authRegForm);
    m_stackedWidget->addWidget(m_tasksPage);
    m_stackedWidget->addWidget(m_profilePage);
}

void MainWindow::setupMenu()
{
    m_actionTasks = new QAction("&Задания", this);
    m_actionProfile = new QAction("&Профиль", this);
    m_actionLogout = new QAction("&Выйти", this);

    QMenu *navMenu = menuBar()->addMenu("&Меню");
    navMenu->addAction(m_actionTasks);
    navMenu->addAction(m_actionProfile);
    navMenu->addSeparator();
    navMenu->addAction(m_actionLogout);

    connect(m_actionTasks, &QAction::triggered, this, &MainWindow::navigateToTasks);
    connect(m_actionProfile, &QAction::triggered, this, &MainWindow::navigateToProfile);
    connect(m_actionLogout, &QAction::triggered, this, &MainWindow::logout);

    updateMenuForAuthState();
}

void MainWindow::updateMenuForAuthState()
{
    m_actionTasks->setVisible(m_isAuthenticated);
    m_actionProfile->setVisible(m_isAuthenticated);
    m_actionLogout->setVisible(m_isAuthenticated);
}

void MainWindow::navigateToTasks()
{
    if (!m_isAuthenticated) {
        m_stackedWidget->setCurrentWidget(m_authRegForm);
        return;
    }
    m_stackedWidget->setCurrentWidget(m_tasksPage);
}

void MainWindow::navigateToProfile()
{
    if (!m_isAuthenticated) {
        m_stackedWidget->setCurrentWidget(m_authRegForm);
        return;
    }
    m_stackedWidget->setCurrentWidget(m_profilePage);
}

void MainWindow::logout()
{
    m_apiClient->logout(m_currentUser.id);
    m_isAuthenticated = false;
    m_currentUser = UserInfo();
    updateMenuForAuthState();
    m_stackedWidget->setCurrentWidget(m_authRegForm);
}

void MainWindow::onUserLoggedIn(const UserInfo &user)
{
    m_isAuthenticated = true;
    m_currentUser = user;
    updateMenuForAuthState();
    m_stackedWidget->setCurrentWidget(m_tasksPage);
}

MainWindow::~MainWindow()
{
}
