(function(){

  const internalModuleCache = {};

  class Module {
    constructor(id, parent) {
      this.id = id;
      this.exports = {};
      this.parent = parent;

      if (parent) {
        this.moduleCache = parent.moduleCache;
      } else {
        this.moduleCache = {};
      }

      this.filename = null;
      this.loaded = false;
      this.exited = false;
      this.children = [];
    }
  }

  function createInternalModule(id, constructor) {
    var m = new Module(id);
    constructor(m.exports);
    m.loaded = true;
    internalModuleCache[id] = m;
    return m;
  }

  function loadNative(id) {
    var m = new Module(id);
    internalModuleCache[id] = m;
    var e = m._compile("" , id); // TODO::
    if (e) throw e;
    m.loaded = true;
    return m;
  }

  function findScript(base_directory, name, callback) {
    // var filename = base_directory + '/' + name;
    // fs.exists(filename, function() {
    //   callback(status ? filename : null);
    // });
    callback(name);
  }

  function Sub(name, target) {
    this.name = name;
    this.target = target;

    this.load = function(base_directory, callback) {
      findScript(base_directory, name, function(filename) {
        if (filename === null) {
          throw new Exception("Cannot find a script matching" + name);
        }
        loadScript(filename, target, callback);
      });
    }
  }

  function Scaffold(source, filename, module) {

    var source = "(function (module, exports) {" 
                + "  var onLoad;"
                + "  var exports = this;"
                + "  var require = this.__require;"
                + "  var include = this.__include;"
                + source
                + "  this.__onLoad = onLoad;"
                + "})";
    try {
      var compiled = global.vm.compile(source, filename);
    } catch (e) {
      console.log("Exception:");
    }

    module.__subs = [];
    module.__require = function(name) {
      var target = {};
      module.__subs.push(new Sub(name, target));
      return target;
    }
    module.__include = function(name) {
      module.__subs.push(new Sub(name, module));
    }
    compiled.apply(module, [filename]);

    this.filename = filename;
    this.module = module;
    this.subs = module.__subs.slice();
    this.onLoad = module.__onLoad;

    delete module.__subs;
    delete module.__onLoad;
    delete module.__require;
    delete module.__include;
  }

  function loadScript(filename, target, callback) {
    var content = global.fs.readFileSync(filename);

    var scaffold = new Scaffold(content, filename, target);

    function finish() {
      if (scaffold.onLoad instanceof Function) {
        //node.debug("calling onLoad for <" + filename + ">"); 
        scaffold.onLoad(); 
      }

      if (callback instanceof Function) {
        callback();
      }
    }
    if (scaffold.subs.length == 0) {
      finish();
    } else {
      var ncomplete = 0;
      for (var i = 0; i < scaffold.subs.length; i++) {
        var sub = scaffold.subs[i];
        sub.load(filename, function() {
          ncomplete += 1;
          if (ncomplete === scaffold.subs.length) {
            finish();
          }
        })
      }
    }
  }

  const moduleCache = {};
  
  global.require = function(moduleName) {
    if (moduleCache[moduleName]) return moduleCache[moduleName];

    let code = "(function(exports){" + fs.readFileSync(moduleName) + "})";
    const modulefn = global.vm.compile(code, moduleName);
    const exports = {};
    modulefn.apply(this, [exports]);
    moduleCache[moduleName] = exports;
    return exports;
  }

  if (process.argv[1]) {
    loadScript(process.argv[1], global);
  }
})()