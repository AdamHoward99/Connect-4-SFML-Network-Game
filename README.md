<h1 align="center">Connect 4 SFML Game</h1>
<p align="center">This application is an SFML version of the classic Connect 4 game with multiplayer functionality included using a Client-Server model.</p>

## Screenshots
<p float = "left">
  <img hspace = "10" alt ="Title Screen" src = "https://user-images.githubusercontent.com/74617187/123830371-2095a100-d8fb-11eb-831f-7eaaf9de5283.png" height = "400" width =  "400"/>
  <img alt ="Game Screenshot" src = "https://user-images.githubusercontent.com/74617187/123830824-94d04480-d8fb-11eb-9d82-9b37ad3dfff2.png" height = "400" width = "400"/>
</p>
  
## How To Use
The project files contain folders for the game, server and required SFML libraries. The settings for the game are set to local host by default but can be changed using instructions below. This application is only supported on Windows OS.

To play the game without using local host you will be required to use 2 different computers both on the same network.

Instructions:
- Run the server.exe on one of the computers.
- Open the NetworkConnection.cpp file in the Game folder.
- Locate line 32 and change the IP address value from 127.0.0.1 to the IPv4 address of the computer running the server.
- Rebuild the game application using the settings Release, x86.
- Now run the game.exe on the different computers (both computers NEED to be on the same network) and then press 'Find Match' when in game.
- For matchmaking to work, both players will need to be searching at the same time otherwise the connection will time out.


## Built With
- C++
- WinSock API
- SFML API

## Contributors

**Adam Howard**
- [Profile] (https://github.com/AdamHoward99)
