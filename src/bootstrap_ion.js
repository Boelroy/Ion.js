(function(){
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
    
    console.log(global.env)
})(global)