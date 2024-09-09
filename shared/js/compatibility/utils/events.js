/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/server/index.d.ts" />
/// <reference path="../../../../types/client/index.d.ts" />
// import * as alt from "@altv/shared";

requireBinding("shared/events.js");

/**
 * A Map that stores event type mappings.
 * @type {Map<alt.Enums.EventType, { oldEventName: string, contextToArgsFunc: Function }>}
 */
const eventMap = new Map();

/**
 * A Map that stores event type mappings for custom events.
 * @type {Map<alt.Enums.CustomEventType, { oldEventName: string, contextToArgsFunc: Function }>}
 */
const customEventMap = new Map();

/**
 *
 * @param {alt.Enums.EventType} eventType
 * @param {string} oldEventName
 * @param {Function} contextToArgsFunc
 */
export function registerEventHandler(eventType, oldEventName, contextToArgsFunc, custom = false) {
    const map = custom ? customEventMap : eventMap;
    map.set(eventType, { oldEventName, contextToArgsFunc });
}

export function getEventTypeFromName(eventName) {
    for (const [eventType, { oldEventName }] of eventMap) {
        if (oldEventName === eventName) return eventType;
    }

    for (const [eventType, { oldEventName }] of customEventMap) {
        if (oldEventName === eventName) return eventType;
    }
}

export function getEventArgumentConverter(eventType, custom = false) {
    const map = custom ? customEventMap : eventMap;

    return map.get(eventType)?.contextToArgsFunc;
}

/**
 * If the provided event is an Enum, it's looking for alt.Enums.EventType/CustomEventType. If it's a string, it's looking for the old event name.
 * @param {alt.Enums.EventType | string} event
 * @returns boolean
 */
export function isCustomEvent(event) {
    switch (typeof event) {
        case "string":
            for (const { oldEventName } of customEventMap.values()) {
                if (oldEventName === event) return true;
            }

            return false;
            
        case "number":
        default:
            return customEventMap.has(event);
    }
}
