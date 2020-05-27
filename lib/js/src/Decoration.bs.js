// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");

function Impl(Editor) {
  var decorateHole = function (editor, range, index) {
    var background = Curry._3(Editor.Decoration.highlightBackground, editor, "editor.selectionHighlightBackground", range);
    var indexText = String(index);
    var start = Curry._1(Editor.$$Range.start, range);
    var end_ = Curry._1(Editor.$$Range.end_, range);
    var indexRange = Curry._2(Editor.$$Range.make, start, Curry._3(Editor.Point.translate, end_, 0, -2));
    var index$1 = Curry._4(Editor.Decoration.overlayText, editor, "editorLightBulb.foreground", indexText, indexRange);
    return Belt_Array.concatMany([
                background,
                index$1
              ]);
  };
  return {
          decorateHole: decorateHole
        };
}

exports.Impl = Impl;
/* No side effect */