#include <curses.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

int row,col;
int lineBuff,lineBuffCL = 0;
int seeFile;
char CurrentFname[99];//Current Open files name

//function - clearMessageLine
//Deletes instruction text on the gui
int clearMessageLine(){
    //for each column on the screen
    for(int i = col; i > -2 ; i--){
        mvprintw(row -3,i," ");//overwrite the character in this column
    }
    move(row-1, 0);//move the cursor to take input
}

//function - clearInputLine
//Deletes the user input text on the gui
int clearInputLine(){
    //for each column on the screen
    for(int i = col; i > -2 ; i--){
        mvprintw(row -1,i," ");//overwrite the character in this column
    }
    move(row-1, 0);//move the cursor to take input
}

//function - countlines
//counts the number of lines present in the open file
int countLines(){
    //Declares a filepointer and points it to the current open file
    FILE *fp;
    fp = fopen(CurrentFname,"r");
    //iterates through the file counting the number of newline chars '\n'
    char c;
    int lineCounter = 0;
    while(c != EOF){
        c = fgetc(fp);
        if(c == '\n'){
            lineCounter++;//increments the line counter for each line
        }
    }
    //outputs the number of lines in the file
    return lineCounter;
}

//function printLineCount
//uses the countLines function to display the number of lines of the currently selected file
int printLineCount(){
    //overwrite all characters on the screen
    for(int i = 0; i < col ; i++){
        for(int j = 2; j < row-4; j++){
            mvprintw(j,i," ");
        }
    }

    mvprintw(2,0,"This file has %d line(s)",countLines()+1);
    clearInputLine();
    clearMessageLine();
    mvprintw(row-3,0,"Press any key to continue");
    move(row-1,0);
    getch();
}

//function - showALine
//outputs the contents of a particular line to the screen
int showALine(){
    
    //overwrite all characters on the screen
    for(int i = 0; i < col ; i++){
        for(int j = 2; j < row-4; j++){
            mvprintw(j,i," ");
        }
    }

    //declare a file pointer and point it to the current open file
    FILE *fp = fopen(CurrentFname,"r");
    
    char currentC;
    int lineCounter = 1;

    //take the value of desired line as input
    int seeLine = 0;
    mvprintw(row -3,0,"What line would you like to see:\n");
    clearInputLine();
    scanw("%d",&seeLine);

    //read through the entire file
    move(2,0);
    while(currentC != EOF){
        
        currentC = fgetc(fp);

        if(lineCounter == seeLine){//if the character is on the desired line
            addch(currentC);//write this character to the screen
        }
        if(currentC == '\n'){//if we are on a new line
            lineCounter++;//increment the line counter
        }
    }
    //pause the program until the user enters any keypress
    clearMessageLine();
    mvprintw(row-3,0,"Press any key to continue:");
    clearInputLine();
    getch();

    //close the file
    fclose(fp);
    clearInputLine();
}

//function - showFile
//displays the contents of the file to the screen
int showFile(){
    
    //display the filename at the top of the screen
    mvprintw(0,(col/2)-strlen(CurrentFname),CurrentFname);

    //overwrite all characters on the screen
    for(int i = 0; i < col ; i++){
        for(int j = 2; j < row-4; j++){
            mvprintw(j,i," ");
        }
    }

    //declare a file pointer and point it to the current open file
    FILE *fp = fopen(CurrentFname,"r");
    
    //read through the file
    move(2,0);
    char currentC;
    int lineCounter = 1;
    while(currentC != EOF){
        currentC = fgetc(fp);
        //if the current character is after the linebuffer and will still fit into the window
        if(lineBuff < lineCounter && lineCounter < lineBuff + row -5){
            addch(currentC);//print the character to the screen
        }
        if(currentC == '\n'){//if we are on a newline
            lineCounter++;//increment the line counter
        }
    }
    //close the file
    fclose(fp);
    clearInputLine();
}

//function - showLnums
//Outputs the contents of a file to the screen including the line numbers 
int showLnums(){
    
    clearMessageLine();
    mvprintw(row-3,0,"press any key to continue:");
    clearInputLine();
    //overwrite all characters on the screen 
    for(int i = 0; i < col ; i++){
        for(int j = 2; j < row-4; j++){
            mvprintw(j,i," ");
        }
    }
    //calculate the number of digits of the largest linenumber on the screen
    int i = 0;
    int digits=-1;
    while(digits == -1){
        if(lineBuff+row-5 < pow(10,i)){
            digits = i;
        }
        i++;
    }
    //declare a filepointer and point it to the current open file
    FILE *fp = fopen(CurrentFname,"r");
    
    //print the line number and "|" to each line
    for(int i = 0; i < row-6 ; i++){
        mvprintw(2+i,0,"%d",lineBuff+i+1);
        mvprintw(2+i,digits,"|");
    }
    
    move(2,digits+1);
    //read through the whole file
    char currentC;
    int lineCounter = 1;
    while(currentC != EOF){
        currentC = fgetc(fp);
        if(lineBuff < lineCounter && lineCounter < lineBuff + row -5){//if the current character is after the linebuffer and will still fit into the window
            addch(currentC);//print the character to the screen
        }
        if(currentC == '\n'){//if we are on a newline
            lineCounter++;//increment the line counter
            move(lineCounter-lineBuff+1,digits+1);//more the cursor to print characters to the right of the line numbers
        }
    }
    fclose(fp);
    clearInputLine();
    getch();
}

