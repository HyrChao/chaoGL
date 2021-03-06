#include <Level/LevelManager.h>

#include <Level/LV_Hello.h>
#include <Level/LV_PBR.h>
#include <Level/LV_Model.h>
#include <Level/LV_Shadow.h>

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
	if (m_currentLevel != nullptr)
	{
		delete m_currentLevel;
		m_currentLevel = nullptr;
	}

	switch (levelName) {
		case LevelName::Main:
			LoadMainLevel();
			break;
		case LevelName::BlinnPhong:
			m_currentLevel = new LV_Hello();
			m_currentLevel->name = "BlinnPhong";
			break;
		case LevelName::PBR:
			m_currentLevel = new LV_PBR();
			m_currentLevel->name = "PBR";
			break;
		case LevelName::LoadModel:
			m_currentLevel = new LV_Model();
			m_currentLevel->name = "LoadModel";
			break;
		case LevelName::Shadow:
			m_currentLevel = new LV_Shadow();
			m_currentLevel->name = "Shadow";
			break;
	}
}

void LevelManager::SceneLoop()
{
	m_currentLevel->Loop();
}

void LevelManager::OnSceneManagerGui()
{
	ImGui::Text("Select Scene");

	if (ImGui::Button("BlinnPhong"))
		LoadLevel(LevelName::BlinnPhong);
	if (ImGui::Button("LoadModel"))
		LoadLevel(LevelName::LoadModel);
	if (ImGui::Button("PBR"))
		LoadLevel(LevelName::PBR);
	if (ImGui::Button("Shadow"))
		LoadLevel(LevelName::Shadow);

}

void LevelManager::OnSceneGui()
{
	m_currentLevel->OnGui();
}

Level* LevelManager::GetCurrentLevel()
{
	return m_currentLevel;
}


void LevelManager::LoadMainLevel()
{
	if (m_currentLevel != nullptr)
		delete m_currentLevel;

	m_currentLevel = new LV_PBR();
	m_currentLevel->name = "pbr";
}


