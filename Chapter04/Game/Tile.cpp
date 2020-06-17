#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(class Game* game)
	:Actor(game)
	, mParent(nullptr)
	, f(0.0f)
	, g(0.0f)
	, h(0.0f)
	, mBlocked(nullptr)
	, mSprite(nullptr)
	, mTileState(EDefault)
	, mSelected(false)
{
	mSprite = new SpriteComponent(this);
	UpdateTexture();
}

void Tile::SetTileState(TileState state) {
	mTileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect() {
	mSelected = !mSelected;
	UpdateTexture();
}

void Tile::UpdateTexture() {
	std::string text;
	switch (mTileState) {
	case EStart:
		text = "Assets/TileTan.png";
			break;
	case EBase:
		text = "Assets/TileGreen.png";
		break;
	case EPath:
		if (mSelected)
			text = "Assets/TileGraySelected.png";
		else
			text = "Assets/TileGray.png";
		break;
	case EDefault:
	default:
		if (mSelected)
			text = "Assets/TileBrownSelected.png";
		else
			text = "Assets/TileBrown.png";
		break;
	}
	mSprite->SetTexture(GetGame()->GetTexture(text));
}