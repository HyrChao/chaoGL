#include <Level/LevelManager.h>

Level* LevelManager::m_currentLevel = nullptr;

LevelManager::LevelManager()
{

}

LevelManager::~LevelManager()
{

}
void LevelManager::SetSection(LevelName selectedSection)
{

}

string& LevelManager::GetCurrentLevelName()
{
	return m_currentLevel->name;
}

void LevelManager::SetCurrentLevel(Level * level)
{
	m_currentLevel = level;
}


void LevelManager::LoadLevel(LevelName levelName)
{
	if(m_currentLevel != nullptr)
		delete m_currentLevel;

	switch (levelName) {
		case LevelName::Main:
			LoadMainLevel();
			break;
		case LevelName::BlinnPhong:
			m_currentLevel = new LV_Hello();
			m_currentLevel->name = "BlinnPhong";
			break;
		case LevelName::LoadModel:
			m_currentLevel = new LV_Model();
			m_currentLevel->name = "LoadModel";
			break;
		case LevelName::PBR:
			m_currentLevel = new LV_PBR();
			m_currentLevel->name = "PBR";
			break;
	}
}

void LevelManager::SceneLoop()
{
	m_currentLevel->Loop();
}

void LevelManager::SceneGui()
{
	m_currentLevel->OnGui();
}

void LevelManager::LoadMainLevel()
{
	if (m_currentLevel != nullptr)
		delete m_currentLevel;

	m_currentLevel = new LV_Model();
	m_currentLevel->name = "Main";
}


