import subprocess
import sys
import os

def is_module_installed(module_name):
    """Check if the module is installed"""
    try:
        subprocess.check_output([sys.executable, "-m", "pip", "show", module_name], stderr=subprocess.STDOUT)
        return True
    except subprocess.CalledProcessError:
        return False

def get_module_version(module_name):
    """Get the installed version of a module"""
    try:
        result = subprocess.check_output([sys.executable, "-m", "pip", "show", module_name], stderr=subprocess.STDOUT)
        for line in result.decode().split('\n'):
            if line.startswith('Version:'):
                return line.split(':')[1].strip()
        return None
    except subprocess.CalledProcessError:
        return None

def install_module(module_name, version=None):
    """Install the module with optional version specification"""
    if version:
        module_spec = f"{module_name}=={version}"
        print(f"{module_name} version {version} is not installed. Installing now...")
    else:
        module_spec = module_name
        print(f"{module_name} is not installed. Installing now...")
    
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", module_spec])
        if version:
            print(f"{module_name} version {version} installation completed.")
        else:
            print(f"{module_name} installation completed.")
    except Exception as e:
        print(f"Error occurred while installing {module_name}: {e}")
        sys.exit(1)

def uninstall_module(module_name):
    """Uninstall the module"""
    print(f"Uninstalling {module_name}...")
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "uninstall", module_name, "-y"])
        print(f"{module_name} uninstalled successfully.")
    except Exception as e:
        print(f"Error occurred while uninstalling {module_name}: {e}")
        sys.exit(1)

def check_and_install_tkinter():
    """Check if tkinter is available, attempt to install if not"""
    try:
        import tkinter
        print("tkinter is available.")
        return True
    except ImportError:
        print("tkinter is not available in this Python installation.")
        print("Trying to install tkinter...")
        
        # Try to install tkinter via system package manager (for Linux)
        if sys.platform.startswith("linux"):
            try:
                # Try apt (Debian/Ubuntu)
                subprocess.check_call(["sudo", "apt", "update"])
                subprocess.check_call(["sudo", "apt", "install", "-y", "python3-tk"])
                print("Successfully installed tkinter via apt.")
                return True
            except (subprocess.CalledProcessError, FileNotFoundError):
                try:
                    # Try yum (CentOS/RHEL/Fedora)
                    subprocess.check_call(["sudo", "yum", "install", "-y", "tkinter"])
                    print("Successfully installed tkinter via yum.")
                    return True
                except (subprocess.CalledProcessError, FileNotFoundError):
                    print("Could not automatically install tkinter. Please install it manually.")
                    return False
        else:
            print("Please install tkinter manually for your system.")
            return False

def select_firmware_tkinter():
    """Select firmware using tkinter dialog with numbered options"""
    import tkinter as tk
    from tkinter import ttk
    
    def select_option(option):
        nonlocal selected
        selected = option
        root.quit()
        root.destroy()
    
    root = tk.Tk()
    root.title("Firmware Selection")
    root.geometry("300x150")
    root.resizable(False, False)
    
    # Center the window
    root.update_idletasks()
    x = (root.winfo_screenwidth() // 2) - (300 // 2)
    y = (root.winfo_screenheight() // 2) - (150 // 2)
    root.geometry(f"300x150+{x}+{y}")
    
    # Create UI elements
    label = tk.Label(root, text="Select firmware:", font=("Arial", 12))
    label.pack(pady=10)
    
    button_frame = tk.Frame(root)
    button_frame.pack(pady=10)
    
    button1 = tk.Button(button_frame, text="1. firmware_cfg1.bin", 
                       width=20, command=lambda: select_option("firmware_cfg1.bin"))
    button1.pack(pady=2)
    
    button2 = tk.Button(button_frame, text="2. firmware_cfg2.bin", 
                       width=20, command=lambda: select_option("firmware_cfg2.bin"))
    button2.pack(pady=2)
    
    selected = None
    root.mainloop()
    
    return selected

def select_firmware_console():
    """Select firmware using console input"""
    while True:
        choice = input("Select firmware (1 or 2):\n1. firmware_cfg1.bin\n2. firmware_cfg2.bin\nEnter your choice (1 or 2): ")
        if choice == "1":
            return "firmware_cfg1.bin"
        elif choice == "2":
            return "firmware_cfg2.bin"
        else:
            print("Invalid choice. Please enter 1 or 2.")

def select_firmware():
    """Select firmware with cross-platform support"""
    # Try to use tkinter first
    try:
        import tkinter
        try:
            return select_firmware_tkinter()
        except tkinter.TclError:
            print("GUI dialog failed. Falling back to console input.")
            return select_firmware_console()
    except ImportError:
        print("tkinter not available. Using console input.")
        return select_firmware_console()

def run_esptool_command(selected_firmware):
    """Execute the esptool command with selected firmware"""
    command = [
        sys.executable, "-m", "esptool",
        "--chip", "esp32s3",
        #"--port", "COMx",
        "--baud", "2000000",
        "--before", "default-reset",
        "--after", "hard-reset",
        "write-flash",
        "-z",
        "--flash-mode", "keep",
        "--flash-freq", "keep",
        "--flash-size", "keep",
        "0x0", "bin/bootloader.bin",
        "0x8000", "bin/partitions.bin",
        "0xe000", "bin/boot_app0.bin",
        "0x10000", f"bin/{selected_firmware}",
        "0x670000", "bin/spiffs.bin",
    ]
    try:
        print(f"Executing esptool command with {selected_firmware}...")
        subprocess.check_call(command)
        print("esptool command executed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"esptool command execution failed: {e}")
        sys.exit(1)

def main():
    module_name = "esptool"
    required_version = "5.0.1"
    
    # Check if esptool is installed
    if not is_module_installed(module_name):
        install_module(module_name, required_version)
    else:
        # Check if the installed version matches the required version
        current_version = get_module_version(module_name)
        if current_version != required_version:
            print(f"Installed version {current_version} does not match required version {required_version}")
            uninstall_module(module_name)
            install_module(module_name, required_version)
        else:
            print(f"{module_name} version {required_version} is already installed.")

    # Check and potentially install tkinter
    check_and_install_tkinter()

    # Select firmware
    selected_firmware = select_firmware()
    print(f"You selected: {selected_firmware}")

    # Execute esptool command with selected firmware
    run_esptool_command(selected_firmware)

if __name__ == "__main__":
    main()