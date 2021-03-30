from tkinter import *
from typing import Tuple, List

import UI_enums

def printResult():
    print("box ticked")

class LeftMenu:
    def __init__(self,root):
        self.root = root
        self.background_color = "white"
        self.foreground_color = "black"
        self.borderwidth = 2
        self.frame = Frame(root)

        self.break_reminder = BooleanVar()
        self.water_reminder = BooleanVar()
        self.walk_reminder = BooleanVar()
        self.weather_station = BooleanVar()

        self.createFrame()
        self.createleftMenuChecklist()
    
    def createFrame(self):
        self.frame.configure(bg=self.background_color,borderwidth=self.borderwidth,relief=SUNKEN)
        self.frame.pack(side=LEFT)

        left_side_label = Label(self.frame, text="Pick active timed functions", bg=self.background_color, fg=self.foreground_color)
        left_side_label.pack()

    def createleftMenuChecklist(self):
        padding_x = 10
        paddiny_y = 10

        break_button = Checkbutton(self.frame, text= "break reminder", variable=self.break_reminder, command=printResult, bg=self.background_color, fg=self.foreground_color )
        break_button.pack(padx = padding_x, pady = paddiny_y)

        water_reminder_button = Checkbutton(self.frame, text= "water reminder", variable=self.water_reminder, command=printResult, bg=self.background_color, fg=self.foreground_color )
        water_reminder_button.pack(padx = padding_x, pady = paddiny_y)

        walk_reminder_button = Checkbutton(self.frame, text= "walk reminder", variable=self.walk_reminder, command=printResult, bg=self.background_color, fg=self.foreground_color )
        walk_reminder_button.pack(padx = padding_x, pady = paddiny_y)

        weather_station = Checkbutton(self.frame, text= "weather station", variable=self.weather_station, command=printResult, bg=self.background_color, fg=self.foreground_color )
        weather_station.pack(padx = padding_x, pady = paddiny_y)

    def getActiveResults(self) -> List[UI_enums.ActivityEnums]:
        return_list = []
        if(self.break_reminder.get()):
            return_list.append(UI_enums.ActivityEnums.BREAK_REMINDER)
        elif(self.water_reminder.get()):
            return_list.append(UI_enums.ActivityEnums.WATER_REMINDER)
        elif(self.walk_reminder.get()):
            return_list.append(UI_enums.ActivityEnums.WALK_REMINDER)
        elif(self.weather_station.get()):
            return_list.append(UI_enums.ActivityEnums.WEATHER_STATION)
        return return_list    


class MiddleMenu:
    def __init__(self, root):
        self.root = root
        self.background_color = "pink"
        self.foreground_color = "black"
        self.borderwidth = 2
        self.frame = Frame(root)

        self.createFrame()

    def createFrame(self):
        self.frame.configure(bg=self.background_color,borderwidth=self.borderwidth,relief=SUNKEN)
        self.frame.pack(side=TOP)

        left_side_label = Label(self.frame, text="Pick active timed functions", bg=self.background_color, fg=self.foreground_color)
        left_side_label.pack()

class RobotUI:
    def __init__(self):
        self.root = Tk()
        self.title = "Robot UI"
        self.left_menu = LeftMenu(self.root)
        self.middle_menu = MiddleMenu(self.root)
        self.background_color = "gray69"

        self.width = 1500
        self.height = 1000
        self.root.title("Robot UI")
        self.root.configure(bg=self.background_color)
        self.root.geometry(str(self.width)+"x"+ str(self.height))

    def start(self):
        self.root.mainloop()

def main():
    ui = RobotUI()
    ui.start()


main()