//function - OpenFile
//Selects the name of a file to be opened/created
int OpenFile(){

    //overwrite all characters on the screen 
    for(int i = 0; i < col ; i++){
        for(int j = 2; j < row-4; j++){
            mvprintw(j,i," ");
        }
    }
    //take the filename as input
    clearMessageLine();
    mvprintw(row -3,0,"Select an existing file to be opened");
    do{
        clearInputLine();
        getstr(CurrentFname);//set the file as the current open file
    }while(access(CurrentFname,F_OK) != 0 && strcmp(CurrentFname,"quit") != 0);//Check the file is valid or allow user to exit using "quit"
    if(strcmp(CurrentFname,"quit") == 0){
        strncpy(CurrentFname,"$$FILENAMEHASNOTBEENCHOSEN$$",99);//Reset current file name value to rouge value
    }
}

//function - AppendLine
//Adds a line to the end of a file
int AppendLine(){

    //Declares a filepointer and points it to the current open file
    FILE *fp;
    fp = fopen(CurrentFname,"a"); 
    
    //takes the new line to be appended as input
    char Line[col];
    clearMessageLine();
    mvprintw(row -3,0,"Enter the new line");
    clearInputLine();
    getstr(Line);
    if(*Line == ' '){//avoid adding empty lines to file
        clearMessageLine();
        mvprintw(row -3 , 0 , "Empty string is not a valid line press any key to continue:");
        clearInputLine();
        getch();
    }else{
        //writes the inputted line on a new line
        fputc('\n',fp);
        fputs(Line,fp);

        //closes the file
        fclose(fp);

        //gets the date and time
        time_t t;
        time(&t);

        //opens the changelog and writes an entry containing what has changed and when
        FILE *clP;
        clP = fopen("ChangeLog.txt","a");
        fputs("Line has been appended to file: ",clP);
        fputs(CurrentFname, clP);
        fprintf(clP," lines: %d - %s",countLines(), ctime(&t));
        fputc('\n',clP);
        //close the change log
        fclose(clP);
    }
}

//function - insertLine
//Puts a line in the file on a particular line
int insertLine(){
    //Declares two file pointers one pointing to the currently selected file and another pointed to a temporary file
    FILE *fp;
    FILE *nfp;
    fp = fopen(CurrentFname,"r");
    nfp = fopen("TEMPCOPYFILE.txt","w");
    //Takes the line number of the new line to be inserted as input
    int insertNum = 0;
    mvprintw(row -3,0,"What line would you like to insert to:");
    move(row -1,0);
    scanw("%d",&insertNum);
    //Iterates through the file counting the current line
    char c;
    int lineCounter = 1;
    while(c != EOF){

        //if at new lines location
        if(lineCounter == insertNum){
            //Take the new line as input
            char Line[col];
            clearMessageLine();
            mvprintw(row -3,0,"Enter New Line:");
            clearInputLine();
            getstr(Line);
            
            //insert the new line into the file
            fputs(Line,nfp);

            //move onto the next line of the file
            fputc('\n',nfp);
            lineCounter++;//Increment line counter
        }
        //get next character of the file
        c = fgetc(fp);
        //write this character to the new file
        fputc(c,nfp);
        if(c == '\n'){//if this char is a new line
            lineCounter++;//increment linecounter
        }
    }

    //remove the original file
    remove(CurrentFname);
    rename("TEMPCOPYFILE.txt",CurrentFname);//rename the temporary file to the original files name

    //Close both files
    fclose(fp);
    fclose(nfp);

    if(insertNum < 0 || insertNum > countLines()){//if an invalid line is chosen
        clearMessageLine();
        mvprintw(row -3, 0 , "please enter a valid line number press any key to continue:");
        clearInputLine();
        getch();
    }else{
        //gets the date and time
        time_t t;
        time(&t);
        
        //open the changelog and write an entry
        FILE *clP;
        clP = fopen("ChangeLog.txt","a");
        fputs("Line has been inserted to file: ",clP);
        fputs(CurrentFname, clP);
        fprintf(clP," lines: %d - %s",countLines(), ctime(&t));
        
        //close the change log
        fclose(clP);
    }
}

