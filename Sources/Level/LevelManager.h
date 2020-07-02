#ifndef LevelManager_H
#define LevelManager_H

#include<Level/Level.h>

enum LevelName
{
	Main,
	BlinnPhong,
	LoadModel,
	PBR,
	Shadow,
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
	static void OnSceneManagerGui();
	static void OnSceneGui();
	static Level* GetCurrentLevel();
private:

	LevelManager();
	~LevelManager();

public:


private:

	static Level* m_currentLevel;


};


#endif // !LevelManager_H
