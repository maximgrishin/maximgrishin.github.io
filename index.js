const PICO8_COLORS = [
	'0 0 0',       // black
	'29 43 83',    // dark_blue
	'126 37 83',   // dark_purple
	'0 135 81',    // dark_green
	'171 82 54',   // brown
	'95 87 79',    // dark_gray
	'194 195 199', // light_gray
	'255 241 232', // white
	'255 0 77',    // red
	'255 163 0',   // orange
	'255 236 39',  // yellow
	'0 228 54',    // green
	'41 173 255',  // blue
	'131 118 156', // indigo
	'255 119 168', // pink
	'255 204 170', // peach
];

const canvas = document.createElement("canvas");
const context = canvas.getContext("2d");
context.font = "100px monospace";
const metrics = context.measureText("1".repeat(33));
let font_height = 100.*window.screen.width/metrics.width;
if (window.screen.width > window.screen.height) {
	font_height *= 0.75*window.screen.height/window.screen.width;
}
const font_width = (font_height/100)*metrics.width/33;

document.body.style = "border:0;margin:0;font-size:"+font_height+"px;font-family:monospace;white-space:nowrap;";
document.body.style.background ="rgb("+PICO8_COLORS[0]+")"
document.body.style.color = "rgb("+PICO8_COLORS[6]+")"

let box = document.createElement("div");
let cells = Array();
for (let i = 0; i < 16; ++i) {
	let row = Array();
	for (let j = 0; j < 32; ++j) {
		const cell = document.createElement("span");
		row.push(cell);
		box.appendChild(cell);
	}
	cells.push(row);
	box.appendChild(document.createElement("br"));
}

const left_offset = (window.screen.width-font_width*33)/2;
document.body.appendChild(box);
box.style="border:solid "+font_width/2+"px;border-color:rgb("+PICO8_COLORS[5]+");width:fit-content;white-space:nowrap;position:absolute;left:"+left_offset+"px;top:0;";
const help = document.createElement("p");
help.style = "position:absolute;top:"+font_height*20+"px;left:"+left_offset+"px;";
document.body.appendChild(help);
const inp = document.createElement("input");
inp.style = "position:absolute;top:-1000px;left:"+left_offset+"px";
document.body.appendChild(inp);

function cset(y, x, c) {
	if (y < 0 || 16 <= y || x < 0 || 32 <= x) {
		return;
	}
	if (typeof(c) == typeof(0)) {
		c = String.fromCharCode(c);
	}
	if (c == ' ') {
		c = '\u00A0';
	}
	let bg = 0;
	let fg = 7;
	cells[y][x].textContent = c;
}
window.cset = cset;

function bset(y, x, bg) {
	if (y < 0 || 16 <= y || x < 0 || 32 <= x) {
		return;
	}
	if (0 <= bg && bg < 16) {
		cells[y][x].style.background = 'rgb(' + PICO8_COLORS[bg] + ')'
	}
}
window.bset = bset;
function fset(y, x, fg) {
	if (y < 0 || 16 <= y || x < 0 || 32 <= x) {
		return;
	}
	if (0 <= fg && fg < 16) {
		cells[y][x].style.color = 'rgb(' + PICO8_COLORS[fg] + ')'
	}
}
window.fset = fset;

function cls() {
	for (let i = 0; i < 16; ++i) {
		for (let j = 0; j < 32; ++j) {
			cset(i, j, ' ');
			fset(i, j, 7);
			bset(i, j, 0);
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
		++i;
	}
}

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

let runtime_initialized = false;
window.started = false;

document.addEventListener("click", () => {
	if (!runtime_initialized) {
		print(13,2,"failed to load C++ module :(");
		print(14,2,"try refreshing the page");
	}
	if (window.started == false) {
		cls();
		frameCallback();
		help.textContent += 'controls: WASD, O, P';
		if (audioCtx.state === "suspended") {
			audioCtx.resume();
		}
		for (let i = 0; i < 4; ++i) {
			osc[i].start(audioCtx.currentTime);
		}
		window.started = true;
	}
	inp.focus();
});

function frameCallback() {
	window.update();
	window.flip();
	requestAnimationFrame(frameCallback);
}

function log(m) {
	help.textContent += m;
}

const audioCtx = new AudioContext();
const types = ["sine", "square", "triangle", "sawtooth"];
const shares = [3/8, 1/8, 3/8, 1/8];
let osc = [];
let gain = [];
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
	runtime_initialized = true;
});
