/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/server/index.d.ts" />
// import * as alt from "@altv/server";

/** @type {typeof import("../../../../shared/js/compatibility/utils/events.js")} */
const { registerEventHandler } = requireBinding("shared/compatibility/utils/events.js");

registerEventHandler(alt.Enums.EventType.PLAYER_CONNECT, "playerConnect", ({ player }) => {
    return [player];
});

registerEventHandler(alt.Enums.EventType.PLAYER_CONNECT_DENIED, "playerConnectDenied", ({ reason, name, ip, passwordHash, isDebug, branch, version, cdnUrl, discordId }) => {
    return [reason, name, ip, passwordHash, isDebug, branch, version, cdnUrl, discordId];
});

registerEventHandler(alt.Enums.EventType.PLAYER_DISCONNECT, "playerDisconnect", ({ player, reason }) => {
    return [player, reason];
});

registerEventHandler(alt.Enums.EventType.PLAYER_DAMAGE, "playerDamage", ({ player, attacker, healthDamage, armourDamage, weaponHash }) => {
    return [player, attacker, healthDamage, armourDamage, weaponHash];
});

registerEventHandler(alt.Enums.EventType.PLAYER_DEATH, "playerDeath", ({ player, killer, weaponHash }) => {
    return [player, killer, weaponHash];
});

registerEventHandler(alt.Enums.EventType.PLAYER_HEAL, "playerHeal", ({ player, oldHealth, newHealth, oldArmour, newArmour }) => {
    return [player, oldHealth, newHealth, oldArmour, newArmour];
});

registerEventHandler(alt.Enums.EventType.PLAYER_REQUEST_CONTROL, "playerRequestControl", ({ player, target }) => {
    return [player, target];
});

registerEventHandler(alt.Enums.EventType.PLAYER_CHANGE_INTERIOR_EVENT, "playerInteriorChange", ({ player, oldInterior, newInterior }) => {
    return [player, oldInterior, newInterior];
});

registerEventHandler(alt.Enums.EventType.PLAYER_DIMENSION_CHANGE, "playerDimensionChange", ({ player, oldDimension, newDimension }) => {
    return [player, oldDimension, newDimension];
});

registerEventHandler(alt.Enums.EventType.PLAYER_WEAPON_CHANGE, "playerWeaponChange", ({ player, oldWeapon, newWeapon }) => {
    return [player, oldWeapon, newWeapon];
});

registerEventHandler(alt.Enums.EventType.REQUEST_SYNCED_SCENE, "requestSyncedScene", ({ player, sceneID }) => {
    return [player, sceneID];
});

registerEventHandler(alt.Enums.EventType.START_SYNCED_SCENE, "startSyncedScene", ({ player, sceneID, startPos, startRot, animDict, entityAndAnimHashPairs }) => {
    return [player, sceneID, startPos, startRot, animDict, entityAndAnimHashPairs];
});

registerEventHandler(alt.Enums.EventType.STOP_SYNCED_SCENE, "stopSyncedScene", ({ player, sceneID }) => {
    return [player, sceneID];
});
registerEventHandler(alt.Enums.EventType.UPDATE_SYNCED_SCENE, "updateSyncedScene", ({ player, sceneID, startRate }) => {
    return [player, startRate, sceneID];
});

registerEventHandler(alt.Enums.EventType.PLAYER_SPAWN, "playerSpawn", ({ player }) => {
    return [player];
});

registerEventHandler(alt.Enums.PLAYER_ENTER_VEHICLE, "playerEnteredVehicle", ({ player, vehicle, seat }) => {
    return [player, vehicle, seat];
});

registerEventHandler(alt.Enums.EventType.PLAYER_ENTERING_VEHICLE, "playerEnteringVehicle", ({ player, vehicle, seat }) => {
    return [player, vehicle, seat];
});

registerEventHandler(alt.Enums.EventType.PLAYER_LEAVE_VEHICLE, "playerLeftVehicle", ({ player, vehicle, seat }) => {
    return [player, vehicle, seat];
});

registerEventHandler(alt.Enums.EventType.PLAYER_CHANGE_VEHICLE_SEAT, "playerChangedVehicleSeat", ({ player, vehicle, oldSeat, newSeat }) => {
    return [player, vehicle, oldSeat, newSeat];
});
