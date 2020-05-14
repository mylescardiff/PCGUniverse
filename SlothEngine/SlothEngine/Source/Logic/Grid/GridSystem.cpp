#include "GridSystem.h"
#include <Logic/Grid/GridTile.h>
#include <Logic/Actor.h>
#include <Logic/ActorFactory.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Components/RenderBoxComponent.h>
#include <Utilities/RectInt.h>
#include <Logic/ActorFactory.h>
#include <Common.h>
#include <Utilities/Utilities.h>
#if _DEBUG_DRAW
#include <Logic/Components/TextComponent.h>
#endif
using slth::GridSystem;

GridSystem::GridSystem()
	: m_tileSpacing(0)
	, m_hidden(true)
{
	m_invalidTile.SetType(kInvalidTile);
}

bool slth::GridSystem::Init(slth::Vector2 size, slth::Vector2 position, slth::Vector2 tileSize, float tileSpacing, ActorFactory* pActorFactory, std::map<Id32, slth::Actor*>& actorList)
{
	m_size = size;
	m_position = position;
	m_tileSize = tileSize;
	m_tileSpacing = (u32)tileSpacing;
	m_noTileColor = Color(255.f, 255.f, 255.f, 0.5f);

	for (int y = 0; y < (int)m_size.y; ++y)
	{
		for (int x = 0; x < (int)m_size.x; ++x)
		{
			// figure out where the tile should be 
			float tilePosX = m_tileSize.x * (float)x + (float)x + m_position.x; // add x for 1px of space betweenz
			float tilePosY = m_tileSize.y * (float)y + (float)y + m_position.y;

			// create an actor
			Actor* pActor = pActorFactory->CreateBlockActor("GridTile", { tilePosX, tilePosY }, tileSize, Color::White(), actorList);
			
			pActor->SetVisible(!m_hidden);

#if _DEBUG_DRAW
			TextComponent* pTextComponent = new TextComponent();
			pTextComponent->SetText(std::to_string(Math::IndexFromCoords(x, y, m_size.x)));
			
			pActor->AddComponent(pTextComponent);
#endif

			// create and place the tile
			GridTile pTile = GridTile(x, y, 1.f, m_noTileColor);
			pTile.SetActor(pActor);
			m_tiles.push_back(pTile);
		}
	}
	return true;
}

slth::GridTile& slth::GridSystem::GetCenterTile()
{
	if (m_tiles.size() == 0)
		return m_invalidTile;

	return GetTile((u8)m_size.x / 2, (u8)m_size.y / 2);
}

slth::GridTile& slth::GridSystem::GetTile(int x, int y)
{
	// check for off map negatives
	if (x < 0 || y < 0 || x >= m_size.x || y >= m_size.y)
		return m_invalidTile;

	size_t index = IndexFromCoords(x, y);
	return GetTile(index);
}

slth::GridTile& slth::GridSystem::GetTile(const size_t index)
{
	if (index < m_tiles.size())
	{
		return m_tiles[index];
	}
	else
	{
		return m_invalidTile;
	}
}

slth::Vector2 slth::GridSystem::GetScreenPositionFromTile(int x, int y)
{
	return Vector2(
		m_position.x + (m_tileSize.x + m_tileSpacing) * x,
		m_position.y + (m_tileSize.y + m_tileSpacing) * y
	);
}

slth::RectInt slth::GridSystem::GetRect(int x, int y)
{
	Vector2 screenCoords = GetScreenPositionFromTile(x, y);
	return RectInt(
		static_cast<int>(screenCoords.x),
		static_cast<int>(screenCoords.y),
		static_cast<int>(screenCoords.x + m_tileSize.x),
		static_cast<int>(screenCoords.y + m_tileSize.y)
	);
}

void slth::GridSystem::PaintTiles(int startX, int startY, Color color, float weight, int endX, int endY, u32 identifier, u32 type)
{
	// if end x and y aren't supplied, assume they are just painting a single tile in that direction
	if (endX == -1)
		endX = startX;

	if (endY == -1)
		endY = startY;

	Vector2 cursorPos((float)startX, (float)startY);

	while (cursorPos.x <= endX)
	{
		while (cursorPos.y <= endY)
		{
			// paint!
			GridTile& pTile = GetTile(static_cast<int>(cursorPos.x), static_cast<int>(cursorPos.y));
			if (pTile.GetType() != slth::GridSystem::kInvalidTile && cursorPos.x >= 0 && cursorPos.y >= 0 && cursorPos.x < m_size.x && cursorPos.y < m_size.y)
			{
				pTile.SetColor(color);
				pTile.SetWeight(weight);
				pTile.SetIdentifier(identifier);
				pTile.SetType(type);
			}
			++cursorPos.y;
		}
		++cursorPos.x;
		cursorPos.y = static_cast<float>(startY);
		
	}
}

int slth::GridSystem::IndexFromCoords(int x, int y) const
{
	return Math::IndexFromCoords(x, y, (int)m_size.x);
}

slth::Vector2 slth::GridSystem::CoordsFromIndex(int index) const
{
	return Math::CoordsFromIndex(index, (int)m_size.x);
}

void slth::GridSystem::ToggleVisible()
{
	m_hidden = !m_hidden;
	for (GridTile& tile : m_tiles)
	{
		if (tile.GetActor())
			tile.GetActor()->SetVisible(!m_hidden);
	}
}


void slth::GridSystem::PaintNeighbors(size_t index, u32 state, slth::Color color, bool includeDiagonal)
{
	//TODO: Fix this
}