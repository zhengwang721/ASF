#*************************************************
#
#  Connect to J-Link and debug application in sram.
#

# define 'reset' command
define reset

# Connect to the J-Link gdb server
target remote localhost:2345

# Load the program
load
info reg

# end of 'reset' command
end