int deleteLine(){

    //Declares two file pointers and points one to our open/selected file and another to a temporary copy file
    FILE *ogF;
    FILE *nwF;
    nwF = fopen("TEMPCOPYFILE.txt","w");
    ogF = fopen(CurrentFname,"r");
    //take the line to be deleted as input
    int chosenLine;
    clearMessageLine();
    mvprintw(row -3,0,"Which line would you like to delete:");
    move(row -1,0);
    scanw("%d",&chosenLine);
    //copies the selected file to the temporary file excluding the deleted line
    char c;
    int lineCount = 1;
    while(c!=EOF){
        //counts the current line being read
        if(c == '\n'){
            lineCount++;//increment line counter
        }
        //retrieve the next character of the file
        c = fgetc(ogF);
        if(lineCount != chosenLine && c != EOF){//if the current line isnt being deleted
            fputc(c,nwF);//write this character to the temporary file
        }
    }
    //delete the original file and rename the temp file to the name of the original file
    remove(CurrentFname);
    rename("TEMPCOPYFILE.txt",CurrentFname);
    
    //close both files
    fclose(nwF);
    fclose(ogF);

    //gets the date and time
    time_t t;
    time(&t);

    if(chosenLine < 0 || chosenLine > countLines()){//if an invalid line is chosen dont write to changelog
        //Output helpful message
        clearMessageLine();
        mvprintw(row -3, 0 , "Please enter a valid line number press any key to continue:");
        clearInputLine();
        getch();
    }else{
        //Open the change log and write an entry containing what happened and when
        FILE *clP;
        clP = fopen("ChangeLog.txt","a");
        fputs("A line has been deleted from the file: ",clP);
        fputs(CurrentFname, clP);
        fprintf(clP," lines: %d - %s",countLines(),ctime(&t));
        fputc('\n',clP);
        fclose(clP);
    }
}

int CreateFile(){
    FILE *nfp;
    clearMessageLine();
    mvprintw(row -3,0,"Enter the name of your new file:");
    move(row -1,0);
    //take name of file as input
    char name[col];
    scanw(name);
    //create file 
    nfp = fopen(name,"w");
    fclose(nfp);
    //sets the file as the current open file
    for(int i = 0 ; i < 99 ; i++){
        CurrentFname[i] = name[i];
    }
    //gets the date and time
    time_t t;
    time(&t);

    //opens change log and adds an entry containing what happened and when
    FILE *clP;
    clP = fopen("ChangeLog.txt","a");
    fputs("A new file has been created: ",clP);
    fputs(name, clP);
    fprintf(clP," lines: 0 - %s",ctime(&t));
    fputc('\n',clP);
    fclose(clP);
    
}

int CopyFile(){
    //declare two file pointer one to the original file another to the new copied file
    FILE *ogF;
    FILE *nwF;
    //take the new files name as input
    char NewFileName[col];
    do{
        clearMessageLine();
        clearInputLine();
        mvprintw(row -3,0,"Enter a unique name for your new copy (not empty string):");
        move(row -1,0);
        getstr(NewFileName);
    }while(access(NewFileName,F_OK) == 0);//if a file with this name exists request another name
    //open both files
    ogF = fopen(CurrentFname,"r");
    nwF = fopen(NewFileName,"w");
    //iterate through the original file and write each character to the new file
    char c;
    while(c!=EOF){
        c = fgetc(ogF);
        fputc(c,nwF);
    }
    //close both files
    fclose(nwF);
    fclose(ogF);

    //gets the date and time
    time_t t;
    time(&t);

    //opens change log and adds an entry containing what happened and when
    FILE *clP;
    clP = fopen("ChangeLog.txt","a");
    fputs("copu has been created of the file: ",clP);
    fputs(NewFileName, clP);
    fprintf(clP," lines: %d - %s",countLines(),ctime(&t));
    fputc('\n',clP);
    fclose(clP);
}

int DeleteFile(){
    //Confirm the user wants to delete this file
    clearMessageLine();
    mvprintw(row -3,0,"Are you sure you want to delete this file? enter y to delete");
    clearInputLine();
    char choice;
    choice = getch();
    //if the user chose to delete the file
    if(choice == 'y' || choice == 'Y'){
        //delete this file
        remove(CurrentFname);

        //gets the date and time
        time_t t;
        time(&t);

        //opens the change log and writes an entry containing what happened and when
        FILE *clP;
        clP = fopen("ChangeLog.txt","a");
        fputs("A file has been deleted: ",clP);
        fputs(CurrentFname, clP);
        fprintf(clP," - %s",ctime(&t));
        fputc('\n',clP);
        fclose(clP);
    }
    clearInputLine();
    clearMessageLine();
}
//function -- showCL
//Displays the contents of the changelog to the screen
int showCL(){
    
    mvprintw(0,(col/2)-strlen(CurrentFname),CurrentFname);//display the filename at the top of the screen

    //overwrite all characters on the screen
    for(int i = 0; i < col ; i++){
        for(int j = 2; j < row-4; j++){
            mvprintw(j,i," ");
        }
    }

    //declare a file pointer and point it to the current open file
    FILE *fp = fopen("ChangeLog.txt","r");
    
    //read through the file
    move(2,0);
    char currentC;
    int lineCounter = 1;
    while(currentC != EOF){
        currentC = fgetc(fp);
        //if the current character is after the linebuffer and will still fit into the window
        if(lineBuffCL < lineCounter && lineCounter < lineBuffCL + row -5){
            addch(currentC);//print the character to the screen
        }
        if(currentC == '\n'){//if we are on a newline
            lineCounter++;//increment the line counter
        }
    }
    //close the file
    fclose(fp);
    clearInputLine();
}

