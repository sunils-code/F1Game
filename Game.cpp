/*
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting,
 different camera controls, different shaders, etc.

 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk)

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"
#include "CatmullRom.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "Cube.h"
#include "Triangle.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;
	m_pFighterMesh = NULL;
	m_pCube = NULL;
	m_pCarMesh = NULL;
	m_pStandMesh = NULL;
	m_pFenceMesh = NULL;
	m_pTreeMesh = NULL;
	m_pRepair = NULL;
	m_pConeMesh = NULL;
	m_pLap = NULL;
	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;
	
	
}

// Destructor
Game::~Game()
{
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pCatmullRom;
	delete m_pFighterMesh;
	delete m_pCube;
	delete m_pCarMesh;
	delete m_pStandMesh;
	delete m_pFenceMesh;
	delete m_pTreeMesh;
	delete m_pRepair;
	delete m_pConeMesh;
	delete m_pLap;
	delete m_pBuildingMesh;
	delete m_pStartMesh;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise()
{


	float m_t = 0.0f;
	glm::vec3 m_spaceShipPosition = { 0.f, 0.f, 0.f };
	glm::mat4 m_spaceShipOrientation = glm::mat4(1);
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	m_pFighterMesh = new COpenAssetImportMesh;
	m_pCube = new CCube;
	m_pCarMesh = new COpenAssetImportMesh;
	m_pStandMesh = new COpenAssetImportMesh;
	m_pFenceMesh = new COpenAssetImportMesh;
	m_pTreeMesh = new COpenAssetImportMesh;
	m_pRepair = new CTriangle;
	m_pConeMesh = new COpenAssetImportMesh;
	m_pBuildingMesh = new COpenAssetImportMesh;
	m_pStartMesh = new COpenAssetImportMesh;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	//=====Animation for creating line CreatePath()
	//glm::vec3 p0 = glm::vec3(-500, 10, -200);
	//glm::vec3 p1 = glm::vec3(0, 10, -200);
	//glm::vec3 p2 = glm::vec3(0, 10, 200);
	//glm::vec3 p3 = glm::vec3(-500, 10, 200);

	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack("resources\\textures\\", "r2.jpg");
	//m_pCatmullRom->CreatePath(p0, p1, p2, p3);

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height);
	m_pCamera->SetPerspectiveProjectionMatrix(45.5f, (float)width / (float)height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("sphereShader.vert");
	sShaderFileNames.push_back("sphereShader.frag");

	for (int i = 0; i < (int)sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int)sShaderFileNames[i].size() - 4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\" + sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create the sphere shader program
	CShaderProgram *pSphereProgram = new CShaderProgram;
	pSphereProgram->CreateProgram();
	pSphereProgram->AddShaderToProgram(&shShaders[4]);
	pSphereProgram->AddShaderToProgram(&shShaders[5]);
	pSphereProgram->LinkProgram();
	m_pShaderPrograms->push_back(pSphereProgram);

	//CShaderProgram *pTerrainProgram = new CShaderProgram;
	//pTerrainProgram->CreateProgram();
	//pTerrainProgram->AddShaderToProgram(&shShaders[6]);
	//pTerrainProgram->AddShaderToProgram(&shShaders[7]);
	//pTerrainProgram->LinkProgram();
	//m_pShaderPrograms->push_back(pTerrainProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);

	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grass.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadFont("resources\\fonts\\orangekid.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	//m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	//m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	//m_pFighterMesh->Load("resources\\models\\Fighter\\fighter1.3ds");
	m_pCarMesh->Load("resources\\models\\Car\\ferrari2.obj");
	m_pStandMesh->Load("resources\\models\\Enviroment\\generic medium.obj"); //downloaded from https://www.cgtrader.com/items/183081/download-page 0n 03 April 2020
	m_pFenceMesh->Load("resources\\models\\Enviroment\\fence_01_obj.obj"); //downloaded from https://www.turbosquid.com/3d-models/stadium-fence-3ds-free/545545 on 03 April 2020
	m_pTreeMesh->Load("resources\\models\\Enviroment\\firtree1.3ds"); //downloaded from https://www.turbosquid.com/3d-models/free-firtree-3d-model/480733 on 03 April 2020
	m_pRepair->Create("resources\\textures\\bolt.jpg");
	m_pConeMesh->Load("resources\\models\\Enviroment\\1.obj");
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	m_pBuildingMesh->Load("resources\\models\\Enviroment\\Building.obj");
	m_pStartMesh->Load("resources\\models\\Enviroment\\LevelCrossing.obj");
	//glEnable(GL_CULL_FACE);

	//Cube
	m_pCube->Create("resources\\textures\\wallg.jpg");

	// Initialise audio and play background music
	//m_pAudio->Initialise();
	//m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	//m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();
}

// Render method runs repeatedly in a loop
void Game::Render()
{

	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	pMainProgram->SetUniform("sampler1", 1);
	pMainProgram->SetUniform("wallShift", false);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10;
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);


	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);


	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*

	if (counter % 6 == 0)
	{

		lightPosition2 = glm::vec4(-400.f, 200.f, 355.f, 1);
		lightPosition3 = glm::vec4(-47.f, 200.f, -250.f, 1);
		lightPosition4 = glm::vec4(915.f, 200.f, 335.f, 1);
	}
	else
	{
		lightPosition2 = glm::vec4(0, -30, 0, 1);
		lightPosition3 = glm::vec4(0, -30, 0, 1);
		lightPosition4 = glm::vec4(0, -30, 0, 1);
	}

	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(0.0f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property







	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("renderSkybox", true);
	// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
	glm::vec3 vEye = m_pCamera->GetPosition();
	modelViewMatrixStack.Translate(vEye);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSkybox->Render(cubeMapTextureUnit);
	pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	




//	modelViewMatrixStack.Push(); {
		// Render the F1 CAR
	//	modelViewMatrixStack.SetIdentity();
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(m_carPosition);
	modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), theta);
	modelViewMatrixStack.Scale(5.0f);
	
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);

	m_pCarMesh->Render();
	modelViewMatrixStack.Pop();


	

	//stand 1
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(400.f, 0.0f, 500.0f));
	//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 2
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(295.f, 0.0f, 500.0f));
	//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 3
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(190.f, 0.0f, 500.0f));
	//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 4
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(85.f, 0.0f, 500.0f));
	//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 5
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(400.f, 0.0f, 700.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 6
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(295.f, 0.0f, 700.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 7
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(190.f, 0.0f, 700.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 8
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(85.f, 0.0f, 700.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 3.14f);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 9
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-780.f, 0.0f, -260.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 40 * M_PI / 180);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 10
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-618.f, 0.0f, -397.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 40 * M_PI / 180);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	//stand 11
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-698.f, 0.0f, -329.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 40 * M_PI / 180);
	modelViewMatrixStack.Scale(4.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStandMesh->Render();
	modelViewMatrixStack.Pop();

	////fence 1
	for (int h = 0; h <= 50; h++) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(250.f - (h * 20), 0.0f, 560.f));
		//modelViewMatrixStack.Translate(glm::vec3(0.f, 5.0f, 1.f));
		//modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 40 * M_PI / 180);
		modelViewMatrixStack.Scale(0.05f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pFenceMesh->Render();
		modelViewMatrixStack.Pop();
	}

	// Render the wall 4
	for (int h = 0; h <= 50; h++) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(225.f - (h * 20), 0.0f, 560.f));
		modelViewMatrixStack.Scale(5.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
		//pMainProgram->SetUniform("bUseTexture", false);
		m_pCube->Render();
		modelViewMatrixStack.Pop();
	}

	//fence 1 
	for (int h = 0; h <= 50; h++) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(500.f - (h * 20), 0.0f, 650.f));
		//modelViewMatrixStack.Translate(glm::vec3(0.f, 5.0f, 1.f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180 * M_PI / 180);
		modelViewMatrixStack.Scale(0.05f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pFenceMesh->Render();
		modelViewMatrixStack.Pop();
	}

	// Render the wall 4
	//for (int h = 0; h <= 0; h++) {
	//	modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(250.f - (h * 20), 0.0f, 650.f));
	//	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180 * M_PI / 180);
	//	modelViewMatrixStack.Scale(5.0f);
	//	
	//	pMainProgram->SetUniform("wallShift", true);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//	//pMainProgram->SetUniform("bUseTexture", false);
	//	m_pCube->Render();
	//	modelViewMatrixStack.Pop();
	//}

	//Render Wall
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(500.f, 0.0f, 650.f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180 * M_PI / 180);
	modelViewMatrixStack.Scale(5.0f);

	pMainProgram->SetUniform("wallShift", true);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pCube->Render();
	pMainProgram->SetUniform("wallShift", false);
	modelViewMatrixStack.Pop();
		//buildings
		for (int y = 0; y <= 4; y++) {
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(550.f - (y * 300), 0.0f, 900.f));
			//modelViewMatrixStack.Translate(glm::vec3(0.f, 5.0f, 1.f));
			modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90 * M_PI / 180);
			modelViewMatrixStack.Scale(0.09f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pBuildingMesh->Render();
			modelViewMatrixStack.Pop();
		}
		////Tree 1
		for (int z = 0; z <= 6; z++) {
			for (int x = 0; x <= 8 ; x++) {
				modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(glm::vec3(-550.f + (x * 65), 0.0f, 250.f - (z * 65)));
				modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 270 * M_PI / 180);
				modelViewMatrixStack.Scale(16.f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				m_pTreeMesh->Render();
				modelViewMatrixStack.Pop();
			}
		}
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(-550.f, 0.0f, 250.f));
			modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 270 * M_PI / 180);
			modelViewMatrixStack.Scale(16.f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pTreeMesh->Render();
			modelViewMatrixStack.Pop();

		//Repair 1
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_repair1);
		modelViewMatrixStack.Scale(2.f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pRepair->Render();
		modelViewMatrixStack.Pop();

		if (counter < 600)
		{
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(200.f, 20.f, 610.f);
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180 * M_PI / 180);
			modelViewMatrixStack.Scale(0.5f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pStartMesh->Render();
			modelViewMatrixStack.Pop();
		}
		

		//trafficelight 2
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_repair2);
		//	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 280 * M_PI / 180);
		modelViewMatrixStack.Scale(2.f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pRepair->Render();
		modelViewMatrixStack.Pop();
		
		//cone1
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_cone1);
		modelViewMatrixStack.Scale(1.2f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pConeMesh->Render();
		modelViewMatrixStack.Pop();
		//cone2
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_cone2);
		modelViewMatrixStack.Scale(1.2f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pConeMesh->Render();
		modelViewMatrixStack.Pop();
		//cone3
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_cone3);
		modelViewMatrixStack.Scale(1.2f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pConeMesh->Render();
		modelViewMatrixStack.Pop();
		//cone4
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_cone4);
		modelViewMatrixStack.Scale(1.2f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pConeMesh->Render();
		modelViewMatrixStack.Pop();
		//cone5
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_cone5);
		modelViewMatrixStack.Scale(1.2f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pConeMesh->Render();
		modelViewMatrixStack.Pop();
		//cone6
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_cone6);
		modelViewMatrixStack.Scale(1.2f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pConeMesh->Render();
		modelViewMatrixStack.Pop();

		//Track
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix",m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		// Render your object here
		//m_pCatmullRom->RenderPath();
		//m_pCatmullRom->RenderCentreline();
		//m_pCatmullRom->RenderOffsetCurves();
		m_pCatmullRom->RenderTrack();
		modelViewMatrixStack.Pop();

		// Switch to the sphere program
		/*CShaderProgram *pTerrainProgram = (*m_pShaderPrograms)[3];
		pTerrainProgram->UseProgram();
		pTerrainProgram->SetUniform("sampler0", 0);
		pTerrainProgram->SetUniform("sampler1", 1);*/
	//	// Render the planar terrain
	//	modelViewMatrixStack.Push();
	////	pMainProgram->SetUniform("bUseTerrain", true);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	m_pPlanarTerrain->Render();
	//	modelViewMatrixStack.Pop();

		// Switch to the sphere program
		CShaderProgram *pSphereProgram = (*m_pShaderPrograms)[2];
		pSphereProgram->UseProgram();

		// sphere spotlights
			pSphereProgram->SetUniform("light1.position", viewMatrix*lightPosition2);
			pSphereProgram->SetUniform("light1.La", glm::vec3(1.0f, 0.6f, 0.0f));
			pSphereProgram->SetUniform("light1.Ld", glm::vec3(0.0f, 1.0f, 0.0f));
			pSphereProgram->SetUniform("light1.Ls", glm::vec3(0.0f, 1.0f, 0.0f));
			pSphereProgram->SetUniform("light1.direction", glm::normalize(viewNormalMatrix*glm::vec3(0, -0.2, 0)));
			pSphereProgram->SetUniform("light1.exponent", 15.0f);
			pSphereProgram->SetUniform("light1.cutoff", 10.0f);

			pSphereProgram->SetUniform("light2.position", viewMatrix*lightPosition3);
			pSphereProgram->SetUniform("light2.La", glm::vec3(1.0f, 0.6f, 0.0f));
			pSphereProgram->SetUniform("light2.Ld", glm::vec3(0.0f, 1.0f, 0.0f));
			pSphereProgram->SetUniform("light2.Ls", glm::vec3(0.0f, 1.0f, 0.0f));
			pSphereProgram->SetUniform("light2.direction", glm::normalize(viewNormalMatrix*glm::vec3(0, -0.2, 0)));
			pSphereProgram->SetUniform("light2.exponent", 15.0f);
			pSphereProgram->SetUniform("light2.cutoff", 10.0f);

			pSphereProgram->SetUniform("light3.position", viewMatrix*lightPosition4);
			pSphereProgram->SetUniform("light3.La", glm::vec3(1.0f, 0.6f, 0.0f));
			pSphereProgram->SetUniform("light3.Ld", glm::vec3(0.0f, 1.0f, 0.0f));
			pSphereProgram->SetUniform("light3.Ls", glm::vec3(0.0f, 1.0f, 0.0f));
			pSphereProgram->SetUniform("light3.direction", glm::normalize(viewNormalMatrix*glm::vec3(0, -0.2, 0)));
			pSphereProgram->SetUniform("light3.exponent", 15.0f);
			pSphereProgram->SetUniform("light3.cutoff", 10.0f);

			pSphereProgram->SetUniform("material1.shininess", 3.0f);
			pSphereProgram->SetUniform("material1.Ma", glm::vec3(1.0f, 0.0f, 0.2f));
			pSphereProgram->SetUniform("material1.Md", glm::vec3(0.0f, 0.0f, 1.0f));
			pSphereProgram->SetUniform("material1.Ms", glm::vec3(1.0f, 1.0f, 1.0f));

	
		
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_ball1);
		pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_ball2);
		pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_ball3);
		pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_ball4);
		pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_ball5);
		pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_ball6);
		pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
		modelViewMatrixStack.Pop();
	// Draw the 2D graphics after the 3D graphics
	//DisplayFrameRate();

	//display speed
	DisplaySpeed();

	//display damage
	DisplayDamage();

	//displaying lap count
	if (m_lapCount == 3)
	{
		DisplayLap(2);
		up1 = 0;

	}
	else
	{
		DisplayLap(m_lapCount);
	}
	
	//display time
	DisplayTime();

	//display gameover
	if (m_topSpeed == 0)
	{
		up1 = 0;
		
		m_currentDistance = 0.0f;
		gameOver = true;
		
		DisplayGameOver();
	}

	//display finsihed
	if ((up1 == 0) && (gameOver == false)) 
	{
		
		DisplayFinished();
		m_currentDistance = 0.0f;
		DisplayLap(2);
	}

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());

}
void Game::SideMovement()
{
	if (m_TrackPos == 0)
	{
		if (m_sideMovement > -15.0f)
		{
			m_sideMovement -= 0.1f;
		}
	}
	else if (m_TrackPos == 1)
	{
		if (m_sideMovement > 0.0f)
		{
			m_sideMovement -= 0.1f;
		}
		else if (m_sideMovement < 0.0f)
		{
			m_sideMovement += 0.1f;
		}
		else
		{
			m_sideMovement += 0.0f;
		}
	}
	else if (m_TrackPos == 2)
	{
		if (m_sideMovement < 15.0f)
		{
			m_sideMovement += 0.1f;
		}
	}


}

