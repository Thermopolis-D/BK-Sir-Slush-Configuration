#include "functions.h"
#include "variables.h"

extern void particleEmitter_func_802EFA20(ParticleEmitter *, f32, f32);
extern void subaddie_set_state_with_direction(Actor *, s32, f32, s32);
extern void func_80328FB0(Actor *, f32);
extern void func_803300C0(ActorMarker *, void *);

typedef struct {
    ActorMarker *unk0;
    f32 unk4;
    u8 pad8[0x1];
    u8 unk9;
    u8 unkA;
    u8 unkB;
} ActorLocal_chSirSlush;

Actor *chSnowman_draw(ActorMarker *marker, Gfx **gfx, Mtx **mtx, Vtx **vtx); //chSirSlush_draw
void chSnowman_update(Actor *this); //chSirSlush_update

ActorAnimationInfo chSnowmanAnimations[] = {
    {0, 0.0f},
    {ASSET_108_ANIM_SIR_SLUSH_IDLE,   0.8f},
    {ASSET_109_ANIM_SIR_SLUSH_ATTACK, 4.0f},
    {ASSET_220_ANIM_SIR_SLUSH_DIE,    1.6f}
};

enum chsnowman_state_e{
    CHSNOWMAN_STATE_1_IDLE = 1,
    CHSNOWMAN_STATE_2_ATTACK,
    CHSNOWMAN_STATE_3_DIE
};

ActorInfo chSnowman = { 
    MARKER_B1_SIR_SLUSH, ACTOR_124_SIR_SLUSH, ASSET_377_MODEL_SIR_SLUSH,
    CHSNOWMAN_STATE_1_IDLE, chSnowmanAnimations,
    chSnowman_update, actor_update_func_80326224, chSnowman_draw,
    0, 0x199, 0.0f, 0
};

void __chSnowman_spawnSnowball(ActorMarker *marker);
void __chSnowman_spawnHat(ActorMarker *marker);
void __chSnowman_deathCallback(ActorMarker *marker, ActorMarker *other_marker);
int __chSnowman_func_802E1F70(ActorMarker *marker, s32 arg1);
bool __chSnowman_isYawNearYawTarget(Actor *this, s32 max_angle_degree);
int __chSnowman_CCW_playerInProtectedZone(void);
int __chSnowman_isPlayerInAttackRange(Actor *this, s32 min_distance, s32 max_distance);
void __chSnowman_enterDeath(Actor *this);
void __chSnowman_setYawTarget(Actor *this, f32 max_rotation);
bool func_8038DD34(ActorMarker *marker);
void func_8032BC18(Actor *this);
enum map_e map_get(void);
bool maSlalom_isActive(void);
void maSnowy_incTotal(void);
bool func_8038DD14(void);
int subaddie_playerIsWithinSphere(Actor *this, s32 dist);
bool subaddie_playerIsWithinSphereAndActive(Actor *this, s32 dist);
bool fileProgressFlag_get(enum file_progress_e);
enum bsgroup_e player_movementGroup(void);
enum bswatergroup_e player_getWaterState(void);
s32 globalTimer_getTime(void);
