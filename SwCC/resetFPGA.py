import argparse
from os import popen, system
import getpass

def toInt(i):
    try:
        ret = int(i)
    except ValueError:
        ret = 0
    return ret

if __name__ == "__main__":

    if not getpass.getuser() == "root":
        print("\033[41mERROR: Please run this script as root!\033[0m")
        quit(1)

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--module_path", default=None, type=str, help="Path of QDMA driver module file, usually ends with qdma-pf.ko")
    parser.add_argument("-b", "--bus_id", default=None, type=str, help="Bus ID of FPGA in hex such as `1a` or `40`.")
    parser.add_argument("-q", "--queue_num", default=1, type=int, help="Number of queues you wanna create.")
    args = parser.parse_args()

    modulePath  = args.module_path
    busId       = args.bus_id
    qNum        = args.queue_num

    # Check arguments
    
    if qNum <= 0:
        print("\033[41mERROR: `queue_num` should be larger than 0.\033[0m")
        quit(1)
    if modulePath == None:
        # Manually choose module path.
        f = popen("locate qdma-pf.ko")
        lines = f.readlines()
        if len(lines) == 0:
            print("\033[41mERROR: No QDMA driver found.\033[0m")
            quit(1)
        elif len(lines) == 1:
            modulePath = lines[0].strip()
        else:
            print(
                "Multiple possible locations of QDMA driver found.\n"
                "Please choose the driver you want to load:"
            )
            for idx, line in enumerate(lines):
                print(f"[\033[36m{idx+1}\033[0m] {line.strip()}")
            choice = toInt(input("Enter number here: "))
            while choice <= 0 or choice > len(lines):
                print("Invalid input. Please try again.")
                choice = toInt(input("Enter number here: "))
            modulePath = lines[choice-1].strip()
    print(f"QDMA driver module path: \033[33m{modulePath}\033[0m")
    if busId == None:
        # Manually choose FPGA bus ID.
        f = popen("lspci | grep Xilinx")
        lines = f.readlines()
        if len(lines) == 0:
            print("\033[41mERROR: No FPGA card found.\033[0m")
            quit(1)
        elif len(lines) == 1:
            busId = lines[0][:2]
        else:
            print(
                "Multiple FPGA card found.\n"
                "Please choose the card you want to program:"
            )
            for idx, line in enumerate(lines):
                print(f"[\033[36m{idx+1}\033[0m] {line.strip()}")
            choice = toInt(input("Enter number here: "))
            while choice <= 0 or choice > len(lines):
                print("Invalid input. Please try again.")
                choice = toInt(input("Enter number here: "))
            busId = lines[choice-1][:2]
    print(f"FPGA device: \033[33m{busId}:00.0\033[0m")

    # Main execution
    print("Please make sure FPGA is running now and not re-programmed yet.")
    print("Otherwise FPGA will not run correctly after programmed.")
    system(f"cp /sys/bus/pci/devices/0000:{busId}:00.0/config myConfig")
    system("rmmod qdma-pf")
    print("Now, program the FPGA.")
    _ = input("\033[30;46mPress Enter after programming process finishes.\033[0m")
    system(f"cp myConfig /sys/bus/pci/devices/0000:{busId}:00.0/config")
    system("rm myConfig")
    system(f"insmod {modulePath}")
    system(f"chmod 666 /sys/bus/pci/devices/0000:{busId}:00.0/qdma/qmax")
    system(f"echo 1024 > /sys/bus/pci/devices/0000:{busId}:00.0/qdma/qmax")
    qid = 0
    while qid < qNum:
        system(f"dma-ctl qdma{busId}000 q add idx {qid} mode st dir bi")
        system(f"dma-ctl qdma{busId}000 q start idx {qid} dir bi desc_bypass_en pfetch_bypass_en")
        qid += 1