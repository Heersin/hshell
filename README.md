# hshell
 A unix-based shell demo 

## Description
A Shell for Unix terminal, this source code based on smsh(I will push it's source later).  I write hshell to figure out how a shell works.

## Build
```sh
cd src
make && make clean
```

## Support Features
- smsh support the following features
    * execute command
    * varible
    * simple script
    * Exit 

- hshell support more:
    * history
    * delete command char
    * moving cursor
    * Insert char mode
    * auto complete hint
    * simple redirect
    * simple pipe
    * clear screen

## Usage
### History
push up arrow will get your last command, you can use up and down arrow key to change cmd in your history inputs. The history file is "./hsh_history" 
![history](https://github.com/Heersin/hshell/blob/master/doc/history.gif)
### Delete Char
push backspace will delete the command char

### Cursor Moving
push Left and Right Arrow key can move the cursor

### Insert Mode
You can insert char in front of the cursor

### Auto Complete hint
Tab to get the hint of your command, but hsh won't complete your command
### Simple redirect
Support simple redirect : only process one redirect simple 
```
[cmd] > [file]
    echo "hello" > test.txt
[cmd] >> [file]
    echo "world" >> test.txt
[cmd] < [file]
    echo < test.txt
```
![tab_redirect](https://github.com/Heersin/hshell/blob/master/doc/tab_redirect.gif)
### simple pipe
Support max 8 cmd to pipe
```
cmd1 | cmd2 | cmd3 ...
    >>$ ls | wc
    >>$ 0 14 16
```
![pipe](https://github.com/Heersin/hshell/blob/master/doc/pipe_exit.gif)
### Clear and Exit
Clear Screen : ctrl-L<br>
Exit : ctrl-D
![clear](https://github.com/Heersin/hshell/blob/master/docs/clear.gif)

## Contribution
Reconstruct This Project code is welcome~