int menu(){
    //write the title to the top of the screen
    for(int i = 0;i< col ; i++){
        mvprintw(0,i," ");
    }
                mvprintw(0,(col/2)-strlen("editFile.c"),"editFile.c");
        //show the contents of either the file or changelog (depending on the value of seeFile)
        if(seeFile == 1){
            if(access(CurrentFname,F_OK) == 0){
                showFile();
            }else{
                //overwrite all characters on the screen
                for(int i = 0; i < col ; i++){
                    for(int j = 2; j < row-4; j++){
                        mvprintw(j,i," ");
                        }
                    }
                }
        }else{
            showCL();
        }

    //take a command as input
    clearMessageLine();
    mvprintw(row -3,0,"Enter A command");
    clearInputLine();
    char input[col];
    getstr(input);
    clearInputLine();
    
    if(strcmp(input,"quit") == 0){
        return 0;
    }

    //run the function that corresponds to the entered command

    if(strcmp(input,"open") == 0){
        OpenFile();
        if(strcmp(CurrentFname,"$$FILENAMEHASNOTBEENCHOSEN$$") == 0){//if CurrentFname set to rougue value
            return 0;//quit
        }
    }
    if(strcmp(input,"createfile") == 0){
        CreateFile();
    }
    if(strcmp(CurrentFname,"") == 0){//Makes sure the user opens a file before attempting to execute any commands
        OpenFile();
        if(strcmp(CurrentFname,"$$FILENAMEHASNOTBEENCHOSEN$$") == 0){//if CurrentFname set to rougue value
            return 0;//quit
        }
    }
    if(strcmp(input,"appendline") == 0){
        AppendLine();
    }
    if(strcmp(input,"insertline") == 0){
        insertLine();
    }
    if(strcmp(input,"deleteline") == 0){
        deleteLine();
    }
    if(strcmp(input,"copyfile") == 0){
        CopyFile();
    }
    if(strcmp(input,"deletefile") == 0){
        DeleteFile();
    }
    if(strcmp(input,"changelog") == 0){
        seeFile = 0;
    }
    if(strcmp(input,"file") == 0){
        seeFile = 1;
    }
    if(strcmp(input,"down") == 0){
        if(seeFile == 1){
            //adjusts the linebuff variable to view a different part of the screen
            lineBuff += row/2;
        }else{
            //adjusts the linebuff variable to view a different part of the screen
            lineBuffCL += row/2;
        }
        
    }
    if(strcmp(input,"up") == 0){
        if(seeFile == 1){
            //adjusts the linebuff variable to view a different part of the screen
            lineBuff -= row/2;
            if(lineBuff < 0){//if the user scrolls too far upp
                lineBuff = 0;//readjust the page
            }
        }else{
            //adjusts the linebuff variable to view a different part of the screen
            lineBuffCL -= row/2;
            if(lineBuffCL < 0){//if the user scrolls too far upp
                lineBuffCL = 0;//readjust the page
            }
        }
        
    }
    if(strcmp(input,"seeline") == 0){
        showALine();
    }
    if (strcmp(input,"linenums") == 0){
        showLnums();
    }
    if(strcmp(input,"countlines") == 0){
        printLineCount();
    }
    //Recall menu function
    menu();
}

int main(){
    //default to view the open files contents
    seeFile = 1;
    initscr();//start ncurses ui window
    getmaxyx(stdscr,row,col);//set row and col to the number of rows and columns respectively
    
    //write the characters of the gui to the screen
    mvprintw(0,(col/2)-strlen("editFile.c"),"editFile.c");
    for(int i = 0; i < col ; i++){
        mvprintw(1,i,"-");
    }
    for(int i = 0; i < col ; i++){
        mvprintw(row -2,i,"-");
        mvprintw(row -4,i,"-");
    }
    move(row -1,0);//move the cursor to take input
    
    //start the main programs control flow
    menu();

    refresh();
    endwin();//close the ui window
}