import customtkinter as ctk
import serial
import serial.tools.list_ports
import time
import datetime
from datetime import datetime
import struct

eco_set = dict(Between=0, Duration=0, Delay=0)
ctd_set = dict()


ctk.set_appearance_mode("dark")  # Modes: "System" (standard), "Dark", "Light"
ctk.set_default_color_theme("blue")  # Themes: "blue" (standard), "green", "dark-blue"

class CTD_WINDOW(ctk.CTkToplevel):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.geometry("400x300")
        self.title("CTD Sensor")

        self.interval_entry = ctk.CTkEntry(self, placeholder_text="Sampling Interval")
        self.interval_entry.grid(row=0, column=1, padx=10, pady=15)
        self.enter_button = ctk.CTkButton(self, text = "Enter", command=self.enter)
        self.enter_button.grid(row=1, column=1, padx=10, pady=15)

        self.wrong = ctk.CTkLabel(self, text="Enter a value 6-21600", text_color="red")
        
    
    def enter(self):
        entry = self.interval_entry.get()
        try:   
            if int(entry) > 21600 or int(entry)<6:
                self.wrong.grid(row=0, column=1, padx=10, pady=15)
            else:
                ctd_set.append(entry)
                self.wrong.destroy()
                self.destroy()
        except:
            
            self.wrong.grid(row=0, column=1, padx=10, pady=15)

class ECO_WINDOW(ctk.CTkToplevel):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("ECO Sensor")

        self.title1 = ctk.CTkLabel(self, text="Change Settings To:")
        self.title1.grid(row=0, column=1, padx=10, pady=15)
        self.title2 = ctk.CTkLabel(self, text="Current Settings :")
        self.title2.grid(row=0, column=2, padx=10, pady=15)

        self.between_button = ctk.CTkButton(self, text = "Set Time Between Samples", command=self.enter_between)
        self.between_button.grid(row=1, column=0, padx=10, pady=15)
        self.between_entry = ctk.CTkEntry(self, width = 200, placeholder_text="Time Between Samples (m)")
        self.between_entry.grid(row=1, column=1, padx=10, pady=15)
        between_val = eco_set.get("Between")
        self.between_current = ctk.CTkLabel(self, text=f"Time Between Samples: {between_val} m")
        self.between_current.grid(row=1, column=2, padx=10, pady=15)

        self.collection_button = ctk.CTkButton(self, text = "Set Sample Duration", command=self.enter_duration)
        self.collection_button.grid(row=2, column=0, padx=10, pady=15)
        self.collection_entry = ctk.CTkEntry(self, width = 200, placeholder_text="Sample Duration (s)")
        self.collection_entry.grid(row=2, column=1, padx=10, pady=15)
        collection_val = eco_set.get("Duration")
        self.collection_current = ctk.CTkLabel(self, text=f"Sample Duration: {collection_val} s")
        self.collection_current.grid(row=2, column=2, padx=10, pady=15)


        self.delay_button = ctk.CTkButton(self, text = "Set Start Delay", command=self.enter_delay)
        self.delay_button.grid(row=3, column=0, padx=10, pady=15)
        self.delay_entry = ctk.CTkEntry(self, width = 200, placeholder_text="Start Delay (h)")
        self.delay_entry.grid(row=3, column=1, padx=10, pady=15)
        delay_val = eco_set.get("Delay")
        self.delay_current = ctk.CTkLabel(self, text=f"Start Delay: {delay_val} h")
        self.delay_current.grid(row=3, column=2, padx=10, pady=15)

        self.exit_button = ctk.CTkButton(self, text = "Exit", command=self.exit)
        self.exit_button.grid(row=4, column=0, padx=10, pady=15)

        self.calc_button = ctk.CTkButton(self, text = "Calculate Lifetime", command=self.calc)
        self.calc_button.grid(row=4, column=1, padx=10, pady=15)

        self.wrong = ctk.CTkLabel(self, text="Enter a value 1-255", text_color="red")
        self.delay_wrong = ctk.CTkLabel(self, text="Enter a value 0-36", text_color="red")
        self.lifetime = ctk.CTkLabel(self,text=f"Expected Lifetime: NA days")



    def calc(self):
        if(self.collection_entry.get() and self.between_entry.get()):
            numerator = 1800*(int(self.collection_entry.get())+(60*int(self.between_entry.get())))
            denomonator = (60*0.37*int(self.between_entry.get()))+(60*int(self.collection_entry.get()))
            battery = int((numerator/denomonator)/24)
            self.lifetime.grid_remove()
            self.lifetime.configure(self, text=f"Expected Lifetime: {battery} days")
            self.lifetime.grid(row = 4, column = 2, padx=10, pady=15)

    def exit(self):
        self.destroy()

    def enter_between(self):
        try:
            between = int(self.between_entry.get())
            if between>255 or between<1:
                self.wrong.grid(row=1, column=3, padx=10, pady=15)
            else:
                eco_set["Between"] = between
                self.between_current.configure(text = f"Time Between Samples: {between} m")
                self.wrong.grid_remove() 
        except:
            self.wrong.grid(row=1, column=3, padx=10, pady=15)

    def enter_duration(self):
        try:
            collection = int(self.collection_entry.get())
            if collection>255 or collection<1:
                self.wrong.grid(row=2, column=3, padx=10, pady=15)
            else:
                eco_set["Duration"] = collection
                self.collection_current.configure(self, text=f"Sample Duration: {collection} s")
                self.wrong.grid_remove()
        except:
            self.wrong.grid(row=2, column=3, padx=10, pady=15)

    def enter_delay(self):
        try:

            delay = int(self.delay_entry.get())
            if delay>36 or delay<0:
                self.wrong.grid(row=3, column=3, padx=10, pady=15)
            else:
                eco_set["Delay"] = delay
                self.delay_current.configure(text=f"Start Delay: {delay} h")
                self.delay_wrong.grid_remove()
        except:
            self.delay_wrong.grid(row=3, column=3, padx=10, pady=15)

