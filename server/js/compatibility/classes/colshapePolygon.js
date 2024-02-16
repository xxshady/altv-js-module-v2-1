/// <reference path="../../../../types/shared/index.d.ts" />
/// <reference path="../../../../types/server/index.d.ts" />
// import * as alt from "@altv/server";

class ColshapePolygon extends alt.ColShape {
    constructor(...args) {
        if (!args.length) return super();

        const [minZ, maxZ, points] = args;

        return alt.ColShapePolygon.create({
            minZ,
            maxZ,
            points
        });
    }
}

cppBindings.registerCompatibilityExport("ColshapePolygon", ColshapePolygon);
