alt.TextDecoder.prototype.decode = function (input = new Uint8Array(0), options = {}) {
    if (this.__encoding !== "utf-8") return;

    // todo: don't ignore options

    let str = "";
    let i = 0;
    while (i < input.length) {
        let byte = input[i];
        let codePoint = 0;

        if (byte < 128) {
            codePoint = byte;
        } else if (byte >= 192 && byte < 224) {
            codePoint = ((byte & 31) << 6) | (input[i + 1] & 63);
            i += 1;
        } else if (byte >= 224 && byte < 240) {
            codePoint = ((byte & 15) << 12) | ((input[i + 1] & 63) << 6) | (input[i + 2] & 63);
            i += 2;
        } else if (byte >= 240 && byte < 248) {
            codePoint = ((byte & 7) << 18) | ((input[i + 1] & 63) << 12) | ((input[i + 2] & 63) << 6) | (input[i + 3] & 63);
            i += 3;
        }

        str += String.fromCodePoint(codePoint);
        i++;
    }

    return str;
};

globalThis.TextDecoder = alt.TextDecoder;
