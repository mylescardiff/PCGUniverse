#include <Logic/GameLogic.h>
#include <Views/HumanView.h>
#include <Utilities/Rng.h>
#include <Utilities/Utilities.h>
#include <Utilities/Noise.h>
#include <Logic/Components/TransformComponent.h>
#include <PCG/NameGenerator.h>
#include <SFML/Graphics.hpp>

#if _WIN64
#include <Application/Resources/ResourceFile.h>
#endif
#include <iostream>
#include <PCG\Grammar.h>

constexpr static float kPlanetGridWidth = 100.f;
constexpr static float kPlanetGridHeight = 80.f;

GameLogic::GameLogic()
	: m_rng()
	, m_currentScene(Scene::kStarField)
	, m_pTargetMarker(nullptr)
	, m_selectedStarId(0)
	, m_pStarDisplay(nullptr)
	, m_pMessageBox(nullptr)
	, m_solIndex(0)
	, m_artifactCount(0)
	, m_finishIndex(0)
	, m_exploreMode(false)
	, m_pArtifactCount(0)
	, m_planetIndex(0)
{
	//
}

GameLogic::~GameLogic()
{
	//
}

slth::IGameLayer* GameLogic::Create()
{
	return new GameLogic;
}

bool GameLogic::Init(slth::ApplicationLayer* pAppLayer)
{
	IGameLayer::Init(pAppLayer);

	pAppLayer->GetWindow()->SetTitle("GAP 391 - Final Project");

	// load UI elements
	m_pActorFactory->CreateActors("Data\\ui.xml", m_uiElements, m_pGraphics, m_pWorld);

	// load starting actors
	m_pActorFactory->CreateActors("Data\\actors.xml", m_actors, m_pGraphics, m_pWorld);

	m_pGraphics->LoadTexture("Assets\\star-0.png");
	m_pGraphics->LoadTexture("Assets\\star-1.png");
	m_pGraphics->LoadTexture("Assets\\star-2.png");
	m_pGraphics->LoadTexture("Assets\\star-3.png");
	m_pGraphics->LoadTexture("Assets\\star-4.png");

	// init views
	HumanView* hView = new HumanView;
	hView->SetInput(pAppLayer->GetKeyboard(), pAppLayer->GetMouse());
	AddView(hView);
	hView->Init(this);

	m_pTargetMarker = FindActor("TargetMarker");
	m_pStarDisplay = FindUIElement("StarDisplay");
	m_pMessageBox = FindUIElement("Instructions");
	m_pArtifactCount = FindUIElement("ArtifactCount");

	return PostInit();

	
}

bool GameLogic::PostInit()
{
	IGameLayer::PostInit();

	ReseedRng();

	GenerateStars();

	return true;
}

void GameLogic::Update(float deltaTime)
{
	IGameLayer::Update(deltaTime);

	slth::IKeyboard* pKeyboard = m_pAppLayer->GetKeyboard();
	slth::IMouse* pMouse = m_pAppLayer->GetMouse();

	if (pKeyboard->IsKeyPressed(KeyCode::Space))
	{
		FindUIElement("Intro")->Hide();
	}

	if (pKeyboard->IsKeyPressed(KeyCode::R))
	{
		ReseedRng();
	}

	if (pKeyboard->IsKeyPressed(KeyCode::Esc))
	{
		Quit();
	}

	if (pKeyboard->IsKeyPressed(KeyCode::E))
	{
		m_exploreMode = true;
		for (GraphNode& node : m_questGraph.GetNodes())
		{
			node.SetDiscovered(true);
		}
	}

	// handle double clicks
	if (pMouse->IsDoubleClicked(slth::IMouse::MouseButton::Left))
	{
		if (m_currentScene == Scene::kStarField)
		{
			EnterStarSystem(m_selectedStarId);
		}
		else if (m_currentScene == Scene::kSolarSystem)
		{
			// deternime planet clicked 

			auto pos = pMouse->GetPosition();
			HandlePlanetClick(pos.x, pos.y);

			// transfer image data to the grid
			
			
		}
	}

	// handle single clicks
	if (pMouse->IsButtonPressed(slth::IMouse::MouseButton::Left))
	{
		if (m_currentScene == Scene::kStarField)
		{
			// star field view
			auto pos = pMouse->GetPosition();
			HandleStarClick(pos.x, pos.y);
		}
		else if (m_currentScene == Scene::kSolarSystem)
		{
			// solar system view
		}
		
	
	}

	if (pMouse->IsButtonPressed(slth::IMouse::MouseButton::Right))
	{
		if (m_currentScene == Scene::kSolarSystem)
		{
			// clear the star data
			m_stars.at(m_selectedStarId).ClearData();

			m_currentScene = Scene::kStarField;
			m_pTargetMarker->Show();
			m_pStarDisplay->Hide();
		}
		else if (m_currentScene == Scene::kPlanet)
		{
			m_currentScene = Scene::kSolarSystem;
			Star& star = m_stars.at(m_selectedStarId);
			m_pStarDisplay->GetComponent<slth::TextComponent>()->SetText(star.GetName() + " - " + star.GetTypeName());
		}
	}
}

