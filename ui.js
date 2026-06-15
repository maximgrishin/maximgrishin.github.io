addToLibrary({
	cset: function(y, x, c) {
		window.cset(y, x, String.fromCharCode(c));
	},
	bset: function(y, x, b) {
		window.bset(y, x, b);
	},
	fset: function(y, x, f) {
		window.fset(y, x, f);
	},
	btnp: function() {
		return window.btnp();
	},
	nset: function(c, n) {
		return window.nset(c, n);
	},
	vset: function(c, v) {
		return window.vset(c, v);
	},
});
