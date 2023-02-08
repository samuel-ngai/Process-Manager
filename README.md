# Process Manager

Assignment 1 for UVic CSC 360, Operating Systems.
Samuel Ngai, V00917462

## How to run
Navigate to p1s folder, and use "*make run*". 
Supported commands: 
* bg [Executable] [Arguments]
* bglist 
* bgkill [PID]
* bgstart [PID]
* bgstop [PID]
* pstat [PID]
* exit

The exit command is a custom command, not specified by the assignment writeup.

## Usage
To execute a program in the background, type in "bg (Executable)". If the executable accepts  arguments, then arguments can also be inputted by "bg (Executable) (Arguments)"

### Other command descriptions:

* #### bglist 
  * Lists out all currently executing or paused processes.
* #### bgkill (PID)
  * Terminates a process based on PID. 
* #### bgstart (PID)
  * Resumes a process based on PID.  
* #### bgstop (PID)
  * Pauses a process based on PID.
* #### pstat (PID)
  * Lists out information related to our process based on PID:
    * comm
    * state
    * utime
    * stime
    * rss
    * voluntary context switches
    * nonvoluntary context switches
* #### exit
  * Exits the PMan program.
