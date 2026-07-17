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
document.body.style.height = "100%";
document.querySelector("html").style.overflow = "hidden";
document.body.style.overflow = "hidden";
document.body.style.userSelect = "none";
document.body.style.touchAction = "none";

const box = document.createElement("div");

const inp = document.createElement("input");
inp.style.position = "absolute";
inp.style.top = 0;
function focus() {
	inp.focus();
}
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
		td.y = i;
		td.x = j;
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
	document.querySelectorAll("td").forEach(data => {data.style.fontSize = bound/boxWidthFor1pxFont + "px";});
	document.querySelectorAll("tr").forEach(row => {row.style.lineHeight = bound/HEIGHT + "px";});
};

function cset(x, y, c) {
	if (x < 0 || WIDTH <= x || y < 0 || HEIGHT <= y) {
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

function bset(x, y, bg) {
	if (x < 0 || WIDTH <= x || y < 0 || HEIGHT <= y) {
		return;
	}
	if (0 <= bg && bg < PICO8_COLORS.length) {
		cells[y][x].style.background = PICO8_COLORS[bg]
	}
}

function fset(x, y, fg) {
	if (x < 0 || WIDTH <= x || y < 0 || HEIGHT <= y) {
		return;
	}
	if (0 <= fg && fg < PICO8_COLORS.length) {
		cells[y][x].style.color = PICO8_COLORS[fg]
	}
}

function processInput(callback) {
	while (inp.value != '') {
		callback(inp.value.charCodeAt(0));
		inp.value = inp.value.slice(1);
	}
}

let audioCtx;
const types = ["square", "square"];
const CHANNELS = types.length;
const shares = [1/2, 1/2];
const osc = [];
const gain = [];

function createAudioCtx() {
	audioCtx = new AudioContext();
	for (let i = 0; i < CHANNELS; ++i) {
		osc.push(new OscillatorNode(audioCtx, {
			type: types[i],
			frequency: 440,
		}));
		gain.push(new GainNode(audioCtx, {"gain":0.0001}));
		osc[i].connect(gain[i]).connect(audioCtx.destination);
	}

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
}

function nset(ch, note) {
	volume = Math.max(0, note);
	volume = Math.min(127, note);
	osc[ch].frequency.value = (440 * (2 ** ((note - 69) / 12)));
}

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

let lastX = 0;
let lastY = 0;
function handleMouse(pageX, pageY, type, callback) {
	const element = document.elementFromPoint(pageX, pageY);
	if (element.x == undefined) {
		return;
	}
	const x = element.x;
	const y = element.y;
	if (x == lastX && y == lastY && event.type == 2) {
		return;
	}
	callback(x, y, type);
	lastX = x;
	lastY = y;
}

const importObject = {env: {
	cset,
	bset,
	fset,
	nset,
	vset,
	focus,
}};
WebAssembly.instantiateStreaming(fetch("./compiled.wasm"), importObject).then((obj) => {
	requestAnimationFrame(() => {
		window.onresize();
	});

	obj.instance.exports.oninit();

	document.body.addEventListener("click", () => {
		createAudioCtx();

		inp.addEventListener("input", () => {
			processInput(obj.instance.exports.onchar);
		});

		const mouseCallback = obj.instance.exports.onmouse;
		document.body.addEventListener("mousedown", (event) => {
			handleMouse(event.pageX, event.pageY, 1, mouseCallback);
		});
		document.body.addEventListener("mousemove", (event) => {
			handleMouse(event.pageX, event.pageY, 2, mouseCallback);
		});
		document.body.addEventListener("mouseup", (event) => {
			handleMouse(event.pageX, event.pageY, 3, mouseCallback);
		});
		document.body.addEventListener("touchstart", (event) => {
			event.preventDefault();
			handleMouse(event.changedTouches[0].pageX, event.changedTouches[0].pageY, 2, mouseCallback);
			handleMouse(event.changedTouches[0].pageX, event.changedTouches[0].pageY, 1, mouseCallback);
		}, {passive: false});
		document.body.addEventListener("touchmove", (event) => {
			event.preventDefault();
			handleMouse(event.changedTouches[0].pageX, event.changedTouches[0].pageY, 2, mouseCallback);
		}, {passive: false});
		document.body.addEventListener("touchend", (event) => {
			event.preventDefault();
			handleMouse(event.changedTouches[0].pageX, event.changedTouches[0].pageY, 3, mouseCallback);
		}, {passive: false});
		document.body.addEventListener("touchcancel", (event) => {
			event.preventDefault();
			handleMouse(event.changedTouches[0].pageX, event.changedTouches[0].pageY, 3, mouseCallback);
		}, {passive: false});

		const FPS = 30;
		setInterval(obj.instance.exports.onframe, 1000/FPS);
	}, {once: true});
});
