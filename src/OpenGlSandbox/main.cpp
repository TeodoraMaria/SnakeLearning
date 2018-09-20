#include <QApplication.h>

#include <AppUI/Application.h>
#include <AppUI/ApplicationModel.h>

int main(int argc,char** argv)
{
    //GameView::GameWindow game(1000,500);

    srand(time(0));
    QApplication qApplication(argc, argv);

    AppUI::ApplicationModel* appModel= AppUI::ApplicationModel::getInstance();

    AppUI::Application app(appModel);

    app.start("lol", 500, 500);

    qApplication.exec();

	return 0;
}
