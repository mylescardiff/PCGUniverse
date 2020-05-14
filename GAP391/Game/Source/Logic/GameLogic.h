#pragma once
/** \file GameApp.h */

// Created by Myles Cardiff

// --------------------------------------------------------------------- //
// GAP 391  - Final Project Notes
// --------------------------------------------------------------------- //
//
// PCG METHODS USED
//
//	*	Random Noise: each solar system uses the a seed generated using 2D 
//		noise based on the master seed.
//
//	*	Perlin Noise: planet surfaces are perlin noise for height with some
//		tuning based on dominant soil element (iron, sulfer, potasium, etc)
//		that dictates overall color, water level and temperature based on
//		distance from the sun. 
//	
//	*	Graph Transformation: The quest line is set with basic start / end
//		points and morphed using a few different rules for expansion. I had
//		hoped to make this a little more complex.
//
//	*	Graph Visualization: The graph tries to trend in the direction it's 
//		drawing in and has some success, it's a definite improvement over the
//		radial system I had in the assignment.
//
//	*	L-System: Star and some planet names use an L-System generator. I
//		chose not to name every planet, so you'll see [StarName] [RomanNumeral]
//		format in addition to named planets. 
//
// CHALLENGES
//
//	*	I was storing pixel colors and drawing them every frame, that's been
//		fixed and now it generates an image (waaaaay faster), but there was 
//		some bug creating a pure texture that took me forever to get around, and 
//		finally had to use sf::Image and setPixel to create my graphics rather 
//		than load in an array of UInt8s.
//	
// KNOWN ISSUES / FUTURE IMPROVEMENTS
// 
//	*	You can now zoom down the the planet surface, the plan was to have cloud 
//		layers, plant life, etc and maybe some animation but yeah life got in
//		the way here, and I ran out of time.
// 
//  *	There may be a bug in mapping my graph nodes to the stars, occasinally 
//		it jumps to a distant star because it can't find a neighbor, and I'm 
//		99% sure its an off-by-one error somewhere but I ran out of time to
//		track it down. It doesn't break anything it just makes for slightly less
//		pretty graph visualization.
//
//	*	Gameplay is a little basic, I would have like to have a more complex, 
//		varied quest, e.g. maybe there are resources you're mining, or something. 

#include <Application/ApplicationLayer.h>
#include <Logic/IGameLayer.h>
#include <Logic/Actor.h>
#include <Logic/ActorFactory.h>
#include <Utilities/Rng.h>
#include <PCG/Grammar.h>
#include <Logic/Components/TextComponent.h>
#include <Graph/Graph.h>
#include <Universe/Star.h>

#undef _DEBUG_DRAW
#define _DEBUG_DRAW 1


/** \class GameLogic */
class GameLogic
	: public slth::IGameLayer
{

public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

	enum class Scene
	{
		kStarField,
		kSolarSystem,
		kPlanet
	};
	
	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	
	GameLogic();

	virtual ~GameLogic();
	
	virtual IGameLayer* Create() override;

	const char* GetGameName() const override{ return "GAP 391 - Final Project"; }
	
	virtual bool Init(slth::ApplicationLayer* pAppLayer) override;
	
	virtual bool PostInit() override;
	
	virtual void Update(float deltaTime) override;
	
	virtual void Draw() override;


protected:

	// --------------------------------------------------------------------- //
	// Constants / Tunables
	// --------------------------------------------------------------------- //

	static constexpr float kUniverseCellSize = 75.f;
	static constexpr float kStarZoomFactor = 100.f;
	static constexpr int kNumRules = 15;
	constexpr static float kOrbitFactor = 0.232f;
	constexpr static float kRadiusFactor = 0.71f;

	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	
	// rng
	slth::Rng m_rng;
	u64 m_seed = 0;

	// general
	Scene m_currentScene;
	bool m_exploreMode;

	// star field scene
	std::unordered_map<u32, Star> m_stars;
	
	u32 m_selectedStarId;
	u32 m_solIndex;
	u32 m_finishIndex;

	// quest graph
	Graph m_questGraph;
	int m_artifactCount;
	std::string portalStarName; 

	// planet surface
	u8 m_planetIndex;

	// tracked actors
	slth::Actor* m_pTargetMarker;
	slth::Actor* m_pStarDisplay;
	slth::Actor* m_pMessageBox;
	slth::Actor* m_pArtifactCount;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
	
	/** Generate all stars in the universe */
	void GenerateStars();

	/** Detect which star was clicked on */
	void HandleStarClick(u32 x, u32 y);

	/** Detect which planet was clicked on */
	void HandlePlanetClick(u32 x, u32 y);

	/** Load solar system planet view*/
	void EnterStarSystem(u32 id);

	/** Put the target marker over the selected star */
	void SetSelectedStar(u32 id);

	/** Recursive function for mapptin the quest nodes to planets */
	void MapStarNode(u64 thisNodeId, u32 currentStarIndex, slth::Vector2 directionTrend);

	/** Recersive function for drawing the hyperspace lanes */
	void DrawStarsInternal(u64 currentNodeId, slth::IGraphics* pGraphics);

	/** Reseed the RNG */
	virtual void ReseedRng();
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
	
	void SetScene(Scene scene) { m_currentScene = scene; }
	void SetMessage(std::string& message) { m_pMessageBox->GetComponent<slth::TextComponent>()->SetText(message); }
	void SetMessage(const char* message) { m_pMessageBox->GetComponent<slth::TextComponent>()->SetText(std::string(message)); }
};

