import os, subprocess
import pygame
from pygame.locals import *
import Tkinter
import tkFileDialog
rpt = Tkinter.Tk()
rpt.withdraw()


PATH = ""

class Screen(object):
	"""Base type of a type of screen (menu/tutorial etc)"""
	def __init__(self, surface, rects, text, objects):
		self.surface = surface
		self.rects = rects
		self.text = text
		self.objects = objects
		self.vals = ["Press Browse... to choose file" for i in range(len(objects))]
		self.background =     [(200, 0, 5, 40),	# Positions of football pitch lines
				      (300, 0, 5, 40),
			              (200, 40, 105, 5)]

	def draw(self):
		for line in self.background:					#Begin drawing football self.background
			pygame.draw.rect(screen, WHITE, Rect(line))
		for line in self.background:
			pygame.draw.rect(screen, WHITE, Rect(line[0], 600-line[1], line[2], -line[3]))
		pygame.draw.rect(screen, WHITE, Rect(0, 300, 600, 5))
		pygame.draw.circle(screen, WHITE, (500/2, 600/2), 50, 5) #End drawing background
		for obj in self.rects:					 #Begin drawing screen specific objects
			pygame.draw.rect(self.surface, obj[0], obj[1], obj[2])
		for line in self.text:
			self.surface.blit(line[0], line[1])
		for obj in self.objects:
			obj.draw()
	
	def handle(self, mouse_pos):
		i = 0
		for obj in self.objects:
			self.vals[i] = obj.handle(mouse_pos, self.vals)
			i += 1
			
class Displaybox(object):
	"""Box on screen that has variable text"""
	def __init__(self, surface, colour, dimensions, text, font, behbox):
		self.surface = surface
		self.colour = colour
		self.dimensions = dimensions
		self.text = text
		self.font = font
		self.display = self.font.render(self.text, 1, self.colour)
		self.behboxpair = behbox
	
	def __update_disp(self):
		if (self.behboxpair.get_val() != ""):
			self.text = self.behboxpair.get_val()
		if (len(self.text) > 40):
			self.text = self.text[:37] + "..."
		self.display = self.font.render(self.text, 1, (0,0,0))

	def draw(self):
		self.__update_disp()
		pygame.draw.rect(self.surface, self.colour, Rect(self.dimensions))
		pygame.draw.rect(self.surface, (0,0,0), Rect(self.dimensions),1)
		self.surface.blit(self.display, (self.dimensions[0]+5, self.dimensions[1]+5))

	def get_text(self):
		return self.text

	def set_text(self, string):
		self.text = string

	def handle(self, mouse_pos, params):
		return False
		
		
		

class Behaviorbox(object):
		"""Box on screen that performs an action once clicked"""
		def __init__(self, surface, colour, dimensions, text, function):
			self.surface = surface
			self.colour = colour
			self.dimensions = dimensions
			self.function = function
			self.text = text
			self.val = ""
		
		def draw(self):
			pygame.draw.rect(self.surface, self.colour, Rect(self.dimensions))
			pygame.draw.rect(self.surface, (0,0,0), Rect(self.dimensions),1)
			self.surface.blit(self.text, (self.dimensions[0]+5, self.dimensions[1]+5))
		
		def handle(self, mouse_pos, params):
			if Rect(self.dimensions).collidepoint(mouse_pos[0], mouse_pos[1]):
				self.val = self.function(params)
				return self.val
			else:
				return self.val

		def get_val(self):
			return self.val

		def set_text(self, textobj):
			self.text = textobj
			
			

class FileViewer(object):
	"""Widget to display contents of a (log) file"""
	def __init__(self, surface, dimensions, file_path):
		self.dimensions = dimensions
		self.file_path = file_path
		self.lines = []
		self.surface = surface

	def draw(self):
		self.update()
		pygame.draw.rect(self.surface, WHITE, Rect(self.dimensions))
		pygame.draw.rect(self.surface, BLACK, Rect(self.dimensions), 1)
		if len(self.lines) == 0:
			self.lines.append("No errors reported\n")
		for i in range(0, len(self.lines)):
			img = small_default.render(self.lines[i][:len(self.lines[i])-1], 1, BLACK)
			self.surface.blit(img, ((self.dimensions[0]+2, self.dimensions[1]+15*i+2)))

	def update(self):
		try:
			ifile = open(self.file_path)
		except(IOError):
			return -1
		self.lines = ifile.readlines()
		ifile.close()
	
	def check_error(self):
		self.update()
		if len(self.lines == 0):
			return 0
		else:
			return -1

	def handle(self, mouse_pos, params):
		return False



