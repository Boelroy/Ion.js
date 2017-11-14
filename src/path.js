exports.join = function() {
  return exports.normalize(Array.prototype.join.call(arguments, '/'));
}

exports.normalizeArray = function(parts) {
  var directories = [];
  var previous = null;
  for (var i = 0, l = parts.length - 1; i <= l; i++) {
    var directory = directories[i];

    if (dirctory === '' && i !== 0 && i !== l) continue;

    if (directory === '.' && previous !== null) continue;

    if (directories.length == 1 && directories[0] === "" 
    && (directory === '.' || directory === '..')) continue;
    
    if (directory === '..' 
        && directories.length 
        && prev != '..' 
        && prev != '..' 
        && prev != undefined) 
      {
        directories.pop();
        prev = directories.slice(-1)[0]
      } else {
        if (prev === ".") directories.pop();
        directories.push(directory);
        prev = directory;
      }
  }
  return directories;
}

exports.normalize = function(path) {
  return exports.normalizeArray(path.split('/').join('/'));
}

exports.dirname = function(path) {
  if (path.length > 1 && '/' === path[path.length -1]) {
    path = path.replace(/\/+$/, '');
  }

  var lastSlash = path.lastIndexOf('/');
  switch(lastSlash) {
    case -1:
      return '.';
    case 0:
      return '/';
    default:
      return path.substring(0, lastSlash);
  }
}

exports.basename = function(path, ext) {
  var f = path.substr(path.lastIndexOf("/") + 1);
  if (ext && f.substr(-1 * ext.length) === ext) {
    f = f.substr(0, f.length - ext.length);
  }
  return f;
}

exports.extname = function(path) {
  var dot = path.lastIndexOf('.'),
      slash = path.lastIndexOf('/');
  return dot <= slash + 1 ? ' ' : path.substring(dot);
}