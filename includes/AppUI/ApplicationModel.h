#pragma once
#include <string>

#include <map>

namespace AppUI
{   
    class IScene;

    class ApplicationModel
    {
    public:
        ~ApplicationModel();


        static ApplicationModel* getInstance();

        void defineScenes();
        void defineInitialScene();
        std::map<const std::string, IScene*> getScenes()const ;
        std::string getInitialSceneName() const;
        IScene* getInitialScene() const;
        IScene* getScene(const std::string& name) const;

        static const std::string startMenuSceneName;
        static const std::string gameSceneName;
        static const std::string optionsSceneName;

    private:
        ApplicationModel();
        static  ApplicationModel* instance;
        std::string m_initialScene;
        std::map<const std::string, IScene*> m_scenes;
    };
}