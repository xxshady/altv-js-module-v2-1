/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/server/index.d.ts" />
// import * as alt from "@altv/server";

requireBinding("shared/entity.js");

const { extendAltEntityClass } = requireBinding("shared/compatibility/utils/classes.js");

class AreaBlip {
    constructor(...args) {
        if (!args.length) return this;

        const [x, y, z, width, height, global] = args;

        return alt.AreaBlip.create({
            pos: { x, y, z },
            scale: { x: width, y: height },
            global
        });
    }
}

extendAltEntityClass(AreaBlip);

cppBindings.registerCompatibilityExport("AreaBlip", AreaBlip);