// Update method runs repeatedly with the Render method
void Game::Update()
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);

	//m_pAudio->Update();


	//catmull
	glm::vec3 z = glm::vec3(0, 0, 0);
	glm::vec3 p, pNext,p1,p2,d, firstP, thirdP;

	m_pCatmullRom->Sample(m_currentDistance, p1, z);
	m_pCatmullRom->Sample(m_currentDistance + 1.0f, p2, z);
	m_pCatmullRom->Sample(m_currentDistance + 2.f, firstP, z);
	m_pCatmullRom->Sample(m_currentDistance - 17.5f, thirdP, z);
	//m_currentDistance += 0.1f;
	m_pCatmullRom->Sample(m_currentDistance, p, z);
	m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext, z);


	T = glm::normalize(pNext - p);
	N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
	B = glm::normalize(glm::cross(N, T));
	
	SideMovement();
	counter++;

	fps += up1;
	millis = fps;
	sec = (millis / 1000) % 60;
	min = ((millis / (1000 * 60)) % 60);
	hr = ((millis / (1000 *60 * 60)) % 24);


	if (m_shakeAngle > 0.0f)
	{
		m_shakeAngle -= m_dt * 0.003f;
	}
	else if (m_shakeAngle < 0.0f)
	{
		m_shakeAngle += m_dt * 0.003f;
	}


	if (CameraView == 0)
	{
		if (shake == false)
		{
			glm::vec3 cameraPos = firstP + (4.6f * (B)) + (N * m_sideMovement);
			glm::vec3 viewPoint = p1 + (20.0f * T);
			m_pCamera->Set(cameraPos, viewPoint + (N * m_sideMovement), glm::vec3(0, 1, 0));
		}
		else if (shake == true)
		{
			glm::vec3 cameraPos = firstP + (4.6f * (B)) + (N * m_sideMovement);
			glm::vec3 viewPoint = p1 + (20.0f * T);
			m_pCamera->Set(cameraPos, viewPoint + (N * m_sideMovement), glm::vec3(0, 1, 0));
			
			shakeTime += 1.0f;
			m_pCamera->RotateViewPoint(m_shakeAngle,viewPoint);
			if (shakeTime == 100.f)
			{
				shake = false;
				shakeTime = 0.0f;
			}
		}
		
	}
	else if (CameraView == 1)
	{
		/*glm::vec3 cameraPos = thirdP + (8.0f * (B)) + (N * m_sideMovement);
		glm::vec3 viewPoint = p1 + (100.0f * T);
		m_pCamera->Set(cameraPos, viewPoint + (N * m_sideMovement), glm::vec3(0, 1, 0));*/
		if (shake == false)
		{
			glm::vec3 cameraPos = thirdP + (8.0f * (B)) + (N * m_sideMovement);
			glm::vec3 viewPoint = p1 + (100.0f * T);
			m_pCamera->Set(cameraPos, viewPoint + (N * m_sideMovement), glm::vec3(0, 1, 0));
		}
		else if (shake == true)
		{
			glm::vec3 cameraPos = thirdP + (8.0f * (B)) + (N * m_sideMovement);
			glm::vec3 viewPoint = p1 + (100.0f * T);
			m_pCamera->Set(cameraPos, viewPoint + (N * m_sideMovement), glm::vec3(0, 1, 0));

			shakeTime += 1.0f;
			m_pCamera->RotateViewPoint(m_shakeAngle, viewPoint);
			if (shakeTime == 100.f)
			{
				shake = false;
				shakeTime = 0.0f;
			}
		}
		
	}
	else if (CameraView == 2)
	{
	/*	glm::vec3 cameraPos = thirdP + (8.0f * (B)) + (N * m_sideMovement);
		glm::vec3 viewPoint = p1 + (100.0f * T);*/
		
	}
	//Car Orientation and Position
	d = p2 - p1;
	theta = atan2(d.x,d.z);
	m_carPosition = p1 + (N * m_sideMovement);
	
	//speed
	if (m_speedometer == 0)
	{
		m_speed = 0.0f;
	}
	else if (m_speedometer == 50)
	{
		m_speed = 0.05f;
	}
	else if (m_speedometer == 100)
	{
		m_speed = 0.12f;
	}
	else if (m_speedometer == 150)
	{
		m_speed = 0.19f;
	}
	else if (m_speedometer == 200)
	{
		m_speed = 0.26f;
	}

	m_currentDistance += m_dt * (m_speed);

	//collisions
	if ((glm::length(m_repair1 - m_carPosition) < 10.f) && (m_damage > 0))
	{
		m_repair1 = glm::vec3(0.f, -100.0f, 0.f);
		if (m_topSpeed != 200)
		{
			m_topSpeed += 50;
			m_damage -= 10;
			
		}
	}
	if ((glm::length(m_repair2 - m_carPosition) < 10.f) && (m_damage > 0))
	{
		m_repair2 = glm::vec3(0.f, -100.0f, 0.f);
		if (m_topSpeed != 200)
		{
			m_topSpeed += 50;
			m_damage -= 10;
		}
	}
	if (glm::length(m_cone1 - m_carPosition) < 10.f)
	{
		m_cone1 = glm::vec3(0.f, -100.0f, 0.f);
		m_damage += 10;
		m_topSpeed -= 50;
		m_speedometer -= 50;
		shake = true;
		m_shakeAngle = 0.05f;
		m_ball1 = glm::vec3(0.f, -100.f, 0.f);
	}
	if (glm::length(m_cone2 - m_carPosition) < 10.f)
	{
		m_cone2 = glm::vec3(0.f, -100.0f, 0.f);
		m_damage += 10;
		m_topSpeed -= 50;
		m_speedometer -= 50;
		shake = true;
		m_shakeAngle = 0.05f;
		m_ball2 = glm::vec3(0.f, -100.f, 0.f);
	}
	if (glm::length(m_cone3 - m_carPosition) < 10.f)
	{
		m_cone3 = glm::vec3(0.f, -100.0f, 0.f);
		m_damage += 10;
		m_topSpeed -= 50;
		m_speedometer -= 50;
		shake = true;
		m_shakeAngle = 0.05f;
		m_ball3 = glm::vec3(0.f, -100.f, 0.f);
	}
	if (glm::length(m_cone4- m_carPosition) < 10.f)
	{
		m_cone4 = glm::vec3(0.f, -100.0f, 0.f);
		m_damage += 10;
		m_topSpeed -= 50;
		m_speedometer -= 50;
		shake = true;
		m_shakeAngle = 0.05f;
		shake = true;
		m_shakeAngle = 0.05f;
		m_ball4 = glm::vec3(0.f, -100.f, 0.f);
	}
	if (glm::length(m_cone5 - m_carPosition) < 10.f)
	{
		m_cone5 = glm::vec3(0.f, -100.0f, 0.f);
		m_damage += 10;
		m_topSpeed -= 50;
		m_speedometer -= 50;
		shake = true;
		m_shakeAngle = 0.05f;
		m_ball5 = glm::vec3(0.f, -100.f, 0.f);
	}
	if (glm::length(m_cone6 - m_carPosition) < 10.f)
	{
		m_cone6 = glm::vec3(0.f, -100.0f, 0.f);
		m_damage += 10;
		m_topSpeed -= 50;
		m_speedometer -= 50;
		shake = true;
		m_shakeAngle = 0.05f;
		m_ball6 = glm::vec3(0.f, -100.f, 0.f);
	}
	
	
		m_lapCount = m_pCatmullRom->CurrentLap(m_currentDistance);

}





