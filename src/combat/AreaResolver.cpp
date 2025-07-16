#include "../otpch.h"
#include "AreaResolver.h"
#include "../combat.h"
#include "../game.h"
#include "../map.h"

AreaResolver::AreaResolver(Game& game) : game(game) {}

std::vector<Tile*> AreaResolver::getList(const MatrixArea& area, const Position& targetPos, Direction dir) const
{
    auto casterPos = getNextPosition(dir, targetPos);

    std::vector<Tile*> vec;

    auto center = area.getCenter();
    Position tmpPos(targetPos.x - center.first, targetPos.y - center.second, targetPos.z);
    for (uint32_t row = 0; row < area.getRows(); ++row, ++tmpPos.y) {
        for (uint32_t col = 0; col < area.getCols(); ++col, ++tmpPos.x) {
            if (area(row, col)) {
                if (game.isSightClear(casterPos, tmpPos, true)) {
                    Tile* tile = game.map.getTile(tmpPos);
                    if (!tile) {
                        tile = new StaticTile(tmpPos.x, tmpPos.y, tmpPos.z);
                        game.map.setTile(tmpPos, tile);
                    }
                    vec.push_back(tile);
                }
            }
        }
        tmpPos.x -= area.getCols();
    }
    return vec;
}

std::vector<Tile*> AreaResolver::resolve(const Position& centerPos, const Position& targetPos, const AreaCombat* area) const
{
    if (targetPos.z >= MAP_MAX_LAYERS) {
        return {};
    }

    if (area) {
        return getList(area->getArea(centerPos, targetPos), targetPos, getDirectionTo(targetPos, centerPos));
    }

    Tile* tile = game.map.getTile(targetPos);
    if (!tile) {
        tile = new StaticTile(targetPos.x, targetPos.y, targetPos.z);
        game.map.setTile(targetPos, tile);
    }
    return {tile};
}
