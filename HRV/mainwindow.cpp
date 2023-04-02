#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Create menu tree
    masterMenu = new Menu("MAIN MENU", {"START A NEW SESSION","SETTINGS","HISTORY"}, nullptr);
    mainMenuOG = masterMenu;
    initializeMainMenu(masterMenu);

    // Initialize the main menu view
    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());

//    //WHEN POWERED ON
//    activeQListWidget->setVisible(true);
//    ui->menuLabel->setVisible(true);
//    ui->statusBarQFrame->setVisible(true);
//    ui->treatmentView->setVisible(false);
//    ui->frequencyLabel->setVisible(true);
//    ui->programViewWidget->setVisible(true);




    // device interface button connections
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->backButton, &QPushButton::pressed, this, &MainWindow::navigateBack);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeMainMenu(Menu* m) {

    Menu* session = new Menu("START A NEW SESSION", {"YES","NO"}, m);
    Menu* settings = new Menu("SETTINGS", {"CHALLENGE LEVEL","BREATH INTERVAL"}, m);
    Menu* history = new Menu("HISTORY", {"VIEW","CLEAR"}, m);

    m->addChildMenu(session);
    m->addChildMenu(settings);
    m->addChildMenu(history);


    Menu* challengeSetting = new Menu("CHALLENGE LEVEL",{"YES","NO"}, settings);
    Menu* breathSetting = new Menu("BREATH INTERVAL",{"YES","NO"}, settings);
    settings->addChildMenu(challengeSetting);
    settings->addChildMenu(breathSetting);



    Menu* viewHistory = new Menu("VIEW",{}, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);
}

void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}
void MainWindow::navigateSubMenu() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    // Prevent crash if ok button is selected in view, or challenge, or breath
    if (masterMenu->getName() == "VIEW") {
        return;
    }

    //Logic for when the menu is the delete menu.
    if (masterMenu->getName() == "CLEAR") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            //delete recordings
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    if (masterMenu->getName() == "START A NEW SESSION") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            //start a session here
            navigateBack();
            return;
        }
        else {//NO selected, and go back
            navigateBack();
            return;
        }
    }
    if (masterMenu->getName() == "CHALLENGE LEVEL") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            //edit challenge level here
            navigateBack();
            return;
        }
        else {//NO selected, and go back
            navigateBack();
            return;
        }
    }

    if (masterMenu->getName() == "BREATH INTERVAL") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            //edit challenge level here
            navigateBack();
            return;
        }
        else {//NO selected, and go back
            navigateBack();
            return;
        }
    }


    //If the menu is a parent and clicking on it should display more menus.//prob not needed
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }
}

void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(selectedMenuItem);
}


void MainWindow::navigateBack() {

    ui->rightButton->blockSignals(true);
    ui->leftButton->blockSignals(true);
    //stop session
    //save session

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }

//    ui->programViewWidget->setVisible(false); // dk why this is done, this shutsdown the menu screen
//    ui->electrodeLabel->setVisible(false);
}

void MainWindow::navigateToMainMenu() {
    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
//    ui->programViewWidget->setVisible(false);
//    ui->electrodeLabel->setVisible(false);
}
