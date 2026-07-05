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

function bset(y, x, bg) {
	if (y < 0 || HEIGHT <= y || x < 0 || WIDTH <= x) {
		return;
	}
	if (0 <= bg && bg < PICO8_COLORS.length) {
		cells[y][x].style.background = PICO8_COLORS[bg]
	}
}

function fset(y, x, fg) {
	if (y < 0 || HEIGHT <= y || x < 0 || WIDTH <= x) {
		return;
	}
	if (0 <= fg && fg < PICO8_COLORS.length) {
		cells[y][x].style.color = PICO8_COLORS[fg]
	}
}

function processInput(callback) {
	while (inp.value != '') {
		const head = inp.value[0];
		inp.value = inp.value.slice(1);
		if (head == 'w' || head == 'W') {
			callback(1);
		}
		else if (head == 's' || head == 'S') {
			callback(2);
		}
		else if (head == 'a' || head == 'A') {
			callback(3);
		}
		else if (head == 'd' || head == 'D') {
			callback(4);
		}
		else if (head == 'p' || head == 'P') {
			callback(5);
		}
		else if (head == 'o' || head == 'O') {
			callback(6);
		}
	}
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

document.body.addEventListener("click", () => {
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

const importObject = {env:{cset,bset,fset,nset,vset}};
WebAssembly.instantiateStreaming(fetch("./compiled.wasm"), importObject).then((obj) => {
	obj.instance.exports.call_oninit();

	inp.addEventListener("input", () => {
		processInput(obj.instance.exports.call_onbutton);
	});

	document.body.addEventListener("click", () => {
		obj.instance.exports.call_onclick(1,2);
	});

	const FPS = 30;
	setInterval(obj.instance.exports.call_onframe, 1000/FPS);

	requestAnimationFrame(()=>{
		window.onresize();
	});
});
