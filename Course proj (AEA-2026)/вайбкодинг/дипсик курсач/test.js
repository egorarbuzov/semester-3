// === Сгенерировано компилятором IVA-2026 ===
// Стандартная библиотека
function intToStr(x) { return String(x); }
function strLen(s) { return s.length; }

let a = 5;
let b = 3;
let flag = true;
let msg = "result";
function bitOr(x, y) {
  return ((x | y) & 0xFF);
}
function bitAnd(x, y) {
  return ((x & y) & 0xFF);
}
function bitNot(x) {
  return ((~x) & 0xFF);
}
// === main ===
(function() {
  let c = bitOr(a, b);
  console.log(("c = " + intToStr(c)));
  let i = 0;
  do {
    console.log(("iter " + intToStr(i)));
    i = (i + 1);
  } while ((i < 3));
  if (flag) {
    console.log("flag is true");
  } else {
    console.log("flag is false");
  }
})();
