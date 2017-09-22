var console = {
  log(...args) {
    let message = args.reduce((pre, item) => {
        if (typeof item == "object") {
            item = JSON.stringify(item);
        }
        return pre + item + " ";
    }, "");
    global.Debug(message.slice(0, message.length-1));
  }
}

function setTimeout(callback, delay) {
  var timer = new global.Timer(callback);
  timer.Start(delay, 0);
  return timer;
};

function setInterval(callback, delay) {
  var timer = new global.Timer(callback);
  timer.Start(delay, delay);
  return timer;
};

function clearTimeout(timer) {
  timer.Stop();
  delete timer;
};

var clearInterval = clearTimeout;
