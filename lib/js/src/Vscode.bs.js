// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Caml_chrome_debugger = require("bs-platform/lib/js/caml_chrome_debugger.js");

function onMessage(callback) {
  window.addEventListener("message", (function (_event) {
          return Curry._1(callback, _event.data);
        }));
  return /* () */0;
}

var Api = {
  onMessage: onMessage
};

var Disposable = { };

var $$Event = { };

var ExtensionContext = { };

var simple = { };

function sized(v) {
  return v;
}

var Layout = {
  simple: simple,
  sized: sized
};

var Commands = {
  Layout: Layout
};

function makeChange(authority, fragment, path, query, scheme, param) {
  return {
          authority: authority,
          fragment: fragment,
          path: path,
          query: query,
          scheme: scheme
        };
}

var Uri = {
  makeChange: makeChange
};

var ViewColumn = { };

var WebviewOptions = { };

var Webview = { };

function single(uri) {
  return uri;
}

function both(dark, light) {
  return {
          dark: dark,
          light: light
        };
}

function classify(param) {
  if ((v.dark === undefined)) {
    return /* Single */Caml_chrome_debugger.variant("Single", 0, [param]);
  } else {
    return /* Both */Caml_chrome_debugger.variant("Both", 1, [
              param,
              param
            ]);
  }
}

var IconPath = {
  single: single,
  both: both,
  classify: classify
};

var Options = { };

var WebviewPanel = {
  IconPath: IconPath,
  Options: Options
};

var TextDocument = { };

var TextEditor = { };

var InputBoxOptions = { };

var CancellationToken = { };

function make(enableCommandUris, enableScripts, localResourceRoots, portMapping, enableFindWidget, retainContextWhenHidden, param) {
  return {
          enableCommandUris: enableCommandUris,
          enableScripts: enableScripts,
          localResourceRoots: localResourceRoots,
          portMapping: portMapping,
          enableFindWidget: enableFindWidget,
          retainContextWhenHidden: retainContextWhenHidden
        };
}

var WebviewAndWebviewPanelOptions = {
  make: make
};

var $$Window = {
  InputBoxOptions: InputBoxOptions,
  CancellationToken: CancellationToken,
  WebviewAndWebviewPanelOptions: WebviewAndWebviewPanelOptions
};

var Workspace = { };

exports.Api = Api;
exports.Disposable = Disposable;
exports.$$Event = $$Event;
exports.ExtensionContext = ExtensionContext;
exports.Commands = Commands;
exports.Uri = Uri;
exports.ViewColumn = ViewColumn;
exports.WebviewOptions = WebviewOptions;
exports.Webview = Webview;
exports.WebviewPanel = WebviewPanel;
exports.TextDocument = TextDocument;
exports.TextEditor = TextEditor;
exports.$$Window = $$Window;
exports.Workspace = Workspace;
/* No side effect */