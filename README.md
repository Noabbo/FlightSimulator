# Automatic FlightSimulator
github: https://github.com/Noabbo/FlightSimulator

Welcome to our program's repo, this file contains instructions and information to help you with anything on your first setup.
The main purpose of this repo is for a learning course (Advanced Programming).

# How does it work?
Run the program and then the simulator.
# What is the goal of this project?
An assignment to study tcp-socket server/client, implementing command pattern and more..

# Instructions for how to use, and test the code
(FOR LINUX USERS)

      1. Install FlightGear simulator software from your software manager/terminal.
      2. Go to "/usr/share/games/flightgear/Protocol" on your machine. (note: you might want to open the folder with root privillages).
      3. Copy "generic_small.xml" file from this repo to folder.
      4. Compile the above files.
      5. Open up FlightGear app and go to settings.
      6. Under "Additional Settings" add the following:
            6.a --telnet=socket,in,10,127.0.0.1,5402,tcp
            6.b --generic=socket,out,10,127.0.0.1,5400,tcp,generic_small
            NOTE: make sure there are no whitespaces or chars sneaking in there!*
      7. Run the C++ program in this repo.
      8. Run the FlightGear Simulator (Press Fly!)
      9. And the rest is automatic!

# Contact us

If you have any suggestions\questions please feel free to contact us at : noabbo96@gmail.com or fannyguthmann@gmail.com
