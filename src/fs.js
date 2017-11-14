
fs.readFileSync = function(path, encoding) {
  const constants = fs.constants;
  let buffer;
  let fd = fs.open(path, constants.O_RDONLY, 0, null);
  buffer = fs.read(fd);
  fs.close(fd, null);
  return buffer;
}

fs.writeFileSync = function(fd, content, offset) {
  const constants = fs.constants;
  fs.write(fd, content, offset);
}

fs.statSync = function(path) {
  return fs.stat(path, undefined);
}

fs.renameSync = function(path, newPath) {
  return fs.rename(path, newPath, undefined);
}