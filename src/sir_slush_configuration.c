#include "modding.h"
#include "functions.h"
#include "variables.h"
#include "bksnowman.h"
#include "recomputils.h"
#include "recompconfig.h"


#define CFG_VANILLA_FLYING_DELAY (bool)recomp_get_config_u32("vanilla_flying_delay")
#define CFG_ATTACK_ENABLED (bool)recomp_get_config_u32("attack_enabled")

RECOMP_PATCH void __chSnowman_enterIdle(Actor *this){
    ActorLocal_chSirSlush *local = (ActorLocal_chSirSlush *) &this->local;
    subaddie_set_state_with_direction(this, CHSNOWMAN_STATE_1_IDLE, 0.01f, 1);
    actor_loopAnimation(this);
    //Each increment of 0.5f increases the attack time by about 1 second, with a value of 0.0f just playing the animation with no cooldown
    //Original value is 0.4f
    local->unk4 = recomp_get_config_double("seconds_delay") * 0.5f;
}

RECOMP_PATCH void chSnowman_update(Actor *this){
    ActorLocal_chSirSlush *local = (ActorLocal_chSirSlush *) &this->local;
    f32 dt;

    dt = time_getDelta();
    if(!this->volatile_initialized){
        this->volatile_initialized = TRUE;
        this->marker->propPtr->unk8_3 = 0;
        actor_collisionOn(this);
        marker_setCollisionScripts(this->marker, NULL, NULL, __chSnowman_deathCallback);
        func_803300C0(this->marker, __chSnowman_func_802E1F70);
        local->unk9 = 0;
        local->unkA = 1;
        local->unkB = 0;
        local->unk4 = 0.0f;
        anctrl_setTransitionDuration(this->anctrl, 0.8f);
        anctrl_setAnimTimer(this->anctrl, randf());
        func_8032BC18(this);
        if(map_get() == MAP_27_FP_FREEZEEZY_PEAK){
            local->unk0 = actorArray_findActorFromActorId(0x336)->marker;
            maSnowy_incTotal();
        }
    }//L802E21D8
    if(map_get() == MAP_27_FP_FREEZEEZY_PEAK){
        if(maSlalom_isActive() || func_8038DD14()){
            actor_collisionOff(this);
            this->unk58_0 = 0;
            return;
        }
        else{//L802E2224
            actor_collisionOn(this);
            this->unk58_0 = 1;

        }
    }//L802E223C
    if(!subaddie_playerIsWithinSphere(this, 4000))
        return;

    if(!local->unkB && this->marker->unk14_21){
        local->unkB = 1;
    }//L802E2280
    switch(this->state){
        case CHSNOWMAN_STATE_1_IDLE://L802E22B0
            local->unk9 = FALSE;
            local->unkA = 1;
            __chSnowman_setYawTarget(this, 6.0f);
            if(!subaddie_playerIsWithinSphereAndActive(this, 3150)){
                __chSnowman_enterDeath(this);
            }
            else if( 
                map_get() != MAP_27_FP_FREEZEEZY_PEAK
                || func_8038DD34(local->unk0) == 0
                || fileProgressFlag_get(FILEPROG_13_COMPLETED_TWINKLIES_MINIGAME)
            ){//L802E2318
                if(0.0 < local->unk4){
                    if (CFG_VANILLA_FLYING_DELAY){
                        if( (globalTimer_getTime() & 1)
                            || player_movementGroup() == BSGROUP_A_FLYING //usually decrements every other frame, changes to every frame when flying
                        ){//L802E236C
                            local->unk4 -= dt;
                        }
                    }
                    else {
                        if(globalTimer_getTime() & 1)
                        {//L802E236C
                            local->unk4 -= dt;
                        }
                    }
                }
                else{//L802E2380
                    if( __chSnowman_isPlayerInAttackRange(this, 500, 2750)
                        && __chSnowman_isYawNearYawTarget(this, 3)
                        && player_getWaterState() != BSWATERGROUP_2_UNDERWATER
                        && !__chSnowman_CCW_playerInProtectedZone()
                    ){
                        if (CFG_ATTACK_ENABLED){
                            subaddie_set_state_with_direction(this, CHSNOWMAN_STATE_2_ATTACK, 0.01f, 1); //this is what makes the snowman attack
                            actor_playAnimationOnce(this); // this is what plays the animation of the attack
                        }
                    }
                }
            }
            break;
        case CHSNOWMAN_STATE_2_ATTACK://L802E23E8
            if(!subaddie_playerIsWithinSphereAndActive(this, 3150)){
                __chSnowman_enterDeath(this);
            }//L802E240C
            else if( 
                0.98 < anctrl_getAnimTimer(this->anctrl)
                || !__chSnowman_isPlayerInAttackRange(this, 500, 2750)
                || player_getWaterState() == BSWATERGROUP_2_UNDERWATER
                || __chSnowman_CCW_playerInProtectedZone()
            ){
                __chSnowman_enterIdle(this);
            }
            else{
                if(anctrl_getAnimTimer(this->anctrl) < 0.45){
                    __chSnowman_setYawTarget(this, 6.0f);
                }
                if( actor_animationIsAt(this, 0.19f)
                    || actor_animationIsAt(this, 0.28f)
                    || actor_animationIsAt(this, 0.37f)
                ){
                    sfx_playFadeShorthandDefault(SFX_A7_WOODEN_SWOSH, 1.3f, 18000, this->position, 800, 3050);
                }//L802E24FC
                if(actor_animationIsAt(this, 0.15f)){
                    func_8030E878(SFX_3F5_UNKNOWN, randf2(1.35f, 1.5f),32000, this->position, 800.0f, 3050.0f);
                }//L802E2558

                if(actor_animationIsAt(this, 0.45f)){
                    local->unk9 = TRUE;
                }
                else if(
                    actor_animationIsAt(this, 0.58f)
                    && local->unkB
                ){
                    func_8030E878(SFX_8F_SNOWBALL_FLYING, randf2(0.95f, 1.05f), 30000, this->position, 800.0f, 3050.0f);
                    __spawnQueue_add_1((GenFunction_1)__chSnowman_spawnSnowball, (s32)this->marker);
                    local->unk9 = FALSE;
                }
                
            }
            break;
        case CHSNOWMAN_STATE_3_DIE://L802E2604
            if(subaddie_playerIsWithinSphereAndActive(this, 3150)){
                __chSnowman_enterIdle(this);
            }
            break;
    }
}
