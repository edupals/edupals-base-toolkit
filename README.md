# edupals-base-toolkit


This is a simple C++ toolkit for Linux. It should keep small, UI toolkit independent and have no other C/C++ dependencies.

Actually the following modules are working:
* cmd: for command line argument parsing
* console: for common console actions, color and styles
* log: thread-safe console log
* workqueue: a thread-safe consumer-producer queue
* filesystem: some fs goodies, as glob
* network: list network devices. Get IP status will come soon
* system: get processes, kernel version, boot parameters, loaded modules,...
