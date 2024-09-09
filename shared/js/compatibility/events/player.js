/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/server/index.d.ts" />
/// <reference path="../../../../types/client/index.d.ts" />
// import * as alt from "@altv/shared";

/** @type {typeof import("../utils/events.js")} */
const { registerEventHandler } = requireBinding("shared/compatibility/utils/events.js");

registerEventHandler(alt.Enums.EventType.PLAYER_CHANGE_ANIMATION_EVENT, "playerAnimationChange", ({ player, oldAnimDict, newAnimDict, oldAnimName, newAnimName }) => {
    return [player, oldAnimDict, newAnimDict, oldAnimName, newAnimName];
});
