#ifndef LevelManager_H
#define LevelManager_H

#include <Level/LV_Hello.h>
#include <Level/LV_PBR.h>
#include <Level/LV_Model.h>

enum LevelName
{
	Main,
	BlinnPhong,
	LoadModel,
	PBR
};

class LevelManager
{
public:
	static void SwitchSections();
	static void SetSection(LevelName selectedSection);
	static string& GetCurrentLevelName();
	static void SetCurrentLevel(Level* level);

	static void LoadLevel(LevelName levelName);
	static void LoadMainLevel();
	static void SceneLoop();
	static void SceneGui();

private:

	LevelManager();
	~LevelManager();

private:

	static Level* m_currentLevel;


};


#endif // !LevelManager_H
