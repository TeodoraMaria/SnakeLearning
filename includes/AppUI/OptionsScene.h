#pragma once

#include "AppUI/IScene.h"
#include "qobject.h"
#include "ui_OptionsScene.h"
#include "GameSettings.h"

#include <string>
#include <memory>

namespace AppUI
{

    class OptionsScene :public IScene
    {
        Q_OBJECT
    public:

        OptionsScene(const std::string& name);
        ~OptionsScene();

        // Inherited via IScene
        virtual void createScene() override;
        virtual void release() override;

    protected slots:
        void quitButtonPressed();

    signals:
        void settingsChanged(const GameSettings& gameSettings);

    private:
        QWidget* m_centralWidget;
        std::shared_ptr<Ui_OptionsScene> m_ui;

        GameSettings m_gameSettings;
    };
}