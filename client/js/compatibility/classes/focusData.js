/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/client/index.d.ts" />
// import * as alt from "@altv/client";

/** @type {typeof import("../../../../shared/js/utils.js")} */
const { isVector3 } = requireBinding("shared/utils.js");

class FocusData {
    static get isFocusOverriden() {
        return alt.FocusData.isFocusOverriden;
    }

    static get focusOverridePos() {
        return alt.FocusData.focusOverridePos;
    }

    static get focusOverrideOffset() {
        return alt.FocusData.focusOverrideOffset;
    }

    static get focusOverrideEntity() {
        return alt.FocusData.focusOverrideEntity;
    }

    static overrideFocus(value, focusOffset) {
        if (isVector3(value)) {
            focusOffset ??= { x: 0, y: 0, z: 0 };

            alt.FocusData.overrideFocus(value, focusOffset);
            return;
        }

        alt.FocusData.focusOverrideEntity = value;
    }

    static clearFocus() {
        alt.FocusData.clearFocusOverride();
    }
}

cppBindings.registerCompatibilityExport("FocusData", FocusData);
