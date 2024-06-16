#include <QGraphicsView>
#include <QTimer>
#include <qaction.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qmessagebox.h>

#include "constants.h"
#include "gamecontroller.h"
#include "mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene, this)),
      game(new GameController(*scene, this))
{
    setCentralWidget(view);

    setFixedSize(600, 600);
    setWindowIcon(QIcon(":/images/snake_ico"));

	createActions();
	createMenus();
    initScene();
    initSceneBackground();

    QTimer::singleShot(0, this, SLOT(adjustViewSize()));
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::adjustViewSize()
{
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::createActions()
{
    newGameAction = new QAction(tr("&Новая игра"), this);
	newGameAction->setShortcuts(QKeySequence::New);
    newGameAction->setStatusTip(tr("Начать новую игру"));
	connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    exitAction = new QAction(tr("&Выйти"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Выйти из игры"));
	connect(exitAction, &QAction::triggered, this, &MainWindow::close);

    pauseAction = new QAction(tr("&Пауза"), this);
    pauseAction->setStatusTip(tr("Пауза..."));
	connect(pauseAction, &QAction::triggered, game, &GameController::pause);

    resumeAction = new QAction(tr("&Продолжить"), this);
    resumeAction->setStatusTip(tr("Продолжение..."));
    resumeAction->setEnabled(false);
    game->setResumeAction(resumeAction);
    connect(resumeAction, &QAction::triggered, game, &GameController::resume);

    gameHelpAction = new QAction(tr("Игры &Помощь"), this);
	gameHelpAction->setShortcut(tr("Ctrl+H"));
    gameHelpAction->setStatusTip(tr("Помощь по игре"));
	connect(gameHelpAction, &QAction::triggered, this, &MainWindow::gameHelp);

    aboutAction = new QAction(tr("&О приложении"), this);
    aboutAction->setStatusTip(tr("Просмотр"));
	connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    aboutQtAction = new QAction(tr("О &Qt"), this);
    aboutQtAction->setStatusTip(tr("Просмотр информации о qt"));
	connect(aboutQtAction, &QAction::triggered, qApp, QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    QMenu *options = menuBar()->addMenu(tr("&Настройки"));
	options->addAction(newGameAction);
	options->addSeparator();
	options->addAction(pauseAction);
	options->addAction(resumeAction);
	options->addSeparator();
	options->addAction(exitAction);

    QMenu *help = menuBar()->addMenu(tr("&Помощь"));
	help->addAction(gameHelpAction);
	help->addAction(aboutAction);
	help->addAction(aboutQtAction);
}

void MainWindow::initScene()
{
    scene->setSceneRect(-100, -100, 200, 200);
}

void MainWindow::initSceneBackground()
{
    QPixmap bg(TILE_SIZE, TILE_SIZE);
    QPainter p(&bg);
    p.setBrush(QBrush(Qt::gray));
    p.drawRect(0, 0, TILE_SIZE, TILE_SIZE);

    view->setBackgroundBrush(QBrush(bg));
}

void MainWindow::newGame()
{
	QTimer::singleShot(0, game, SLOT(gameOver()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("О игре"), tr("<h2>Игра змейка</h2>"
        "<p>Эта игра была написанна на фреймворке Qt."
        "<p>Автор Dubnovitskiy Dmitry"
        "<p>Github: dmitryreaper"));
}

void MainWindow::gameHelp()
{
    QMessageBox::about(this, tr("Помощь по игре"), tr("Используйте  ←, →, ↑, ↓ для перемещения"
        "<p>Пробел - пауза & продолжить"));
}
