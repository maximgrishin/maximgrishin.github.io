addToLibrary({
	cset: function(y, x, c, bg, fg) {
		window.cset(y, x, String.fromCharCode(c), bg, fg);
	},
	btnp: function() {
		return window.btnp();
	},
});
