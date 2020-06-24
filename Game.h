#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class CCube;
class CTriangle;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	CCatmullRom *m_pLap;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	CSphere *m_pSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom *m_pCatmullRom;
	COpenAssetImportMesh *m_pFighterMesh;
	CCube *m_pCube;
	CTriangle *m_pRepair;
	COpenAssetImportMesh *m_pCarMesh;
	COpenAssetImportMesh *m_pStandMesh;
	COpenAssetImportMesh *m_pFenceMesh;
	COpenAssetImportMesh *m_pTreeMesh;
	COpenAssetImportMesh *m_pConeMesh;
	COpenAssetImportMesh *m_pBuildingMesh;
	COpenAssetImportMesh *m_pStartMesh;
	
	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
	glm::vec3 T;
	glm::vec3 N;
	glm::vec3 B;
	glm::vec3 R;
	glm::vec3 L;

	int CameraView = 0;
	float m_sideMovement = 0.0f;
	float m_t;
	glm::vec3 m_carPosition;
	glm::vec3 m_carPosition1;
	glm::vec3 m_repair1 = glm::vec3(600, 0.0f, 610.f);
	glm::vec3 m_repair2 = glm::vec3(-240.f, 1.0f, -250.f);
	glm::vec3 m_cone1 = glm::vec3(-400, 1, 355);
	glm::vec3 m_cone2 = glm::vec3(-407.f, 1.f, 370.f);
	glm::vec3 m_cone3 = glm::vec3(-47.f, 1.f, -250.f);
	glm::vec3 m_cone4 = glm::vec3(-50.f, 1.f, -235.f);
	glm::vec3 m_cone5 = glm::vec3(915.f, 1.f, 335.f);
	glm::vec3 m_cone6 = glm::vec3(945.f, 1.f, 335.f);
	glm::mat4 m_carOrientation;
	float m_currentDistance;
	float theta;
	void SideMovement();
	int m_TrackPos = 1;
	int m_speedometer = 0;
	int m_damage = 0;
	int m_topSpeed = 200;
	float m_speed = 0.0f;
	int m_lapCount = 0;
	bool shake = false;
	float m_shakeAngle = 0.0f;
	float shakeTime = 0.0f;
	int fps = 0;
	int millis = 0;
	int sec = 0;
	int min = 0;
	int hr = 0;
	int m_lapNum;
	int up1 = 1;
	int counter = 0;
	glm::vec4 lightPosition2;
	glm::vec4 lightPosition3;
	glm::vec4 lightPosition4;
	glm::vec3 m_ball1 = glm::vec3(-400.f, 10.5f, 355.f);
	glm::vec3 m_ball2 = glm::vec3(-407.f, 10.5f, 370.f);
	glm::vec3 m_ball3 = glm::vec3(-47.f, 10.5f, -250.f);
	glm::vec3 m_ball4 = glm::vec3(-50.f, 10.5f, -235.f);
	glm::vec3 m_ball5 = glm::vec3(915.f, 10.5f, 335.f);
	glm::vec3 m_ball6 = glm::vec3(945.f, 10.5f, 335.f);
	bool gameOver = false;
	bool lapsFinsihed = false;


public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void DisplaySpeed();
	void DisplayDamage();
	void DisplayLap(int lap);
	void DisplayFinished();
	void DisplayTime();
	void DisplayGameOver();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;


};
