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
document.body.style.height = "100vh";
document.querySelector("html").style.overflow = "hidden";
document.body.style.overflow = "hidden";

const box = document.createElement("div");

const inp = document.createElement("input");
inp.style.position = "absolute";
inp.style.top = 0;
document.onclick = function() {
	inp.focus();
};
inp.style.opacity = 0;
box.appendChild(inp);

const table = document.createElement("table");
table.style.borderSpacing = 0;
box.appendChild(table);
const cells = Array();
for (let i = 0; i < HEIGHT; ++i) {
	const tr = document.createElement("tr");
	table.appendChild(tr);
	const row = Array();
	for (let j = 0; j < WIDTH; ++j) {
		const td = document.createElement("td");
		td.style.padding = 0;
		td.style.margin = 0;
		td.style.width = "fit-content";
		row.push(td);
		tr.appendChild(td);
	}
	cells.push(row);
}
document.body.appendChild(box);
box.style.width = "fit-content";
box.style.margin = "auto";

const boxWidthFor1pxFont = function() {
	const canvas = document.createElement("canvas");
	const context = canvas.getContext("2d");
	const testHeight = 1000;
	context.font = testHeight + "px monospace";
	const metrics = context.measureText("a".repeat(WIDTH));
	return metrics.width/testHeight;
}();

window.onresize = function() {
	const bound = Math.min(window.innerHeight, window.innerWidth);
	document.querySelectorAll("td").forEach(data => {data.style.fontSize = bound/boxWidthFor1pxFont;});
	document.querySelectorAll("tr").forEach(row => {row.style.lineHeight = bound/HEIGHT + "px";});
};
requestAnimationFrame(()=>{
	window.onresize();
});

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

function processInput() {
	while (inp.value != '') {
		const head = inp.value[0];
		inp.value = inp.value.slice(1);
		if (head == 'w' || head == 'W') {
			window.btnp(1);
		}
		else if (head == 's' || head == 'S') {
			window.btnp(2);
		}
		else if (head == 'a' || head == 'A') {
			window.btnp(3);
		}
		else if (head == 'd' || head == 'D') {
			window.btnp(4);
		}
		else if (head == 'p' || head == 'P') {
			window.btnp(5);
		}
		else if (head == 'o' || head == 'O') {
			window.btnp(6);
		}
	}
}

function frameCallback() {
	const FPS = 60;
	processInput();
	window.update();
	window.flip();
	setTimeout(frameCallback, 1000/FPS);
}

const audioCtx = new AudioContext();
const types = ["sine", "square", "triangle", "sawtooth"];
const CHANNELS = types.length;
const shares = [3/8, 1/8, 3/8, 1/8];
const osc = [];
const gain = [];
for (let i = 0; i < CHANNELS; ++i) {
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
	volume = Math.min(2, volume);
	volume /= 2;
	volume *= shares[ch];
	if (volume == 0) {
		volume = 0.0001;
	}
	gain[ch].gain.linearRampToValueAtTime(volume, audioCtx.currentTime + 0.01);
}
window.vset = vset;

const importObject = {env:{cset,bset,fset,nset,vset}};
WebAssembly.instantiateStreaming(fetch("./compiled.wasm"), importObject).then((obj) => {
	window.btnp = obj.instance.exports.call_btnp;
	window.update = obj.instance.exports.call_update;
	window.flip = obj.instance.exports.flip;
	document.body.addEventListener("click", () => {
		cls();
		frameCallback();
		if (audioCtx.state === "suspended") {
			audioCtx.resume();
		}
		for (let i = 0; i < CHANNELS; ++i) {
			osc[i].start(audioCtx.currentTime);
		}
		window.addEventListener("visibilitychange", () => {
			if (document.hidden) {
				audioCtx.suspend();
			} else {
				audioCtx.resume();
			}
		});
	}, {once: true});
});
