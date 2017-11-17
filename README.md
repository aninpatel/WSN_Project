TDMA Emulator in C++
2 folders are present namely activity_node and TDMA_node

TDMA_node contains code for Generating Node and channel manager.
activity_node contains code to send data packet to node(s) created by TDMA_node.

Follow these steps to execute the code.

1. Run the object file present in /TDMA_node/bin/Debug using ./TDMA_node 
2. It will ask for the number of nodes you want to creat.
3. Then, it will start asking for ip address and port number for these nodes. You can provide any IP address but it wil always take local host address (which is IP address 127.0.0.1) and even though you give port number explicitely it takes a random port number. But it is recommended that you give port number greater than 5000.
4. Then it will ask for location parameters (x and y co-ordinates) for node.
5. Then it will print the port number it is listening on, please note that down.
6. Now use the same port numbert in activity_node code. As of now it is hard coded, so you need to set it every time you run TDMA_node program. 
7. Once you set the port number, compile the code and run it, it will ask you for the message. Enter the message and press enter, you will the same message in output of TDMA_node.

Note: I am gonna fix the issue with port number present here in activity_node code. So, it is absolutely fine if you don't run the code and just see the code.
