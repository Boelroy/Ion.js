(function(){

  const module = {};
  global.require = function(moduleName) {
    let code = "(function(module, exports){" + fs.readFileSync(moduleName) + "})";
    const modulefn = global.vm.compile(code, moduleName);
    const exports = {};
    modulefn.apply(this, module,exports);
  }

  require("../test/test.js");
})()