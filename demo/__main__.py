import pui

i = 0
col = 0
m = 1
xs = [1,1,1,2,3,3,3,2]
ys = [1,2,3,3,3,2,1,1]

x = 0
y = 0

def update():
	pui.cls()
	global i, m, col
	i += 1
	i %= 8
	if i == 0:
		col += 1
		col %= 16
	if pui.btnp(4):
		m = 1
	if pui.btnp(5):
		m = -1
	pui.cset(xs[m*i], ys[m*i], "d", col, 15-col)
	global x, y
	if pui.btnp(0):
		x -= 1
	if pui.btnp(1):
		x += 1
	if pui.btnp(2):
		y -= 1
	if pui.btnp(3):
		y += 1
	pui.cset(x, y, 'w', 3, 15)

pui.run(update)