void GameLogic::Draw()
{
	if (m_currentScene == Scene::kStarField)
	{
		// draw the quest path
		DrawStarsInternal(m_questGraph.GetEntrance(), m_pGraphics);

		// star field view
		for (auto& star : m_stars)
		{
			if (star.second.GetIndex() == m_finishIndex)
			{
				m_pGraphics->DrawCircle(star.second.GetPosition(), star.second.GetSize(), slth::Color::Green(), slth::Color::Red(), 0.f);
			}
			else
			{
				m_pGraphics->DrawCircle(star.second.GetPosition(), star.second.GetSize(), star.second.GetColor(), star.second.GetColor(), 0.f);
			}
		}
	}
	else if (m_currentScene == Scene::kSolarSystem)
	{

		Star& currentStar = m_stars.at(m_selectedStarId);
		
		std::string starTexture = "Assets\\star-" + std::to_string(static_cast<int>(currentStar.GetType())) + ".png";
		slth::RectInt drawRect((int)m_pWorld->GetSize().x - 172, 0, (int)m_pWorld->GetSize().x, (int)m_pWorld->GetSize().y);
		slth::RectInt textureRect(0, 0, 172, 765);
		m_pGraphics->DrawTexture(starTexture.c_str(), drawRect, textureRect, false, 0.f, { 0.f, 0.f });

		//draw planets
		float planetSpacing = (m_pWorld->GetSize().x - Star::kStarPortionOnScreen - 50.f) / currentStar.GetPlanets().size();

		for (int i = 0; i < currentStar.GetPlanets().size(); ++i)
		{
			Planet& planet = currentStar.GetPlanets()[i];
			planet.Draw(m_pGraphics, 1.f);
		}

	}
	else if (m_currentScene == Scene::kPlanet)
	{
		Star& currentStar = m_stars.at(m_selectedStarId);
		currentStar.GetPlanets()[m_planetIndex].Draw(m_pGraphics, 12.f);
	}
	
}

void GameLogic::DrawStarsInternal(u64 currentNodeId, slth::IGraphics* pGraphics)
{
	GraphNode& currentNode = m_questGraph[currentNodeId];
	Star& currentStar = m_stars.at(currentNode.GetStar());

	std::vector<u64>& neighbors = currentNode.OutgoingAdjacentNodes();

	for (u64 neighborId : neighbors)
	{
		GraphNode& neighborNode = m_questGraph[neighborId];
		if (neighborNode.IsDiscovered())
		{
			slth::Vector2 lineStart = m_stars.at(currentNode.GetStar()).GetPosition() + slth::Vector2(m_stars.at(currentNode.GetStar()).GetSize(), m_stars.at(currentNode.GetStar()).GetSize());
			slth::Vector2 lineEnd = m_stars.at(neighborNode.GetStar()).GetPosition() + slth::Vector2(m_stars.at(neighborNode.GetStar()).GetSize(), m_stars.at(neighborNode.GetStar()).GetSize());
			
			// draw connection
			m_pGraphics->DrawLine(lineStart, lineEnd, slth::Color::LightBlue());

			// go deeper
			DrawStarsInternal(neighborNode.GetId(), pGraphics);
		}
	}
}

void GameLogic::ReseedRng()
{
	m_seed = time(NULL);
	_LOGI("Seed: " + std::to_string(m_seed));
	m_rng.Seed(m_seed);
}

