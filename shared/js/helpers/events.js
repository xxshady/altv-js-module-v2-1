/** @type {typeof import("../utils.js")} */
const { assert } = requireBinding("shared/utils.js");

/**
 * Creates a function that calls the emit function
 * with the rawEmitEnabled flag set to true.
 *
 * @param {string | Function} fn
 * @returns {(eventName: string, ...args: unknown[]) => void}
 */
export function emitRaw(fn = "emit") {
    assert(fn != null);

    return function (eventName, ...args) {
        const oldValue = alt.Events.rawEmitEnabled;

        alt.Events.rawEmitEnabled = true;
        if (typeof fn === "string") {
            this[fn](eventName, ...args);
        } else {
            fn(eventName, ...args);
        }
        alt.Events.rawEmitEnabled = oldValue;
    };
}
