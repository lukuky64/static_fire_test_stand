# get all the avaiable COM ports to connect to
# create a connect/disconnect button + status

import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import serial
import serial.tools.list_ports
import threading
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
import csv
import time


class MyGUI:
    def __init__(self, root, comport_list):
        self.ComPorts = comport_list
        self.connectionState = False
        self.plotState = False

        self.x_data_raw = []
        self.y_data_raw = []

        self.x_data = []
        self.y_data = []
        self.start_time = None

        self.uplink_rssi = "--"
        self.downlink_rssi = "--"

        self.zero_offset = 0

        # self.max_fill = 100 # 100 Newtons by default

        # RSSI Labels
        self.label_rssi = tk.Label(root, text="Uplink: -- dBm, Downlink: -- dBm", font=("Helvetica", 12))
        self.label_rssi.grid(row=11, column=2, columnspan=3, sticky='ew')


        self.button_fire = tk.Button(root, text="FIRE", bg="black", fg="red", font=("Helvetica", 16, "bold"), command=self.open_fire_window)
        self.button_fire.grid(row=14, column=2, columnspan=3, sticky='nsew')

        # set a default location that will work on any computer
        self.save_directory = ''
        
        self.root = root
        self.style = ttk.Style()
        self.style.theme_use("aqua")  # Change the theme to clam
        self.root.title("Load Cell Monitor")
        self.root.geometry("800x600+400+100")  # Width x Height + X position + Y position
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)
        # self.root.tk.call('tk', 'scaling', 4.0)  # Adjust the scaling factor


        # Configure the grid layout of the root window
        self.row_count = 40  # Set the number of rows
        for i in range(self.row_count):  # Adjust the range based on the number of rows you have
            self.root.grid_rowconfigure(i, weight=1)
        
        self.root.grid_rowconfigure(0, weight=0)  # This makes sure the info_frame stays at the bottom
        self.root.grid_columnconfigure(0, weight=1)
        self.root.grid_columnconfigure(1, weight=1)

        # Frames
        self.data_frame = tk.Frame(root)
        self.data_frame.grid(row=2, column=0, columnspan=2, sticky='nsew')

        self.progress_frame = tk.Frame(root)
        self.progress_frame.grid(row=3, column=0, columnspan=3, sticky='nsew')

        self.info_frame = tk.Frame(root)
        self.info_frame.grid(row=self.row_count, column=0, columnspan=3, sticky='nsew')  # Positioned at the bottom

        self.plot_frame = tk.Frame(self.root, width=400, height=350)
        self.plot_frame.grid(row=20, column=0, columnspan=3, sticky="nsew")
        self.plot_frame.grid_propagate(False)  # Prevent the frame from resizing to fit the plot

        # Menu bar
        self.create_menu_bar()

        # Labels
        self.label_comPorts = tk.Label(root, text="Com Ports")
        self.label_comPorts.grid(row=0, column=0, sticky='ew')

        self.label_dataTitle = ttk.Label(self.data_frame, text="Force (N): ", font=("Helvetica", 16))
        self.label_dataTitle.grid(row=0, column=0, sticky='w', padx=10, pady=20)

        self.label_rawData = ttk.Label(self.data_frame, text="Waiting for data...", font=("Helvetica", 16))
        self.label_rawData.grid(row=0, column=1, sticky='w', padx=10, pady=20)

        # self.label_fill = ttk.Label(self.progress_frame, text="Fill Level", font=("Helvetica", 16))
        # self.label_fill.pack(pady=0)

        # self.label_max_fill = ttk.Label(root, text="Max Fill (N) ", font=("Helvetica", 14))
        # self.label_max_fill.grid(row=2, column=2, columnspan=3, sticky='w')

        # # Progress Bar
        # self.fill_bar = ttk.Progressbar(self.progress_frame, orient=tk.HORIZONTAL, length=400, mode='determinate')
        # self.fill_bar.pack(padx=100, pady=0,expand=True)


        # text boxes
        # self.textbox_max_fill = ttk.Entry(root, width=10, font=("Helvetica", 14))
        # self.textbox_max_fill.grid(row=3, column=2, columnspan=3, sticky='nsew')
        # self.textbox_max_fill.insert(0, str(self.max_fill)) # Set the default value


        # Buttons
        # self.button_connect = tk.Button(root, text="Connect", bg="green", fg="white", activebackground="#FF0000", command=self.on_connect)
        self.button_connect = tk.Button(root, text="Connect", command=self.on_connect)

        self.button_connect.config(state=tk.DISABLED, bg="white")
        self.button_connect.grid(row=1, column=1, sticky='ew')

        # self.button_zeroData = tk.Button(root, text="Zero data", bg="white", fg="white", command=self.on_zero)
        self.button_zeroData = tk.Button(root, text="Zero data", command=self.on_zero)
        self.button_zeroData.grid(row=1, column=2, sticky='ew')
        self.button_zeroData.config(state=tk.DISABLED)

        # self.set_tank_full = ttk.Button(root, text="Set tank full", command=self.saveTankFull)
        # self.set_tank_full.grid(row=10, column=2, sticky='ew')
        # self.set_tank_full.config(state=tk.DISABLED)

        self.button_update_plot = ttk.Button(root, text="Show Graph", command=self.show_or_hide_plot)
        self.button_update_plot.grid(row=12, column=2, columnspan=3, sticky='ew')

        # self.button_save_location = tk.Button(root, text="Save Location",bg='Red', fg='white', command=self.save_location)
        self.button_save_location = tk.Button(root, text="Save Location", command=self.save_location)

        self.button_save_location.grid(row=40, column=2, columnspan=3, sticky='ew')


        # Info label at the bottom
        self.label_info = tk.Label(self.info_frame, text="Info: ", font=("Calibri", 10), fg="orange")
        self.label_info.pack(side=tk.LEFT, padx=10, pady=10)


        # Dropdown Box (Combobox)
        self.dropdown = ttk.Combobox(root, values=self.ComPorts, state="readonly")
        self.dropdown.grid(row=1, column=0, columnspan=1, sticky='ew')
        if self.ComPorts:  # Check if the list is not empty
            self.dropdown.current(0)  # Set the default value if available
        else:
            self.label_info.config(text="info: No COM ports found.")

    def create_menu_bar(self):
        # Create a menu bar
        self.menu_bar = tk.Menu(self.root)
        self.root.config(menu=self.menu_bar)

        # Create a File menu item
        file_menu = tk.Menu(self.menu_bar, tearoff=0)
        file_menu.add_command(label="Open", command=self.open_file)
        file_menu.add_command(label="Save", command=self.save_file)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.on_close)
        self.menu_bar.add_cascade(label="File", menu=file_menu)

        # Create another menu for features
        features_menu = tk.Menu(self.menu_bar, tearoff=0)
        features_menu.add_command(label="Feature 1", command=self.feature1)
        features_menu.add_command(label="Feature 2", command=self.feature2)
        self.menu_bar.add_cascade(label="Features", menu=features_menu)

    # Example method placeholders for menu commands
    def open_file(self):
        pass  # Implement file opening logic

    def save_file(self):
        pass  # Implement file saving logic

    def feature1(self):
        pass  # Implement feature 1

    def feature2(self):
        pass  # Implement feature 2

    def on_close(self):
        # Your existing on_close logic
        pass

    def on_zero(self):

        if not self.y_data_raw:
            self.label_info.config(text="info: No data to zero.")
            return
        else:
            self.label_info.config(text="info: Zeroing data...")
            self.zero_offset = self.y_data_raw[-1]
            # update plot
            self.update_plot()
            # Add functionality here

    def on_connect(self):
        if not self.connectionState:
            self.connectToComPort(self.dropdown.get(), 115200)
        else:
            self.disconnectFromComPort()

    def connectToComPort(self, com_port, baud_rate):
        try:
            self.ser = serial.Serial(com_port, baud_rate)
            self.label_info.config(text="info: Connected to COM port.")
            # self.button_connect.config(text="Disconnect", bg="red")
            self.button_connect.config(text="Disconnect")
            # self.button_zeroData.config(state=tk.NORMAL, bg="blue")
            self.button_zeroData.config(state=tk.NORMAL)
            self.connectionState = True

            self.save_file = open(self.save_directory, 'a', newline='')
            self.csv_writer = csv.writer(self.save_file)
            self.csv_writer.writerow(['time (s)', 'Raw Force (N)', 'Zeroed Force (N)'])  # write headers

            self.start_time = time.time()
            self.thread = threading.Thread(target=self.read_from_port)
            self.thread.daemon = True
            self.thread.start()

            self.rssi_request_event = threading.Event()
            self.rssi_thread = threading.Thread(target=self.send_rssi_command_loop)
            self.rssi_thread.daemon = True
            self.rssi_thread.start()
            

        except serial.SerialException as e:
            message = f"info: Error connecting to {com_port}: {e}"
            self.label_info.config(text=message)

    def send_rssi_command_loop(self):
        while self.connectionState and hasattr(self, 'ser') and self.ser.is_open:
            try:
                self.ser.write(b"R\n")
            except serial.SerialException:
                break
            time.sleep(2)

    def open_fire_window(self):
        # Create a new Toplevel window
        fire_window = tk.Toplevel(self.root)
        fire_window.title("Confirm Fire Command")
        fire_window.geometry("300x150+500+300")
        fire_window.grab_set()  # Force focus to this window

        # Label
        label = tk.Label(fire_window, text="Enter Password to FIRE:", font=("Helvetica", 12))
        label.pack(pady=10)

        # Password Entry
        password_entry = ttk.Entry(fire_window, show="*")
        password_entry.pack(pady=5)

        # Button frame
        button_frame = tk.Frame(fire_window)
        button_frame.pack(pady=10)

        # Cancel Button
        cancel_button = ttk.Button(button_frame, text="Cancel", command=fire_window.destroy)
        cancel_button.grid(row=0, column=0, padx=10)

        # Proceed Button
        def proceed():
            password = password_entry.get()
            self.execute_fire_command(password)
            fire_window.destroy()

        proceed_button = ttk.Button(button_frame, text="Proceed", command=proceed)
        proceed_button.grid(row=0, column=1, padx=10)

    def execute_fire_command(self, password):
        if hasattr(self, 'ser') and self.ser.is_open:
            msg = f"F{password}\n"
            self.ser.write(msg.encode())  # send the bytes of the message
        self.label_info.config(text="info: FIRE command sent!")

    def disconnectFromComPort(self):
        if self.connectionState and hasattr(self, 'ser') and self.ser.is_open:
            # Signal the thread to stop
            self.connectionState = False
            # Wait for the thread to finish its current iteration
            if hasattr(self, 'thread'):
                self.thread.join(timeout=2.0)  # Timeout to avoid hanging indefinitely
            # Close the serial connection
            self.ser.close()
            # Update UI elements
            self.label_info.config(text="info: Disconnected from COM port.")
            self.button_connect.config(text="Connect", bg="green")
            # self.button_zeroData.config(state=tk.DISABLED, bg="white")
            self.button_zeroData.config(state=tk.DISABLED)

        if hasattr(self, 'save_file') and not self.save_file.closed:
            self.save_file.close()

        if hasattr(self, 'rssi_request_event'):
            self.rssi_request_event.set()

    def update_rssi_label(self, data_dict):
        if 'UPLINK_RSSI' in data_dict:
            self.uplink_rssi = data_dict['UPLINK_RSSI']
        if 'DOWNLINK_RSSI' in data_dict:
            self.downlink_rssi = data_dict['DOWNLINK_RSSI']
            
        self.label_rssi.config(
            text=f"Uplink: {self.uplink_rssi} dBm, Downlink: {self.downlink_rssi} dBm"
        )

    def read_from_port(self):
        while self.connectionState:
            if hasattr(self, 'ser') and self.ser.is_open and self.ser.in_waiting:
                try:
                    line = self.ser.readline().decode().strip()
                except UnicodeDecodeError:
                    self.label_info.config(text="info: Invalid data received from COM.")
                    continue
                except serial.SerialException:
                    break

                # Now, parse the incoming line
                try:
                    pairs = [pair.strip() for pair in line.split('\n') if '=' in pair]

                    data_dict = {}
                    for pair in pairs:
                        key, value = pair.split('=')
                        key = key.strip()
                        value = value.strip()
                        data_dict[key] = value

                    # ----- Correct location: after all pairs processed -----

                    # Handle RSSI values (must check entire data_dict, not just last key)
                    if 'UPLINK_RSSI' in data_dict or 'DOWNLINK_RSSI' in data_dict:
                        self.update_rssi_label(data_dict)

                    # Process Force if available
                    if 'Force' in data_dict:
                        force_str = data_dict['Force'].replace('N', '')  # Remove unit
                        force_val = float(force_str)

                        # Update raw and zeroed force
                        current_time = time.time() - self.start_time
                        self.x_data_raw.append(round(current_time, 2))
                        self.y_data_raw.append(force_val)

                        self.x_data = self.x_data_raw
                        self.y_data = [round(y - self.zero_offset, 2) for y in self.y_data_raw]

                        self.csv_writer.writerow([self.x_data_raw[-1], self.y_data_raw[-1], self.y_data[-1]])
                        self.save_file.flush()

                        self.update_label()

                    # Process State if available
                    if 'State' in data_dict:
                        self.label_info.config(text=f"State: {data_dict['State']}")

                except Exception as e:
                    self.label_info.config(text=f"info: Error parsing data: {e}")
                    continue


    def update_label(self):
        if not self.y_data == []:
            latestData = self.y_data[-1]
            self.label_rawData.config(text=latestData)

            # remap between 0 and 100
            # latestDataMapped = np.interp(latestData, [0, self.textbox_max_fill.get()], [0, 100])

            # try:
            #     maxVal = float(self.textbox_max_fill.get())
            #     if maxVal < 1:
            #         maxVal = 100
            #         self.label_info.config(text="info: Minimum fill value is 1.")

            # except ValueError:
            #     maxVal = 100  # Replace DEFAULT_VALUE with your actual default value
            #     self.label_info.config(text="info: Invalid data received in textBox.")

            # self.fill_bar['value'] = int(100*latestDataMapped/maxVal)
            
            if self.plotState:
                self.update_plot()

    def update_plot(self):
        self.line.set_xdata(self.x_data)
        self.line.set_ydata(self.y_data)

        self.ax.set_xlim(min(self.x_data), max(self.x_data))  # Set x-axis limit
        self.ax.set_ylim(min(self.y_data), max(self.y_data))
        
        self.ax.draw_artist(self.ax.patch)
        self.ax.draw_artist(self.line)
        self.canvas.draw()
        self.canvas.flush_events()

    def saveTankFull(self):
        # display a pop up asking if the user is sure they wish to continue
        confirm = messagebox.askyesno("Confirmation", "Are you sure you want to save the current value as the max fill? (You cannot undo this)")
        if confirm:
            self.max_fill = float(self.textbox_max_fill.get())
            self.label_info.config(text=f"info: Max fill set to {self.max_fill} N")

            # grey out the button 
            self.set_tank_full.config(state=tk.DISABLED)

    def show_or_hide_plot(self):
        if not self.plotState:
            self.plotState = True
            self.button_update_plot.config(text="Hide Graph")

            if not hasattr(self, 'canvas'):  # Create the plot only if it doesn't exist
                # Plot
                self.fig, self.ax = plt.subplots(figsize=(10, 4), dpi=80)  # Width, Height in inches
                
                # Set dark background
                self.fig.patch.set_facecolor('#313233')  # Figure background color
                self.ax.set_facecolor('#313233')  # Axes background color

                self.line, = self.ax.plot([], [], 'r-')  # Initial empty line
                self.ax.set_xlim(0, 100)  # Set x-axis limit default
                self.ax.set_ylim(-500, 500)  # Set y-axis limit

                # Embed the plot in the Tkinter window
                self.canvas = FigureCanvasTkAgg(self.fig, master=self.plot_frame)
                self.canvas.draw()

            # Either way, make sure the canvas is visible
            self.canvas.get_tk_widget().grid(row=20, column=0, columnspan=3, sticky="nsew")
        
        else:
            self.plotState = False
            self.button_update_plot.config(text="Show Graph")
            # Hide the plot
            if hasattr(self, 'canvas'):
                self.canvas.get_tk_widget().grid_forget()

    def on_close(self):
        # Perform any necessary cleanup here
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()  # Close serial port if open
        plt.close('all')
        self.root.destroy()  # This ensures the main window closes properly

    def save_location(self):
        # change save location
        self.save_directory = tk.filedialog.asksaveasfilename(defaultextension=".csv", filetypes=[("CSV files", "*.csv")])
        # check if it has been changed
        if self.save_directory:
            self.label_info.config(text=f"info: Save location: {self.save_directory}")
            # self.button_save_location.config(bg="Green")
            self.button_connect.config(state=tk.NORMAL, bg="Green")
            self.set_tank_full.config(state=tk.NORMAL)
        else:
            self.label_info.config(text="info: No new save location selected.")
        pass



def main():
    com_ports = serial.tools.list_ports.comports()
    available_ports = [port.device for port in com_ports]
    root = tk.Tk()
    app = MyGUI(root, available_ports)
    root.mainloop()

if __name__ == "__main__":
    main()