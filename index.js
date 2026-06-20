const PICO8_COLORS = [
	'#000000', // black
	'#1d2b53', // dark_blue
	'#7e2553', // dark_purple
	'#008751', // dark_green
	'#ab5236', // brown
	'#5f574f', // dark_gray
	'#c2c3c7', // light_gray
	'#fff1e8', // white
	'#ff004d', // red
	'#ffa300', // orange
	'#ffec27', // yellow
	'#00e436', // green
	'#29adff', // blue
	'#83769c', // indigo
	'#ff77a8', // pink
	'#ffccaa', // peach
];

const HEIGHT = 16;
const WIDTH = 32;

document.body.style.background = PICO8_COLORS[0]
document.body.style.color = PICO8_COLORS[6]
document.body.style.margin = 0;
document.body.style.padding = 0;
document.body.style.fontFamily = "monospace";
document.body.style.whiteSpace = "nowrap";

const box = document.createElement("div");
const cells = Array();
for (let i = 0; i < HEIGHT; ++i) {
	const row = Array();
	for (let j = 0; j < WIDTH; ++j) {
		const cell = document.createElement("span");
		row.push(cell);
		box.appendChild(cell);
	}
	cells.push(row);
	box.appendChild(document.createElement("br"));
}
document.body.appendChild(box);
box.style.width = "fit-content";
box.style.margin = "auto";

const fontSizeFor1pxScreenWidth = function() {
	const canvas = document.createElement("canvas");
	const context = canvas.getContext("2d");
	const metricsHeight = 1000;
	context.font = metricsHeight + "px monospace";
	const metrics = context.measureText("a".repeat(WIDTH));
	return metricsHeight/metrics.width;
}();

window.onresize = function() {
	const currentFontSize = parseFloat(window.getComputedStyle(box, null).getPropertyValue('font-size'));
	const landscapeFontSize = currentFontSize*window.innerHeight/box.offsetHeight;
	const portraitFontSize = window.innerWidth*fontSizeFor1pxScreenWidth;
	document.body.style.fontSize = Math.min(landscapeFontSize, portraitFontSize);
};
window.onresize();

const inp = document.createElement("input");
inp.style.position = "relative";
inp.style.top = -1000;
document.addEventListener("click", () => {
	inp.focus();
});
box.appendChild(inp);

function cset(y, x, c) {
	if (y < 0 || HEIGHT <= y || x < 0 || WIDTH <= x) {
		return;
	}
	if (typeof(c) == typeof(0)) {
		c = String.fromCharCode(c);
	}
	if (c == ' ') {
		c = '\u00A0';
	}
	cells[y][x].textContent = c;
}
window.cset = cset;

function bset(y, x, bg) {
	if (y < 0 || HEIGHT <= y || x < 0 || WIDTH <= x) {
		return;
	}
	if (0 <= bg && bg < PICO8_COLORS.length) {
		cells[y][x].style.background = PICO8_COLORS[bg]
	}
}
window.bset = bset;
function fset(y, x, fg) {
	if (y < 0 || HEIGHT <= y || x < 0 || WIDTH <= x) {
		return;
	}
	if (0 <= fg && fg < PICO8_COLORS.length) {
		cells[y][x].style.color = PICO8_COLORS[fg]
	}
}
window.fset = fset;

function cls() {
	for (let y = 0; y < HEIGHT; ++y) {
		for (let x = 0; x < WIDTH; ++x) {
			cset(y, x, ' ');
			fset(y, x, 7);
			bset(y, x, 0);
		}
	}
}
cls();
for (let i = 0; i < 7; ++i) {
	cset(5 + i, 10, '|');
	fset(5 + i, 10, 1 + i);
}
for (let i = 0; i < 4; ++i) {
	cset(5 + i, 13 + 3*i, '-');
	fset(5 + i, 13 + 3*i, 8 + i);
}
for (let i = 0; i < 4; ++i) {
	cset(11 - i, 13 + 3*i, '-');
	fset(11 - i, 13 + 3*i, 14 - i);
}
cset(8, 13, 'P');
fset(8, 13, 12);
cset(8, 14, 'L');
fset(8, 14, 13);
cset(8, 15, 'A');
fset(8, 15, 14);
cset(8, 16, 'Y');
fset(8, 16, 15);

function print(y, x, msg) {
	let i = 0;
	for (c of msg) {
		cset(y, x + i, c);
		fset(y, x + i, 6);
		++i;
	}
}
print(14, 6, "controls: WASD, O, P");

function btnp() {
	if (inp.value != '') {
		const head = inp.value[0];
		inp.value = inp.value.slice(1);
		if (head == 'w' || head == 'W') {
			return 1;
		}
		if (head == 's' || head == 'S') {
			return 2;
		}
		if (head == 'a' || head == 'A') {
			return 3;
		}
		if (head == 'd' || head == 'D') {
			return 4;
		}
	}
	return 0;
}
window.btnp = btnp;

function frameCallback() {
	window.update();
	window.flip();
	requestAnimationFrame(frameCallback);
}

const audioCtx = new AudioContext();
const types = ["sine", "square", "triangle", "sawtooth"];
const shares = [3/8, 1/8, 3/8, 1/8];
const osc = [];
const gain = [];
for (let i = 0; i < 4; ++i) {
	osc.push(new OscillatorNode(audioCtx, {
	type: types[i],
	frequency: 440,
}));
	gain.push(new GainNode(audioCtx, {"gain":0.0001}));
	osc[i].connect(gain[i]).connect(audioCtx.destination);
}

function nset(ch, note) {
	volume = Math.max(0, note);
	volume = Math.min(127, note);
	osc[ch].frequency.value = (440 * (2 ** ((note - 69) / 12)));
}
window.nset = nset;

function vset(ch, volume) {
	volume = Math.max(0, volume);
	volume = Math.min(3, volume);
	volume /= 3;
	volume *= shares[ch];
	if (volume == 0) {
		volume = 0.0001;
	}
	gain[ch].gain.linearRampToValueAtTime(volume, audioCtx.currentTime + 0.01);
}
window.vset = vset;

const importObject = {env:{cset,bset,fset,btnp,nset,vset}};
WebAssembly.instantiateStreaming(fetch("./compiled.wasm"), importObject).then((obj) => {
	window.update = obj.instance.exports.call_update;
	window.flip = obj.instance.exports.flip;
	box.addEventListener("click", () => {
		cls();
		frameCallback();
		if (audioCtx.state === "suspended") {
			audioCtx.resume();
		}
		for (let i = 0; i < 4; ++i) {
			osc[i].start(audioCtx.currentTime);
		}
	}, {once: true});
});