def browse_file(params):
	return tkFileDialog.askopenfilename()

def open_file(params):
	return tkFileDialog.asksaveasfilename(defaultextension='.py')

def compile(params):
	inpfile = params[5]
	outfile = params[6]
	return os.system(PATH+"Referee.exe " + inpfile + " " + outfile + " > " +PATH+"log.txt") #Run compiler, pipe output to log file

def compile_run(params):
	inpfile = params[5]
	outfile = params[6]
	if compile(params) == 0:
		subprocess.Popen(["idle", outfile]).pid
	
def change_screen(params):	 # params:  [screen_count, screens, nx_box, active_screen, next_screen]
	params[0][0] = (params[0][0] + 1) % len(params[1]) # Add 1 to active screen count
	params[3][0] = params[1][params[0][0]] #update the active screen
	params[4].set_text(params[2][params[0][0]]) #Update text box
	return params[1][params[0][0]]	#just gimme pointers and easy passing by reference for Guido's sake

try:	
	os.remove(PATH+"log.txt")
except(OSError):
	None	#File already does not exist

pygame.init()
screen = pygame.display.set_mode((500,600))
pygame.display.set_caption("Referee")


#Colours
LIGHT_GREEN = (44,196,5)
DARK_GREEN = (21,107,0)
WHITE = (255,255,255)
BLACK = (0,0,0)
LIGHT_GREY = (180,180,180)
RED = (180,0,0)
ORANGE = (225,45,0)

#Fonts & text
title_font = pygame.font.SysFont("Comic Sans MS", 25)
default_font = pygame.font.SysFont("Arial", 20)
small_default = pygame.font.SysFont("Arial", 15)
info_text = pygame.font.SysFont("Arial", 13)

title = title_font.render("Referee: Compiler to Python", 1, BLACK)
Iselect =  default_font.render("Input File", 1, BLACK)
Oselect = default_font.render("Output File", 1, BLACK)
browse = small_default.render("Browse...", 1, BLACK)
comp = default_font.render("Compile", 1, BLACK)
comp_run = default_font.render("Compile & Run", 1, BLACK )
tut1 = default_font.render("Tutorial 1", 1, BLACK)
tut2 = default_font.render("Tutorial 2", 1, BLACK)
compiler = default_font.render("Compiler", 1, BLACK)


nx_box = (tut1, tut2, compiler)

#Create main screen

#Main screen widgets
compbox = Behaviorbox(screen, RED, (50, 220, 140, 40), comp_run, compile_run)
compbox2 = Behaviorbox(screen, ORANGE, (300, 220, 100, 40), comp, compile)
inp_browse = Behaviorbox(screen, LIGHT_GREY, (420, 80, 70, 40), browse, browse_file)
out_browse = Behaviorbox(screen, LIGHT_GREY, (420, 130, 70, 40), browse, open_file)
inp_box = Displaybox(screen, LIGHT_GREY, (105,80,300,40), "Press Browse... to select file", small_default, inp_browse)
out_box = Displaybox(screen, LIGHT_GREY, (105,130,300,40), "Press Browse... to select file", small_default, out_browse)
file_view = FileViewer(screen, (10, 350, 480, 220), PATH+"log.txt")


main_screen = Screen(screen,  (),	#Static Shapes
			((title_font.render("Referee: Compiler to Python", 1, BLACK), (10,10)),	#Static text
			(default_font.render("Input File", 1, BLACK), (3,85)),
			(default_font.render("Output File", 1, BLACK), (3,135))),
			(compbox, compbox2, inp_box, out_box, file_view, inp_browse, out_browse)) #Objects

#Create Tut1 screen
chant_img = pygame.image.load(PATH+"Assets/Images/chant.png")
compile_img = pygame.image.load(PATH+"Assets/Images/compile_screen.png")
chant_comp_img = pygame.image.load(PATH+"Assets/Images/chant_compiled.png")

