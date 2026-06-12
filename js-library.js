addToLibrary({
	chset: function(y, x, c) {
		window.chset(y, x, String.fromCharCode(c));
	},
	bgset: function(y, x, bg) {
		window.bgset(y, x, bg);
	},
	fgset: function(y, x, fg) {
		window.fgset(y, x, fg);
	},
	btnp: function() {
		return window.btnp();
	},
});
