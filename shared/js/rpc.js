/** @type {typeof import("./utils.js")} */
const { assert, assertIsType } = requireBinding("shared/utils.js");
requireBinding("shared/events/rpc.js");

/**
 * @type {Map<string, RPCHandler>}
 */
const rpcHandlersMap = new Map();

class RPCHandler {
    #name;
    #handler;
    #valid = true;

    constructor(name, handler) {
        this.#name = name;
        this.#handler = handler;
    }

    destroy() {
        if (!this.#valid) return;
        this.#valid = false;
        rpcHandlersMap.delete(this.#name);
    }

    get name() {
        return this.#name;
    }
    get handler() {
        return this.#handler;
    }
    get valid() {
        return this.#valid;
    }
}

// Register RPC handler
alt.RPC.register = function (rpcName, handler) {
    assertIsType(rpcName, "string", "rpcName has to be a string");
    assertIsType(handler, "function", "handler has to be a function");
    assert(!rpcHandlersMap.has(rpcName), `Handler for rpc '${rpcName}' already registered`);

    const rpcHandler = new RPCHandler(rpcName, handler);
    rpcHandlersMap.set(rpcName, rpcHandler);
    return rpcHandler;
};

// Receive RPC event
async function onReceiveRpc(ctx) {
    const { answerID, name, player, args } = ctx;

    const rpcHandler = rpcHandlersMap.get(name);
    if (!rpcHandler) return;

    let isAsync = false;
    try {
        let funcArgs;
        if (alt.isServer) funcArgs = [player, ...args];
        else funcArgs = args;

        let result = rpcHandler.handler(...funcArgs);
        if (result instanceof Promise) {
            ctx.willAnswer();
            isAsync = true;
            result = await result;
            cppBindings.answerRPC(answerID, result, "", player);
        } else ctx.answer(result);
    } catch (e) {
        if (isAsync) cppBindings.answerRPC(answerID, undefined, e.message, player);
        else ctx.answerWithError(e.message);
    }
}
alt.Events.onScriptRPC(onReceiveRpc);

/**
 * Called when a RPC answer is received.
 * @param {Map<number, { resolve: Function, reject: Function }>} rpcMap
 */
export function answerRpc(ctx, rpcMap) {
    const sentRpc = rpcMap.get(ctx.answerID);
    if (!sentRpc) return;

    if (ctx.answerError.length !== 0) sentRpc.reject(ctx.answerError);
    else sentRpc.resolve(ctx.answer);

    rpcMap.delete(ctx.answerID);
}

/**
 * Used to send an RPC call.
 * @returns {{ answerID: number, promise: Promise, resolve: Function, reject: Function }}
 */
export function sendRpc(rpcName, ...args) {
    const result = {};

    const answerID = cppBindings.sendRPC(rpcName, ...args);
    const promise = new Promise((resolve, reject) => {
        result.resolve = resolve;
        result.reject = reject;
    });

    result.answerID = answerID;
    result.promise = promise;

    return result;
}
