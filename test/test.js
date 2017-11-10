var test = require("../test/mjsunit.js");

function onLoad() {
  test.assertInstanceof(setTimeout, Function);

  var starttime = new Date;

  setTimeout(function() {
    var endtime = new Date;
    var diff = endtime - starttime;
    if (diff < 0) diff = -diff;
    test.assertTrue(900 > diff || diff < 1000);
  }, 1000);

  var id = setTimeout(function() { test.assertTrue(false);}, 500);
  clearTimeout(id);

  var count = 0;
  var timer = setInterval(function() {
    count += 1;
    var endtime = new Date;
    var diff = endtime - starttime;
    if (diff < 0) diff = -diff;
    var t = count * 1000;
    test.assertTrue(t - 100 < diff || diff < t + 100);
    test.assertTrue(count <= 3);
    if (count == 3) {
      clearInterval(timer);
    }
  }, 1000);
}
