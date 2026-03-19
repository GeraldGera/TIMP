#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "apiclient.h"
#include "authregform.h"
#include "taskspage.h"
#include "profilepage.h"
#include "userinfo.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void navigateToTasks();
    void navigateToProfile();
    void logout();
    void onUserLoggedIn(const UserInfo &user);

private:
    void setupMenu();
    void setupPages();
    void updateMenuForAuthState();

    ApiClient *m_apiClient;
    QStackedWidget *m_stackedWidget;
    AuthRegForm *m_authRegForm;
    TasksPage *m_tasksPage;
    ProfilePage *m_profilePage;

    QAction *m_actionTasks;
    QAction *m_actionProfile;
    QAction *m_actionLogout;

    UserInfo m_currentUser;
    bool m_isAuthenticated;
};

#endif // MAINWINDOW_H
