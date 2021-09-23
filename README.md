# EditFile
A terminal based text editor project for university

## Functions

### Create File:
In order to create files I utilised the fopen function. When fopen is given a file name and “w” as a parameter it creates a new file with the file name that has been passed. 
I decided to also set the newly created file as the current open file that the text editor was working on, as usually when you create a new file you want to start working on it straight away making the process of starting a new document more efficient.

### Copy File:
My implementation will copy the current selected file to a new file which the user is asked to name.
I decided that it makes sense for the user to have to open the file they want to copy so that they will see the contents of the file they have selected. Reducing the chance they copy the wrong file.
The code will check that a file with the same name doesn’t already exist as this would overwrite the file rather than create a new copy.
The program then will iterate through the entire of the open file writing each character to the new copied file until the end of the file has been reached in which case both files are then closed.

### Delete File:
My deletefile implementation works similarly to the copy file method. The currently selected file is deleted in order to force the user to check that this is the file they intend to delete. 
The program also requires confirmation that the user does intend to delete this file, as it is very easy to mistakenly delete the wrong file therefore a double check is suitable.

### Show File:
The showfile method displays the title of the current open file at the top of the screen. This is so that if the user is switching between many different files they wont lose track of which file is currently open.
The method also determines how much of the contents of the file can be displayed on the screen based off of the row variable.
In order to deal with files that contain more than a page of content I have implemented two commands that work with the ShowFile method.
The variable lineBuff stores the value of the first line which should be printed to the screen, this value can be altered using the commands “up” and “down”. This simulates scrolling up and down through the file and therefore allows the user to view the entire contents of the file.

### Append line:
The append line method takes a line as input which it adds to the end of the file. The length of the line has a maximum length of the width of the screen due to the UI library that I am using.

### Delete line:
The user is prompted to enter the line they want to remove from the current open file.
In order to remove characters from a file I have chosen to create a copy of the current open file and omit the selected line from the new copy.
A temporary copy file is created in which the entire contents of the file are copied to until a variable which counts the current line is equal to the line which is chosen to be deleted. 
The original file is then deleted and the Temporary copy is renamed to the name of the original file, having the effect of removing the line from the file.

### Insert Line:
The insert line function works in a similar way to the delete line function. The user is prompted to enter the line number they would like to insert to. The program creates a temporary copy file and copies every character 1 by 1 whilst simultaneously counting the current line.
Once the chosen line has been reached the user is then prompted to enter the line they wish to insert. The program will then enter that line into the file before proceeding to copy the rest of the original file over. 
The original file is then deleted and the temporary copy file is renamed to match the original file.

### Show line:
The show line function has a similar implementation to the show file function. The only difference is that the condition for printing characters has changed.
The program prompts users for the line number they want to view the contents of, and the program iterates through the file and will display characters once the line counting variable reaches the value of the chosen line.

### Show Changelog:
In order to display the changelog to the screen I have chosen to reuse the code for the show file function, however, rather than taking user input for the name of the file, the program will automatically open “ChangeLog.txt”.
A different linebuff variable has been used in this function as well. This is so that the scrolling feature of both the change log and current open file do not interfere with each other.

### Count lines:
In order to identify a new line in a text file the program looks for ‘\n’ which indicates that a new line should be started. This function has been designed to return the number of lines that have been counted. This is because I can make use of this function in other parts of my program.
To implement this feature in line with the specification I wrote a function called printLineCount(), that will display the number of lines returned by the function +1 as the countLines function is zero indexed.

### Additional Features:
The main weakness that I wanted to address in this program was how files are navigated, as a result the purpose of both of my additional features is to make navigating through files easier for the user.
I have chosen to implement A more sophisticated text based interface. This is something that comes standard with any text editor as it allows users to see the changes made to their file as they are made. This is a useful feature for users as it allows users to work more efficiently, as they are not required to enter commands to view the changes they have made or commit them to memory and as a result can commit multiple changes to a file without interruption.
I also implemented a function that would display the line numbers of each line, this means the user is not required to count each line in order to use the commands to use the software. This can be especially frustrating for longer files as this process begins to waste a lot of time especially when mistakes are made and additional commands are then required.
