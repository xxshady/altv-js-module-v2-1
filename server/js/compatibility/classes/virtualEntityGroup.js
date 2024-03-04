/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/server/index.d.ts" />
// import * as alt from "@altv/server";

/** @type {typeof import("../../../../shared/js/utils.js")} */
const { assert } = requireBinding("shared/utils.js");

requireBinding("shared/factory.js");

class VirtualEntityGroup extends alt.VirtualEntityGroup {
    constructor(...args) {
        // NOTE (xLuxy): This prevents the infinite loop caused by alt.*.create
        if (!args.length) return super();

        assert(args.length === 1, "1 arguments expected");

        return alt.VirtualEntityGroup.create({
            maxEntitiesInStream: args[0]
        });
    }
}

alt.VirtualEntityGroup.setFactory(VirtualEntityGroup);

cppBindings.registerCompatibilityExport("VirtualEntityGroup", VirtualEntityGroup);
