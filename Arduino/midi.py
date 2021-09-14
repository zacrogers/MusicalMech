from ctypes import alignment
import tkinter as tk
import serial
import serial.tools.list_ports as port_list
import time
import struct
from enum import Enum

MIDI_NOTE_OFF = 0x80
MIDI_NOTE_ON  = 0x90
MIDI_CC       = 0x0B

class Material(Enum):
    FOAM = 0
    SILICONE = 1


class MIDI(tk.Frame):
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)
        self.baud = 115200
        self.playing = False
        self.timer = None
        self.serial = None
        self.ports = port_list.comports()
        # self.portname = "/dev/ttyUSB0"
        self.portname = "COM4"

        self.init_gui()
        
        self.serial = serial.Serial(self.portname, self.baud, timeout=1)

    def init_gui(self):
        # Column 0 : Manual Control
        self.col0_label = tk.Label(self, text="Manual Control").grid(row=0, column=0)

        self.pick_btn = tk.Button(self, text="Pick", command=self.pick)
        self.pick_btn.grid(row=1, column=0, sticky=tk.NSEW, padx=10)

        self.damp_btn = tk.Button(self, text="Damp", command=self.damp)
        self.damp_btn.grid(row=2, column=0, sticky=tk.NSEW, padx=10)

        self.ampl_label = tk.Label(self, text="Amplitude (0-127)").grid(row=3, column=0)

        self.ampl_val_var = tk.IntVar(self, value=50)
        self.ampl_val_entry = tk.Entry(self, textvariable=self.ampl_val_var)
        self.ampl_val_entry.grid(row=4, column=0, padx=10)

        # Column 1
        self.col1_label = tk.Label(self, text="Auto Control").grid(row=0, column=1)

        self.start_btn = tk.Button(self, text="Start", command=self.start)
        self.start_btn.grid(row=1, column=1, sticky=tk.NSEW, padx=10)

        self.stop_btn = tk.Button(self, text="Stop", command=self.stop)
        self.stop_btn.grid(row=2, column=1, sticky=tk.NSEW, padx=10)

        self.freq_label = tk.Label(self, text="Picking Frequency").grid(row=3, column=1)

        self.pick_freq_var = tk.IntVar(self, value=50)
        self.pick_freq_entry = tk.Entry(self, textvariable=self.pick_freq_var).grid(row=4, column=1)

        # Column 2
        self.material_label = tk.Label(self, text="Damping Material")
        self.material_label.grid(row=0, column=2, sticky=tk.NSEW)

        self.silicone_btn = tk.Button(self, text="Silicone", command=lambda:self.set_damping(Material.SILICONE))
        self.silicone_btn.grid(row=1, column=2, sticky=tk.NSEW, padx=10)

        self.foam_btn = tk.Button(self, text="Foam", command=lambda:self.set_damping(Material.FOAM))
        self.foam_btn.grid(row=2, column=2, sticky=tk.NSEW, padx=10) 


    def pick(self):
        amplitude = self.ampl_val_var.get()
        msg = bytearray([MIDI_NOTE_ON, 0, amplitude])
        
        self.serial.write(msg)
        self.serial.flush()


    def damp(self):
        amplitude = self.ampl_val_var.get()
        msg = bytearray([MIDI_NOTE_OFF, 0, amplitude])

        self.serial.write(msg)
        self.serial.flush()


    def start(self):
        timing = self.pick_freq_var.get()
        
        if not timing:
            timing = 1000
        
        self.timer = self.after(timing, self.start)
        self.pick()


    def set_damping(self, material:Material):
        msg = bytearray([MIDI_CC, material.value, 0])
        self.serial.write(msg)


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