class App(ctk.CTk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.geometry("500x200")
        self.title("Kelp Connected")
        self.connected = False
        self.COMports=[]
        #THIS BLOCK IS FOR CHOOSING COM PORT
        self.port_connected = ctk.CTkLabel(self, text="Connected!", text_color="green")
        self.port_notConnected = ctk.CTkLabel(self, text="Not Connected", text_color="red")
        self.port_upadte()
        portpicker_var = ctk.StringVar(value="Select COM Port")  # set initial value
        self.port_picker = ctk.CTkOptionMenu(self,
                                   values=self.COMports,
                                   command=self.pickports_callback,
                                   variable=portpicker_var)
        
        self.port_picker.grid(row=0, column=1, padx = 10, pady=15)
        self.refresh_button = ctk.CTkButton(self, text = "Refresh COM", command=self.port_upadte())
        self.refresh_button.grid(row=0, column=0, padx=10, pady=15)

        self.sensor_picker = ctk.CTkOptionMenu(self,
                                       values=["CTD", "ECO"],
                                       command=self.picksensor_callback)
        self.sensor_picker.grid(row=1, column=1, padx = 10, pady=15)
        self.sensor_picker.set("Select Sensor")
        self.err = ctk.CTkLabel(self, text="Device not Connected", text_color="red")
        self.toplevel_window = None

        self.deploy_button = ctk.CTkButton(self, text = "Deploy", command=self.deploy)
        self.deploy_button.grid(row=3, column=1, padx=10, pady=15)
        self.invalid = ctk.CTkLabel(self, text="Cannot Deploy", text_color="red")
        self.deployed = ctk.CTkLabel(self, text="Deployed!", text_color="green")


        #TIME
        '''
        self.system_time_label = ctk.CTkLabel(self)
        self.logger_time_label = ctk.CTkLabel(self)
        self.SYS_time()    
        self.MCU_time()
        self.time_buttons()
        '''

    def picksensor_callback(self, choice):
        if self.connected == False:
            self.err.grid(row=1, column=2, padx = 10, pady=15)
        else:
            self.invalid.grid_remove()
            self.err.grid_remove()
            if self.toplevel_window is None or not self.toplevel_window.winfo_exists():
                if choice == "CTD" :
                    self.toplevel_window = CTD_WINDOW(self)  # create window if its None or destroyed
                elif choice == "ECO":
                    self.toplevel_window = ECO_WINDOW(self)  # create window if its None or destroyed
            else:
                self.toplevel_window.focus()  # if window exists focus it

    def port_upadte(self):
        """!
        Update the list of COM ports avaialable for connection
        @param NA
        @returns NA
        """
        self.port_notConnected.grid_remove()
        self.port_connected.grid_remove()
        ports = []
        ports = serial.tools.list_ports.comports(include_links=True)
        for port in ports:
            self.COMports.append(port.device)

    def pickports_callback(self, choice):
        try:
            self.dev = serial.Serial(choice, 19200, timeout=5)
            connect = struct.pack('BBBB',2,0,0,0)
            self.dev.write(connect)
            msg = self.dev.read(1)
            if(msg.decode() == '8'):
                self.port_connected.grid(row=0, column=3, padx=10, pady=15)
                self.connected = True
                self.err.grid_remove()
            else:
                self.port_notConnected.grid(row=0, column=3, padx=10, pady=15)
        except:
            self.err.grid(row=0, column=2, padx = 10, pady=15)

    def deploy(self):
        btwn = (eco_set.get("Between"))
        dur = (eco_set.get("Duration"))
        dly = (eco_set.get("Delay"))
        if btwn ==0 or dur ==0:
            self.invalid.grid(row=3, column=3, padx=0, pady=0)
        else:
            config = struct.pack('BBBB',3,btwn,dur,dly)
            self.dev.write(config)
            msg = self.dev.read(1)
            if(msg.decode() == '8'):
                deploy = struct.pack('BBBB',1,0,0,0)
                self.dev.write(deploy)
                self.deployed.grid(row=3, column=3, padx=0, pady=0)
        

if __name__ == "__main__":
    app = App()
    app.mainloop()