tutorial1 = Screen(screen, (),	#Static shapes
			((title_font.render("Tutorial 1: Hello World", 1, BLACK), (10,10)),	#Static text and images
			(info_text.render("In this tutorial you will be shown how to make a simple program. Refer to the lesson", 1, BLACK), (5,65)),
			(info_text.render("handout for more a more detailed guide. The most basic program simply prints a value", 1, BLACK), (5,80)),
			(info_text.render("back to the screen. This is done using the CHANT command.", 1, BLACK), (5,95)),
			(chant_img, (40, 118)),
			(info_text.render("Type the above line of code into a text editor (Create a blank file and add it).", 1, BLACK), (5,165)),
			(info_text.render("Save the file as 'helloworld.txt' and go to the main screen of Referee.", 1, BLACK), (5,180)),
			(info_text.render("You should see a screen like the one pictured below:", 1, BLACK), (5,195)),
			(compile_img, (40, 240)),
			(info_text.render("Press the Browse button next to the Input", 1, BLACK), (255,245)),
			(info_text.render("box and navigate to where you saved",1, BLACK), (255,260)),
			(info_text.render("helloworld.txt. Select the file, then do", 1, BLACK), (255,275)),
			(info_text.render("the same with the Output box, choosing", 1, BLACK), (255,290)),
			(info_text.render("the name you want to save the generated", 1, BLACK), (255,305)),
			(info_text.render("code as.", 1, BLACK), (255,320)),
			(info_text.render("Next, compile your program! Click", 1, BLACK), (255,345)),
			(info_text.render("Compile & Run to automatically open", 1, BLACK), (255,360)),
			(info_text.render("your program in IDLE, the python IDE.", 1, BLACK), (255,375)),
			(info_text.render("Run your program in IDLE to execute your", 1, BLACK), (255,390)),
			(info_text.render("program!", 1, BLACK), (255,405)),
			(chant_comp_img, (100,530))),
			()) #Objects

#Create Tut2 screen
if_setup_img =  pygame.image.load(PATH+"Assets/Images/if_setup.png")
if_full_img =  pygame.image.load(PATH+"Assets/Images/if_full.png")
if_compiled =  pygame.image.load(PATH+"Assets/Images/if_compiled.png")

tutorial2 = Screen(screen, (),
			((title_font.render("Tutorial 2: IF statement", 1, BLACK), (10,10)),	#Static text and images
			(info_text.render("The IF statement allows execution of code based on a condition. This means that", 1, BLACK), (5,65)),
			(info_text.render("we can control what happens in our programs on the fly. Start by writing the", 1, BLACK), (5,80)),
			(info_text.render("following program to a blank file:", 1, BLACK), (5,95)),
			(if_setup_img, (40, 118)),
			(info_text.render("This assigns the variable count to 0, allowing us to store values while the", 1, BLACK), (5,140)),
			(info_text.render("program is running. Then we can write the if statement. The full program is below:", 1, BLACK), (5,155)),
			(if_full_img, (20,180)), 
			(info_text.render("The general syntax of an if", 1, BLACK), (330,185)),
			(info_text.render("statements is:", 1, BLACK), (330,200)),
			(info_text.render("IF condition is true THEN:", 1, BLACK), (330,215)),
			(info_text.render("do something,", 1, BLACK), (330,230)),
			(info_text.render("ELSE:", 1, BLACK), (330,245)),
			(info_text.render("do something different,", 1, BLACK), (330,260)),
			(info_text.render("ENDIF.", 1, BLACK), (330,275)),
			(info_text.render("In KICKOFF, IF=IF, PASS=THEN, TRICK=ELSE and SHOOT!=ENDIF. You should familiarise", 1, BLACK), (5,310)),
			(info_text.render("yourself with these commands in the documentation handout if you want to know more.", 1, BLACK), (5,325)),
			(info_text.render("Working through this program, count will be initialised and set to 0. After this", 1, BLACK), (5,340)),
			(info_text.render("the program checks the value of count, comparing its value (0) to 1.", 1, BLACK), (5,355)),
			(info_text.render("Since 0 does not equal 1, the condition is false, so everything before the TRICK", 1, BLACK), (5,370)),
			(info_text.render("command does not execute. Instead the program will execute the part between the", 1, BLACK), (5,385)),
			(info_text.render("TRICK and the SHOOT! statements. To run your program save it as if_statement.txt", 1, BLACK), (5,400)),
			(info_text.render("and compile it as shown in tutorial 1. Run your program for the following result:", 1, BLACK), (5,415)),
			(if_compiled, (10,440))),
			())
			
screens = [main_screen, tutorial1, tutorial2]
screen_count = [0]
active_screen = [screens[screen_count[0]]]
next_screen = Behaviorbox(screen, DARK_GREEN, (390, 20, 100, 40), nx_box[screen_count[0]], change_screen)
running = True

while running:
	mouse_state = pygame.event.poll()
	if mouse_state.type == pygame.QUIT:
		running = False
	elif mouse_state.type == pygame.MOUSEBUTTONDOWN:
		active_screen[0].handle(mouse_state.pos)
		next_screen.handle(mouse_state.pos, [screen_count, screens, nx_box, active_screen, next_screen])
	# Draw background image
	screen.fill(LIGHT_GREEN)
	active_screen[0].draw()
	next_screen.draw()
	#Draw text and title
	pygame.display.update()

pygame.quit()
