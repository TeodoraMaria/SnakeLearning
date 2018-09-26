#pragma once

namespace AppUI
{

    struct GameSettings
    {
        size_t nbHumanPlayers=1;
        size_t nbGeneticBots=0;
        size_t nbQlearningDeepBots=0;
        size_t nbQlearningTabularBots = 0;
        size_t nbSupervisedBots=0;
        size_t nbNormalBots=0;

        size_t mapWidth=25;
        size_t mapHeight=25;
        size_t foodCount=10;
    };
}