void GameLogic::GenerateStars()
{
	const u8 kHorizontalCells = static_cast<u8>(m_pWorld->GetSize().x / kUniverseCellSize);
	const u8 kVerticalCells = static_cast<u8>(m_pWorld->GetSize().y / kUniverseCellSize);

	// set up to name one of the start
	bool solFound = false;
	bool kryptonFound = false;

	m_stars.reserve((size_t)kHorizontalCells * (size_t)kVerticalCells);
	
	u32 kryptonIndex = 0;
	u64 startNodeId = 0;
	for (int y = 0; y < kVerticalCells; ++y)
	{
		for (int x = 0; x < kHorizontalCells; ++x)
		{
			slth::Vector2 randomPosition(m_rng.RandFloatInRange(0.2f, 0.8f), m_rng.RandFloatInRange(0.1f, 0.9f));
			randomPosition = randomPosition * kUniverseCellSize;
			randomPosition.x += x * kUniverseCellSize;
			randomPosition.y += y * kUniverseCellSize;

			int cellIndex = slth::Math::IndexFromCoords(x, y, kHorizontalCells);

			Star::StarType type = Star::DetermineType(m_rng);
			m_stars.emplace(cellIndex, Star(type, randomPosition, cellIndex));

			std::string name;

			if (!solFound && x >= kHorizontalCells / 2 && y >= kVerticalCells / 2)
			{
				// save Sol system as the start node for the graph, more nodes to get mapped later...
				solFound = true;
				name = "Sol";
				m_solIndex = cellIndex;
				startNodeId = m_questGraph.AddNode(static_cast<u32>(GraphNode::NodeType::kStart));
				m_questGraph[startNodeId].SetStar(cellIndex);
				m_stars.at(m_solIndex).SetType(Star::StarType::kMainSequence);
			}
			else
			{
				// generate a name
				NameGenerator nameGen;
				name = nameGen.Generate(m_rng);
			
			}
			m_stars.at(cellIndex).SetName(name);

			SetSelectedStar(m_solIndex);
		}
	}
	
	u64 lastItemNodeID = 0;
	for (int i = 0; i <= 3; ++i)
	{
		u64 talkNodeId = m_questGraph.AddNode(GraphNode::NodeType::kTalk);
		lastItemNodeID = m_questGraph.AddNode(GraphNode::NodeType::kItem);
		m_questGraph.ConnectNodes(startNodeId, talkNodeId);
		m_questGraph.ConnectNodes(talkNodeId, lastItemNodeID);
	}

	u64 finishNodeId = m_questGraph.AddNode(GraphNode::NodeType::kFinish);
	m_questGraph.ConnectNodes(lastItemNodeID, finishNodeId);
	m_questGraph[lastItemNodeID].SetType(static_cast<u32>(GraphNode::NodeType::kLock));

	m_questGraph.SetEndPoints(startNodeId, finishNodeId);

	// rules for tranformation
	m_questGraph.AddOneToTwoRule(
		static_cast<u32>(GraphNode::NodeType::kItem), 
		static_cast<u32>(GraphNode::NodeType::kTalk), 
		static_cast<u32>(GraphNode::NodeType::kItem), 0.7f, "Item -> Task + Item");

	// task diamond rule
	{
		Graph taskGraph;
		u64 room1Id = taskGraph.AddNode(GraphNode::NodeType::kTalk);
		u64 room2Id = taskGraph.AddNode(GraphNode::NodeType::kTalk);
		u64 room3Id = taskGraph.AddNode(GraphNode::NodeType::kTalk);
		u64 room4Id = taskGraph.AddNode(GraphNode::NodeType::kTalk);

		taskGraph.ConnectNodes(room1Id, room2Id);
		taskGraph.ConnectNodes(room2Id, room4Id);
		taskGraph.ConnectNodes(room1Id, room3Id);
		taskGraph.ConnectNodes(room3Id, room4Id);

		taskGraph.SetEndPoints(room1Id, room4Id);
		m_questGraph.AddComplexRule(static_cast<u32>(GraphNode::NodeType::kTalk), std::move(taskGraph), 0.3f, "Task Diamond");
	}

	// transform the graph a bit 
	for (int i = 0; i < kNumRules; ++i)
	{
		m_questGraph.RunRandomRule(m_rng);
	}

	// map the nodes to stars in the galaxy
	slth::Vector2 travelDir = { 0.f, -1.f };
	
	// start the walk through to map the stars to quest nodes
	m_questGraph[startNodeId].SetDiscovered(true);
	m_stars.at(m_solIndex).SetQuestMarker(startNodeId);
	slth::Vector2 directionTrend = slth::Vector2(-1.f, 0.f);
	for (u64 outgoingNodeId : m_questGraph[startNodeId].OutgoingAdjacentNodes())
	{
		m_questGraph[outgoingNodeId].SetDiscovered(true);
		MapStarNode(outgoingNodeId, m_solIndex, directionTrend);
		directionTrend = directionTrend.Rotate(m_rng.RandFloatInRange(0.f, 359.f));
	}

}

