function() {
  var i = 0;
  var j = 1;

  class Calculate {
    constructor(i, j) {
      this.i = i;
      this.j = j;
    }

    Add() {
      return this.i + this.j;
    }
  }

  var cal = new Calculate(i,j);
  return cal.Add();
}