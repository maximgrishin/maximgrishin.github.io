import asyncio
import select
import sys
import termios
import tty

PICO8_COLORS = (
		(  0,  0,  0),  # black
		( 29, 43, 83),  # dark_blue
		(126, 37, 83),  # dark_purple
		(  0,135, 81),  # dark_green
		(171, 82, 54),  # brown
		( 95, 87, 79),  # dark_gray
		(194,195,199),  # light_gray
		(255,241,232),  # white
		(255,  0, 77),  # red
		(255,163,  0),  # orange
		(255,236, 39),  # yellow
		(  0,228, 54),  # green
		( 41,173,255),  # blue
		(131,118,156),  # indigo
		(255,119,168),  # pink
		(255,204,170),  # peach
		)
SET_FG_COLOR = tuple("\x1b[38;2;" + ";".join(map(str,color)) + "m" for color in PICO8_COLORS)
SET_BG_COLOR = tuple(color[:2] + "4" + color[3:] for color in SET_FG_COLOR)
RESET_COLOR = "\x1b[0m"
ALT_SCREEN = "\x1b[?1049h"
MAIN_SCREEN = "\x1b[?1049l"
SHOW_CURSOR = "\x1b[?25h"
HIDE_CURSOR = "\x1b[?25l"
def GO_TO(y, x):
	return f"\x1b[{y+1};{x+1}H"

def cdraw(x, y, c, bg, fg):
	print(GO_TO(y, x) + SET_BG_COLOR[bg] + SET_FG_COLOR[fg] + c, end='', flush=True)

def run(update, main):
	attributes = tty.setcbreak(0)
	global _step
	_step = update
	try:
		print(HIDE_CURSOR+ALT_SCREEN, end='', flush=True)
		print(GO_TO(16, 0) + 'controls: WASD, O, P')
		asyncio.run(main())
	except KeyboardInterrupt:
		pass
	except:
		raise
	finally:
		print(SHOW_CURSOR+MAIN_SCREEN, end='', flush=True)
		termios.tcsetattr(0, termios.TCSAFLUSH, attributes)

spoll = select.poll()
spoll.register(sys.stdin, select.POLLIN)

def read_entered():
	entered = ""
	while spoll.poll(0):
		entered += sys.stdin.read(1)
	return entered
