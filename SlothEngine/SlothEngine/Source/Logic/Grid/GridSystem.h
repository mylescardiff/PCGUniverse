#pragma once
/** \file GridSystem.h */
/** TODO: File Purpose */
// Created by Myles Cardiff

#include <Utilities/Vector2.h>
#include <map>
#include <vector>
#include <Utilities/RectInt.h>
#include <Graphics/ITexture.h>
#include <Logic/Grid/GridTile.h>

//! \namespace TODO: Namespace Description
namespace slth
{
	class ActorFactory;
	class Actor;

	class IGraphics;

/** \class GridSystem */
/** TODO: Class Purpose */
class GridSystem
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //



	static constexpr u32 kInvalidTile = 999;


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	SLTH_API GridSystem();

	

	/** Initialze the grid */
	SLTH_API bool Init(slth::Vector2 size, slth::Vector2 position, slth::Vector2 tileSize, float tileSpacing, ActorFactory* pActorFactory, std::map<Id32, slth::Actor*>& actorList);

	/** Get the center tile, or as close to it as we can */
	SLTH_API GridTile& GetCenterTile();

	/** Grabs the tile at position (x, y), returns nullptr if there isn't one */
	SLTH_API slth::GridTile& GetTile(int x, int y);

	SLTH_API slth::GridTile& GetTile(const size_t index);

	/** Get the screen position of the tile at (x, y)*/
	SLTH_API Vector2 GetScreenPositionFromTile(int x, int y);

	/** Get the drawing rectangle for the tile at (x,y)*/
	SLTH_API RectInt GetRect(int x, int y);

	/** Paint the tiles in a XXYY range, or just a single tile if endpoint is not supplied */
	SLTH_API void PaintTiles(int startX, int startY, Color color, float weight, int endX = -1, int endY = -1, u32 identifier = 0, u32 type = 0);

	SLTH_API void PaintNeighbors(size_t index, u32 state, slth::Color color, bool includeDiagonal);

	/** Hide / show the grid */
	SLTH_API void ToggleVisible();
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	Vector2 m_size; 
	Vector2 m_position;
	Vector2 m_tileSize;
	u32 m_tileSpacing;
	Color m_noTileColor;
	std::vector<GridTile> m_tiles;
	GridTile m_invalidTile;
	bool m_hidden; 

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

	/** Convert coordinates to an index */
	SLTH_API int IndexFromCoords(int x, int y) const;

	/** Convert index to coordinates */
	SLTH_API Vector2 CoordsFromIndex(int index) const;


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API std::vector<GridTile>& GetTiles() { return m_tiles; }
	
	SLTH_API Vector2 GetSize() { return m_size; }

	SLTH_API Color GetNoTileColor() { return m_noTileColor; }

	SLTH_API GridTile& InvalidTile() { return m_invalidTile; }


};
}