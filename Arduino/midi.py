import tkinter as tk
import serial
import serial.tools.list_ports as port_list

MIDI_NOTE_OFF = 0x80
MIDI_NOTE_ON  = 0x90


class MIDI(tk.Frame):
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)
        self.baud = 115200
        self.playing = False
        self.timer = None
        self.serial = None
        self.ports = port_list.comports()
        self.portname = "/dev/ttyUSB0"

        self.init_gui()

    def init_gui(self):
        self.pick_btn = tk.Button(self, text="Pick", command=self.pick).grid(row=0, column=0)
        self.damp_btn = tk.Button(self, text="Damp", command=self.damp).grid(row=1, column=0)

        self.pick_freq = tk.Entry(self).grid(row=2, column=0)

        self.start_btn = tk.Button(self, text="Start", command=self.start).grid(row=3, column=0)
        self.stop_btn = tk.Button(self, text="Stop", command=self.stop).grid(row=4, column=0)

        self.ports_box = tk.Listbox(self)
        self.ports_box.grid(row=5, column=0)

        for i, port in enumerate(self.ports):
            self.ports_box.insert(i, port)
            print(port)
        
        self.serial = serial.Serial(self.portname, self.baud)


    def pick(self):
        print("pick")
        self.serial.write(MIDI_NOTE_ON)
        self.serial.write(0)
        self.serial.write(0)


    def damp(self):
        self.serial.write(MIDI_NOTE_OFF)
        self.serial.write(0)
        self.serial.write(0)


    def start(self):
        timing = 1000

        self.timer = self.after(timing, self.start)
        self.pick()


    def stop(self):
        if self.timer is not None:
            self.after_cancel(self.timer)
            self.timer = None
            print("stopped")



if __name__ =="__main__":
    root = tk.Tk()
    root.title("Picker Damper MIDI")
    MIDI(root).grid()
    root.mainloop()