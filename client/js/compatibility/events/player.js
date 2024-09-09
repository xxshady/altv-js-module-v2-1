/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/client/index.d.ts" />
// import * as alt from "@altv/client";

/** @type {typeof import("../../../../shared/js/compatibility/utils/events.js")} */
const { registerEventHandler } = requireBinding("shared/compatibility/utils/events.js");

registerEventHandler(alt.Enums.EventType.PLAYER_WEAPON_SHOOT_EVENT, "playerWeaponShoot", ({ weapon, totalAmmo, ammoInClip }) => {
    return [weapon, totalAmmo, ammoInClip];
});

registerEventHandler(alt.Enums.EventType.PLAYER_BULLET_HIT_EVENT, "playerBulletHit", ({ weapon, victim, pos }) => {
    return [weapon, victim, pos];
});

registerEventHandler(alt.Enums.EventType.PLAYER_START_ENTER_VEHICLE, "startEnteringVehicle", ({ player, vehicle, seat }) => {
    return [vehicle, seat, player];
});

registerEventHandler(alt.Enums.EventType.PLAYER_START_LEAVE_VEHICLE, "startLeavingVehicle", ({ player, vehicle, seat }) => {
    return [vehicle, seat, player];
});

registerEventHandler(alt.Enums.EventType.PLAYER_WEAPON_CHANGE, "playerWeaponChange", ({ oldWeapon, newWeapon }) => {
    return [oldWeapon, newWeapon];
});

registerEventHandler(alt.Enums.EventType.PLAYER_ENTER_VEHICLE, "enteredVehicle", ({ vehicle, seat }) => {
    return [vehicle, seat];
});

registerEventHandler(alt.Enums.EventType.PLAYER_LEAVE_VEHICLE, "leftVehicle", ({ vehicle, seat }) => {
    return [vehicle, seat];
});

registerEventHandler(alt.Enums.EventType.PLAYER_START_TALKING, "playerStartTalking", ({ player }) => {
    return [player];
});

registerEventHandler(alt.Enums.EventType.PLAYER_STOP_TALKING, "playerStopTalking", ({ player }) => {
    return [player];
});