void Game::DisplayFrameRate()
{

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
	{
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
	}

	//if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 30, 32, "FPS: %d", m_framesPerSecond);
	//}
}

void Game::DisplaySpeed()
{

	
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(20, height - (height - 30), 32, "SPEED: %d mph", m_speedometer);

}

void Game::DisplayDamage()
{

		CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right;


	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(width - 150, height - (height - 30), 32, "DAMAGE: x %d", m_damage);
	
}

void Game::DisplayLap(int lap)
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right;

	
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(20, height - 30, 32, "LAP: %d / 2", lap);

}



void Game::DisplayTime()
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(20, height - (height - 80), 32, "TIME: %d:%d:%d", hr,min,sec);

}

void Game::DisplayGameOver()
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(290, height - (height - 300), 64, "GAME OVER");

}

void Game::DisplayFinished()
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFtFont->Render(290, height - (height - 300), 64, "FINISHED");

}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/


	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();


}


WPARAM Game::Execute()
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if (!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();


	MSG msg;

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (m_appActive) {
			GameLoop();
		}
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch (LOWORD(w_param))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			m_appActive = true;
			m_pHighResolutionTimer->Start();
			break;
		case WA_INACTIVE:
			m_appActive = false;
			break;
		}
		break;
	}

	case WM_SIZE:
		RECT dimensions;
		GetClientRect(window, &dimensions);
		m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch (w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case 'W':
			//moving forward
			if (m_speedometer < m_topSpeed)
			{
				m_speedometer += 50;
			}
			
	
			break;
		case 'S':
			//moving backwards
			if (m_speedometer > 0)
			{
				m_speedometer -= 50;
			}
		
			break;
		case 'A':
			if (m_TrackPos != 0)
			{
				m_TrackPos--;
			}
			
			break;
		case 'D':
			if (m_TrackPos != 2)
			{
				m_TrackPos++;
			}
			break;
		case VK_F1:
		
			CameraView = 0;
			break;
		case VK_F2:
	
			CameraView = 1;
			break;
		case VK_F3:
		
			CameraView = 2;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance()
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance)
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int){
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
