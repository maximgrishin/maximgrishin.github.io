document.body.style = 'font-family: monospace';

let cells = Array()
for (let i = 0; i < 16; ++i) {
	let row = Array();
	for (let j = 0; j < 32; ++j) {
		cell = document.createElement("span");
		cell.innerHTML = '.';
		row.push(cell);
		document.body.appendChild(cell);
	}
	cells.push(row);
	document.body.appendChild(document.createElement("br"));
}
help = document.createElement("p");
help.textContent = 'controls: WASD, O, P';
document.body.appendChild(help);
const inp = document.createElement("input");
inp.style = "position:absolute;top:-1000px;";
document.body.appendChild(inp);
cells[7][12].textContent = 'loading';
for (let i = 13; i < 19; ++i) {
	cells[7][i].textContent = '';
}

PICO8_COLORS = [
	'0 0 0',       // black
	'29 43 83',    // dark_blue
	'126 37 83',   // dark_purple
	'0 135 81',    // dark_green
	'171 82 54',   // brown
	'95 87 79',    // dark_gray
	'194 195 199', // light_gray
	'255 241 232', // white
	'255 0 77',    // red
	'255 163 0',  	// orange
	'255 236 39',  // yellow
	'0 228 54',    // green
	'41 173 255',  // blue
	'131 118 156', // indigo
	'255 119 168', // pink
	'255 204 170', // peach
];

function cdraw(x,y,c,bg,fg) {
	cells[y][x].textContent = c;
	cells[y][x].style = 'background-color:rgb(' + PICO8_COLORS[bg] + ');color:rgb(' + PICO8_COLORS[fg] + ')'
}

function read_entered() {
	entered = inp.value;
	inp.value = '';
	return entered;
}

document.addEventListener("click", () => {
	inp.focus();
});
