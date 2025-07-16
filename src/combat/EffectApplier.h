#ifndef FS_EFFECT_APPLIER_H
#define FS_EFFECT_APPLIER_H

#include "../combat.h"
#include "../game.h"

class EffectApplier {
public:
    explicit EffectApplier(Game& game);

    void applyAfterHit(Creature* caster, Creature* target, const CombatDamage& damage,
                       const CombatParams& params, Player* casterPlayer,
                       int32_t totalDamage, size_t targetsCount) const;

private:
    Game& game;
};

#endif // FS_EFFECT_APPLIER_H
