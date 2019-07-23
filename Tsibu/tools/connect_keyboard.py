from subprocess import Popen, PIPE
from time import sleep

def execute():
	bt_process = Popen(
		['bluetoothctl'],
		stdin=PIPE,
		stdout=PIPE
	)
	sleep(0.1)
	
	bt_process.stdin.write("power on\n")
	sleep(0.1)
	
	bt_process.stdin.write("agent on\n")
	sleep(0.1)
	
	bt_process.stdin.write("trust CC:C5:0A:27:C8:48\n")
	sleep(0.1)
	
	bt_process.stdin.write("connect CC:C5:0A:27:C8:48\n")
	sleep(0.1)
	
	bt_process.kill()

if __name__ == '__main__':
	execute()
