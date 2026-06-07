import js

def run(step, main):
	global _step
	_step = step

def cdraw(x,y,c,bg,fg):
	if c == ' ':
		c = '\u00A0'
	js.cdraw(x,y,c,bg,fg)

def read_entered():
	return js.read_entered()
