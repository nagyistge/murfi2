/********************************************************************************
** Form generated from reading ui file 'frmainwindow.ui'
**
** Created: Tue 12. Aug 01:15:39 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FRMAINWINDOW_H
#define UI_FRMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionTool1;
    QAction *actionTool2;
    QAction *actionTool3;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QMenu *menuEdit;
    QMenu *menuTools;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;
    QToolBar *toolBar_3;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(800, 600);
    actionExit = new QAction(MainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    actionAbout = new QAction(MainWindow);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionTool1 = new QAction(MainWindow);
    actionTool1->setObjectName(QString::fromUtf8("actionTool1"));
    actionTool2 = new QAction(MainWindow);
    actionTool2->setObjectName(QString::fromUtf8("actionTool2"));
    actionTool3 = new QAction(MainWindow);
    actionTool3->setObjectName(QString::fromUtf8("actionTool3"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 800, 19));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    menuView = new QMenu(menubar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menuEdit = new QMenu(menubar);
    menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
    menuTools = new QMenu(menubar);
    menuTools->setObjectName(QString::fromUtf8("menuTools"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);
    toolBar = new QToolBar(MainWindow);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar_2 = new QToolBar(MainWindow);
    toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
    MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);
    toolBar_3 = new QToolBar(MainWindow);
    toolBar_3->setObjectName(QString::fromUtf8("toolBar_3"));
    MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_3);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuView->menuAction());
    menubar->addAction(menuEdit->menuAction());
    menubar->addAction(menuTools->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionExit);
    menuHelp->addAction(actionAbout);
    toolBar_3->addAction(actionExit);
    toolBar_3->addAction(actionAbout);
    toolBar_3->addSeparator();
    toolBar_3->addAction(actionTool1);
    toolBar_3->addAction(actionTool2);
    toolBar_3->addAction(actionTool3);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
    actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
    actionTool1->setText(QApplication::translate("MainWindow", "Tool1", 0, QApplication::UnicodeUTF8));
    actionTool2->setText(QApplication::translate("MainWindow", "Tool2", 0, QApplication::UnicodeUTF8));
    actionTool3->setText(QApplication::translate("MainWindow", "Tool3", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
    menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
    menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
    toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0, QApplication::UnicodeUTF8));
    toolBar_3->setWindowTitle(QApplication::translate("MainWindow", "toolBar_3", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_FRMAINWINDOW_H