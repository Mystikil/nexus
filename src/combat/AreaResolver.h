#ifndef FS_AREA_RESOLVER_H
#define FS_AREA_RESOLVER_H

#include <vector>
#include "../position.h"
#include "../matrixarea.h"

class Game;
class Tile;
class AreaCombat;

class AreaResolver {
public:
    explicit AreaResolver(Game& game);

    std::vector<Tile*> resolve(const Position& centerPos, const Position& targetPos, const AreaCombat* area) const;

private:
    std::vector<Tile*> getList(const MatrixArea& area, const Position& targetPos, Direction dir) const;

    Game& game;
};

#endif // FS_AREA_RESOLVER_H