void GameLogic::MapStarNode(u64 thisNodeId, u32 currentStarIndex, slth::Vector2 directionTrend)
{
	const u8 kHorizontalCells = static_cast<u8>(m_pWorld->GetSize().x / kUniverseCellSize) - 1;
	const u8 kVerticalCells = static_cast<u8>(m_pWorld->GetSize().y / kUniverseCellSize) - 1;
	
	// general direction method (better)
	std::vector<int> neighboringCells = slth::Math::GetNeighbors(currentStarIndex, kHorizontalCells , kVerticalCells );
	
	// try for the trending direction first, then try others if that's not an option
	// swap the one we want to try first with the one at position 0, using the directionTrend
	slth::Vector2 targetTile = { 1.f, 1.f }; // center tile
	targetTile = targetTile + directionTrend;
	int firstCellIndex = std::clamp(slth::Math::IndexFromCoords((int)targetTile.x, (int)targetTile.y, 2), 0, (int)neighboringCells.size() - 1);
	std::swap(neighboringCells[0], neighboringCells[firstCellIndex]);

	directionTrend = directionTrend.Rotate(m_rng.RandFloatInRange(0.f, 359.f));

	u32 foundStarID = 0;
	for (int cell : neighboringCells)
	{
		if (m_stars.at(cell).GetQuestNode() == 0 != m_solIndex)
		{
			foundStarID = cell;
			break;
		}
	}

	// could not find a home, just map to some random star
	for (int outerCell : neighboringCells)
	{
		std::vector<int> neighborNeighborCell = slth::Math::GetNeighbors(outerCell, kHorizontalCells, kVerticalCells);
		for (int cell : neighborNeighborCell)
		{
			if (m_stars.at((u32)cell).GetQuestNode() == 0 && (u32)cell != m_solIndex)
			{
				foundStarID = cell;
				break;
			}
		}

	}

	
	// if this is still zero, every cell for every neightbor is filled, so this should be impossible
	assert(foundStarID != 0);

	Star& thisStar = m_stars.at(foundStarID);
	m_questGraph[thisNodeId].SetStar(foundStarID);
	thisStar.SetQuestMarker(thisNodeId);

	// let the game logic know which star is the end portal
	if (thisNodeId == m_questGraph.GetExit())
		m_finishIndex = foundStarID;

	for (u64 outgoingId : m_questGraph[thisNodeId].OutgoingAdjacentNodes())
	{
		MapStarNode(outgoingId, foundStarID, directionTrend);
	
	}
	
}

void GameLogic::HandleStarClick(u32 x, u32 y)
{
	const u32 xCell = x / (u32)kUniverseCellSize;
	const u32 yCell = y / (u32)kUniverseCellSize;

	const u8 kHorizontalCells = static_cast<u8>(m_pWorld->GetSize().x / kUniverseCellSize);

	u32 starId = slth::Math::IndexFromCoords(xCell, yCell, kHorizontalCells);

	

	SetSelectedStar(starId);
}

void GameLogic::HandlePlanetClick(u32 x, u32 y)
{
	int i = 0;
	for (Planet& planet : m_stars.at(m_selectedStarId).GetPlanets())
	{
		slth::Vector2 planetPos = planet.GetPosition();
		
		if (x > (u32)planetPos.x&& x < (u32)planetPos.x + (u32)planet.GetRadius() * 2) // TODO: Add y? do we need it?
		{
			// clicked on a planet
			m_planetIndex = i;
			for (int pixY = 0; pixY < (int)planet.GetImage().getSize().y; ++pixY)
			{
				for (int pixX = 0; pixX < (int)planet.GetImage().getSize().x; ++pixX)
				{
					auto pixel = planet.GetImage().getPixel(pixX, pixY);
					m_gridSystem.PaintTiles(pixX, pixY, slth::Color((float)pixel.r, (float)pixel.g, (float)pixel.b, (float)pixel.a), 0.f);
				}
			}

			m_currentScene = Scene::kPlanet;
			int orbitDistance = (int)((planet.GetOrbit()) * kOrbitFactor);
			int radius = (int)(planet.GetRadius() * kRadiusFactor);
			m_pStarDisplay->GetComponent<slth::TextComponent>()->SetText(
				planet.GetName() + 
				"\nOrbit: " + std::to_string(orbitDistance) + "M miles" + 
				"\nSize: " + std::to_string(radius) + "K miles"
			);
			break;
		}
		++i;
	}

	
		


}

