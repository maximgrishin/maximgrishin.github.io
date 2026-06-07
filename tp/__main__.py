import random
import pui

anim = 0
canim = 0
qb = 0


def braille(i, j):
	return [[' ','.','.','|','|'],
									['.','.','.','|','|'],
									['.','.','.','|','|'],
									['|','|','|','|','|'],
									['|','|','|','|','|'],
									][i][j]

def graph(x1,y1,x2,y2,g,fg=None,bg=None):
	pui.rectfill(x1,y1,x2,y2,' ',bg,fg)
	#if len(g) % 2 == 1:
	# g = [0] + g
	#for a, b in zip(g[::2], g[1::2]):
	for i in range(y1, y2):
		for j in range(x1, x2):
			x = j - x1
			a = g[2*x]
			b = g[2*x+1]
			height = (y2-i-1)*4
			a = max(0, a-height)
			b = max(0, b-height)
			a, b = min(4,a), min(4,b)
			pui.cset(j, i, braille(a, b))

rps = [[random.randint(0,8) for _ in range(18)] for o in range(3)]
threads = list(random.randint(-5,6) for _ in range(4*8))

def update():
	pui.cls()
	pui.rectfill(0,10,32,16," ",6,5)
	pit.update()
	pit.draw()
	pui.sprint(6,10,"LOAD")
	pui.sprint(17,10,"503")
	pui.sprint(23,10,"LATENCY")
	pui.rectfill(1,11,10,13," ",12,0)
	pui.rectfill(11,11,20,13," ",8,0)
	pui.rectfill(21,11,30,13," ",11,0)
	global anim, canim
	anim %= 60
	if anim == 0 and pit.value("time"):
		for o in range(3):
			rps[o] = rps[o][1:]+[random.randint(0,8)]
	anim += 1
	graph(1,11,10,13,rps[0])
	graph(11,11,20,13,rps[1])
	graph(21,11,30,13,rps[2])
	for i in range(pit.value("cores")):
		pui.sprint(0+i*8,0,"  [  ]")
		#pui.sprint(3+i*8,0,("--","-\\","-|","-/","--","\\-","|-","/-")[int(canim/8)%8])
		pui.sprint(3+i*8,0,("--","\\\\","||","//",)[int(canim/16)%4])
		if pit.value("time"):
			canim += 1
		pui.cset(3+i*8,0,fg=9)
		pui.cset(4+i*8,0,fg=9)
	for y in (2,5,10,13,18,21,26,29):
		pui.cset(y,0,fg=3) #3->11

	for t in range(8):
		for cpu in range(4):
			if random.randint(1,10) > 9 and pit.value("cores") and pit.value("time"):
				threads[t*4+cpu] += 1
			if threads[t*4+cpu] >= 6:
				threads[t*4+cpu] = -5
			if threads[t*4+cpu] == 0 and not pit.value("rps"):
				threads[t*4+cpu] = -1

			pui.sprint(1+cpu*8,t+1,"------")
			if t*4+cpu >= pit.value("threads"):
				continue
			if threads[t*4+cpu] >=0:
				pui.sprint(1+cpu*8+threads[t*4+cpu],t+1,'+')
				for j in range(6):
					pui.cset(1+j+cpu*8, t+1, fg=9 if pit.code[j] == 1 else 1)
			else:
				pui.cset(1+cpu*8,t+1,fg=5)
				pui.cset(2+cpu*8,t+1,fg=5)
				pui.cset(3+cpu*8,t+1,fg=5)
				pui.cset(4+cpu*8,t+1,fg=5)
				pui.cset(5+cpu*8,t+1,fg=5)
				pui.cset(6+cpu*8,t+1,fg=5)

	pui.rectfill(0,9,32,10," ",5,0)
	pui.sprint(0,9,"= "*pit.value("queue"))
	global qb
	if canim % 60 == 0:
		qb = random.randint(0,5)
	for i in range(qb):
		pui.cset(i*2,9,fg=1)

def print_num(x,y,num):
	if num < 10:
		pui.sprint(x,y,'0'+str(num))
	else:
		pui.sprint(x,y,str(num))

class Pit:
	RANGES = {
			"rps": (0,1,2,3,5,6,10,15,30),
			"time": (0,1,2,3,5,6,10,15,30),
			"queue": (0,1,2,3,4,6,8,12,16),
			"threads": (0,1,2,3,4,6,8,12,16,24,32),
			"cores": (0,1,2,4),
			}
	GRID = (("rps","time"),("queue","cores"),("threads","code"))

	def __init__(self):
		self.blink = 0
		#Ctl.make_num(2,3,"cores")
		self.x = 0
		self.y = 0
		self.code_x = 0
		self.values = {
				"rps": 6,
				"queue": 8,
				"threads": 10,
				"cores": 3,
				"time": 4,
				}
		self.code = [1,1,0,0,1,1]
		self.rps_fuse = 0
	def draw(self):
		for i in range(len(Pit.GRID)):
			for j in range(len(Pit.GRID[i])):
				self.draw_cell(i, j)
		self.draw_cursor()
	def value(self, name):
		return Pit.RANGES[name][self.values[name]]
	def name(self):
		return Pit.GRID[self.y][self.x]
	def draw_cursor(self):
		cols = (0,4,8,8,8,4)
		self.blink += 1/4
		self.blink %= len(cols)
		col = cols[int(self.blink)]
		if self.name() == "code":
			pui.cset(17+self.code_x,15,bg=col)
		else:
			pui.cset(1+16*self.x, 13+self.y, bg=col)
			pui.cset(1+16*self.x+1,13+ self.y, bg=col)
	def draw_cell(self, ii, jj):
		x = 1 + jj*16
		y = 13 + ii
		name = Pit.GRID[ii][jj]
		if name == "code":
			for i in range(len(self.code)):
				if self.code[i] == 0:
					col = 1
				else:
					col = 9
				pui.cset(x+i,y,"W",0,col)
			pui.sprint(x+len(self.code)+1,y,"code".upper())
		else:
			pui.rectfill(x,y,x+2,y+1," ",7,0)
			num = self.value(name)
			print_num(x,y,num)
			pui.sprint(x+3,y,name.upper())
	def update(self):
		if pui.btnp(0):
			if self.name() == "code" and self.code_x > 0:
				self.code_x -= 1
			elif self.x > 0:
				self.x -= 1
		if pui.btnp(1):
			if self.name() == "code" and self.code_x + 1 < len(self.code):
				self.code_x += 1
			elif self.x + 1 < len(Pit.GRID[self.x]):
				self.x += 1
		if pui.btnp(2):
			if self.y > 0:
				self.y -= 1
		if pui.btnp(3):
			if self.y + 1 < len(Pit.GRID):
				self.y += 1
		if pui.btnp(4):
			name = self.name()
			if name == "code":
				self.code[self.code_x] = 1
			elif self.values[name] + 1 < len(Pit.RANGES[name]):
				self.values[name] += 1
		if pui.btnp(5):
			name = self.name()
			if name == "code":
				self.code[self.code_x] = 0
			elif self.values[name] > 0:
				self.values[name] -= 1

pit = Pit()

pui.run(update)
