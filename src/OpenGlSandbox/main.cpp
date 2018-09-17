#include <GameView/GameWindow.h>
#include <GameView/OpenGLRenderer.h>

int main(int nargs,char** args)
{
    GameView::GameWindow game(1000,500);

    game.run();

	return 0;
}
