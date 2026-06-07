import asyncio
import pui.pui_io as pui_io

WIDTH = 32
HEIGHT = 16

btns = [False for _ in range(6)]

def btnp(b):
	return btns[b]

screen = [[[' ', 0, 7] for x in range(WIDTH)] for y in range(HEIGHT)]

def cset(x, y, c=None, bg=None, fg=None):
	cell = screen[y][x]
	if c is not None:
		cell[0] = c
	if bg is not None:
		cell[1] = bg
	if fg is not None:
		cell[2] = fg

def run(step):
	pui_io.run(step, _main)

def repaint():
	for y, row in enumerate(screen):
		for x, cell in enumerate(row):
			c, bg, fg = cell
			pui_io.cdraw(x, y, c, bg, fg)

def update_btns(entered):
	for b in range(len(btns)):
		btns[b] = False
	if 'a' in entered or 'A' in entered:
		btns[0] = True
	if 'd' in entered or 'D' in entered:
		btns[1] = True
	if 'w' in entered or 'W' in entered:
		btns[2] = True
	if 's' in entered or 'S' in entered:
		btns[3] = True
	if 'c' in entered or 'C' in entered:
		btns[4] = True
	if 'x' in entered or 'X' in entered:
		btns[5] = True

async def _main():
	while True:
		update_btns(pui_io.read_entered())
		pui_io._step()
		repaint()
		await asyncio.sleep(0.033)

def rectfill(x1, y1, x2, y2, c=None, fg=None, bg=None):
	for y in range(y1, y2):
		for x in range(x1, x2):
			cset(x, y, c, bg, fg)

def cls():
	rectfill(0, 0, 32, 16, ' ', 7, 0)

def sprint(x, y, string='', fg=None, bg=None):
	for i, c in enumerate(string):
		cset(x+i, y, c)