void GameLogic::EnterStarSystem(u32 id)
{
	SetMessage("");

	if (id == m_solIndex)
	{
		// easter eggs
		m_stars.at(id).CreateSol((int)m_pWorld->GetSize().x);
	}
	else
	{
		m_stars.at(id).GeneratePlanets(static_cast<int>(m_pWorld->GetSize().x), (u32)m_seed);
	}

	slth::SFMLGraphics* pSfGraphics = dynamic_cast<slth::SFMLGraphics*>(m_pGraphics);
	for (Planet& planet : m_stars.at(id).GetPlanets())
	{
		pSfGraphics->LoadTexture(planet.GetImage(), planet.GetName().c_str());
	}

	// get the node info about this star
	u64 nodeId = m_stars.at(m_selectedStarId).GetQuestNode();

	if (nodeId == 0 && !m_exploreMode)
	{
		SetMessage("This star is unreachable");
		return;
	}

	if (nodeId > 0 && !m_exploreMode && !m_questGraph[nodeId].IsDiscovered())
	{
		SetMessage("You must find the hyperspace lane to this star first.");
		return;
	}


	if (nodeId > 0)
	{
		// check the type of node and update the quest accordingly
		GraphNode::NodeType nodeType = static_cast<GraphNode::NodeType>(m_questGraph[nodeId].GetType());

		if (nodeType == GraphNode::NodeType::kLock)
		{
			if (m_artifactCount < 3)
			{
				SetMessage("You need all 3 artifacts to open the portal");
				return;
			}
			else
			{
				SetMessage("You have safely evacuated to a new galaxy. You Win!");
				FindUIElement("Win")->Show();
			}
		}
		else
		{
			int numPlanets = (int)m_stars.at(m_selectedStarId).GetPlanets().size() - 1;
			int randPlanetIndex = m_rng.RandIntInRange(0, numPlanets);
			const std::string& planetName = m_stars.at(m_selectedStarId).GetPlanets()[(size_t)randPlanetIndex].GetName();
			std::string message = "";
			if (nodeType == GraphNode::NodeType::kItem)
			{
				message = "You found an alien artifact in orbit around " + planetName;
				++m_artifactCount;
				m_pArtifactCount->GetComponent<slth::TextComponent>()->SetText("Artifact Count:" + std::to_string(m_artifactCount) + " / 3");
				
			}
			else if (nodeType == GraphNode::NodeType::kTalk)
			{
				message = "You're being hailed by " + planetName + ", coordinates for new hyperspace lanes received";
			}
			SetMessage(message);
			
		}

		// change this type to none so no action is triggered next time it's entered.
		m_questGraph[nodeId].SetType(static_cast<u32>(GraphNode::NodeType::kNone));

		for (u64 outgoingNodeId : m_questGraph[nodeId].OutgoingAdjacentNodes())
		{
			m_questGraph[outgoingNodeId].SetDiscovered(true);
		}
	}

	m_currentScene = Scene::kSolarSystem;
	m_pTargetMarker->Hide();
	m_pStarDisplay->Show();

}

void GameLogic::SetSelectedStar(u32 id)
{
	Star& star = m_stars.at(id);
	m_selectedStarId = id;

	// move the target reticle over the star with offset
	slth::TransformComponent* pTargetTransform = m_pTargetMarker->GetComponent<slth::TransformComponent>();
	pTargetTransform->SetPosition(
		{
			(star.GetPosition().x + star.GetSize()) - pTargetTransform->GetSize().x / 2,
			(star.GetPosition().y + star.GetSize()) - pTargetTransform->GetSize().y / 2,
		}
	);

	// set the text of the actor with the stars name
	m_pTargetMarker->GetComponent<slth::TextComponent>()->SetText(star.GetName());
	m_pStarDisplay->GetComponent<slth::TextComponent>()->SetText(star.GetName() + " - " + star.GetTypeName());
}



