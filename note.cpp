/*
  ECE 3574 Project 1: Note.cpp
  Author: Charles Kelley
  Date: 01/27/2020
*/
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


/*------------------------------------------------------------------------------
  Class definitions
------------------------------------------------------------------------------*/

/* Class: NOTE

   Stores the content of a note, its priority, and if the note is complete.

   Members:

      contents -  content stored in note, the task to complete.
      priority -  the priority 1-10 given to a task.
      marked -    whether the task is complete.

   Returns:

      void.

*/
class NOTE
{
private:
  std::string contents;
  int priority;
  bool marked;

public:
  NOTE() { contents = ""; priority = 5; marked = false; }
  NOTE(std::string newCont, int newPrior, bool newMark) {contents = newCont; priority = newPrior; marked = newMark; }
  NOTE(const NOTE &p2) {contents = p2.contents; priority = p2.priority; marked = p2.marked; }

  bool isMarked()
  {
    return marked;
  }
  void mark()
  {
    marked = true;
  }

  int getPriority()
  {
    return priority;
  }
  void setPriority(int newPrior)
  {
    priority = newPrior;
  }

  std::string getContents()
  {
    return contents;
  }
  void setContents(std::string newCont)
  {
    contents = newCont;
  }

};

/*------------------------------------------------------------------------------
  Function definitions
------------------------------------------------------------------------------*/

/* Function: file_exists

   checks if a given file currently exsits.

   Parameters:

      fileName - given file name to check.

   Returns:

      A Boolean if the given file exsits.

*/
bool file_exists(std::string& fileName);

/* Function: create_text_file

   creates a empty text file with the given name.

   Parameters:

      name - name for new textfile.

   Returns:

      void.

*/
void create_text_file(std::string name);

/* Function: read_command_line

   reads all command line arguments and stores the needed information.

   Parameters:

      fileName - string to store a new file name
      priority - int passed by reference to store a new note priority
      contents - string used to store contents for the given command to process
      command - string to store command desired by user
      it - number of command line arguments
      arguments - command line arguments

   Returns:

      void.

*/
void read_command_line(std::string& fileName, int *priority,
  std::string& contents, std::string &command, int it, char **arguments);

/* Function: read_notes

   reads notes from a textfile into a vector for storage.

   Parameters:

      fileName - file name to pull notes from

   Returns:

      std::vector<NOTE> storage vector for all notes

*/
std::vector<NOTE> read_notes(std::string fileName);

/* Function: write_notes

   re-writes a vector of stored notes into a specified text file.

   Parameters:

      storage - vector of notes to store.
      fileName - specified file name to store notes

   Returns:

      void.

*/
void write_notes(std::vector<NOTE> storage, std::string fileName);

/* Function: print_list

   prints a given vector of notes to consel in a readable format.

   Parameters:

      fileNotes - vector of notes to print

   Returns:

      void.

*/
void print_list(std::vector<NOTE> fileNotes);

/*------------------------------------------------------------------------------
  Begin function implimentations
------------------------------------------------------------------------------*/

void printList(std::vector<NOTE> fileNotes)
{
  int lineCount = 1;
  for (NOTE temp : fileNotes)
  {
    std::cout << lineCount << ":[";
    if (temp.isMarked())
        std::cout << "x";
    else
        std::cout << " ";
    int tempP = temp.getPriority();
    std::string tempC = temp.getContents();
    std::cout << "][" << tempP << "] " << tempC << std::endl;
    lineCount++;
  }
}

std::vector<NOTE> read_notes(std::string fileName)
{
  int tempPrior;
  std::string tempCont;
  std::string tempMarked;

  std::ifstream notesFile;
  notesFile.open(fileName);

  std::vector<NOTE> returnVect;
  std::string tempLine;
  //parse text file and store each NOTE in array
  while (std::getline(notesFile, tempLine))
  {
    NOTE temp;
    tempMarked = tempLine.substr(0,1);
    tempPrior = stoi(tempLine.substr(2,3));
    tempCont = tempLine.substr(4, std::string::npos);
    if (tempMarked.find('x') != std::string::npos)
      temp.mark();
    temp.setContents(tempCont);
    temp.setPriority(tempPrior);
    returnVect.push_back(temp);
  }

  notesFile.close();

  return returnVect;
}

void write_notes(std::vector<NOTE> storage, std::string fileName)
{
  //open textfile
  std::ofstream editedNotesStorage;
  editedNotesStorage.open(fileName, std::ios::trunc);

  //write Notes from textfile
  int priority;
  bool markedNote;
  for (int i = 1; i <= 10; i++)
  {
    for (NOTE k : storage)
    {
      markedNote = k.isMarked();
      priority = k.getPriority();
      if (priority == i && !markedNote)
      {
        std::string tempPrintPrior = std::to_string(priority);
        editedNotesStorage << "o " << tempPrintPrior << " " << k.getContents() << std::endl;
      }
    }
  }
  for (NOTE l : storage)
  {
      if (l.isMarked())
      {
        std::string tempPrintPrior = std::to_string(l.getPriority());
        editedNotesStorage << "x " << tempPrintPrior << " " << l.getContents() << std::endl;
      }
  }
  editedNotesStorage.close();
}

void read_command_line(std::string &fileName, int *priority,
    std::string &contents, std::string &command, int it, char **arguments)
{
  bool commandRead = false;
  bool fileRead = false;
  bool priorityRead = false;
  bool printError = false;

	for (int i = 1; i < it; i++)
	{
		std::string temp = arguments[i];
		if (temp == "-f") // && !fileRead && !priorityRead && !commandRead)
    {
      if (!fileRead && !priorityRead && !commandRead)
      {
        i++;
        fileName = arguments[i];
        fileRead = true;
      }
      else
      {
        printError = true;
        i = it;
      }
    }
		else if (temp == "-p") // && !commandRead && !priorityRead)
    {
      if (!commandRead && !priorityRead)
      {
        i++;
        *priority = std::stoi(arguments[i]);
        priorityRead = true;
      }
      else
      {
        i = it;
        printError = true;
      }

    }
    else if (!commandRead)
    {
        command = temp;
        commandRead = true;
        int check = i + 1;
        if (check < it)
        {
          i++;
          contents = arguments[i];
        }
    }
    else
    {
      contents.append(" ");
      contents.append(temp);
    }
	}
  if (printError)
    std::cout << "ERROR: Invalid syntax - note [-f file] [-p priority] command" << std::endl;
}

bool file_exists(std::string& fileName)
{
    std::ifstream exists(fileName.c_str());
    return (bool)exists;
}

void createTextFile(std::string name)
{
    std::ofstream note(name);
    note << "0" << std::endl << "0" << std::endl;		//initalize line count
    note.close();
}

/*------------------------------------------------------------------------------
  Main function
------------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
  //define starting variables needed
  std::string fileName = "note.txt";
  int priority = 5;
	std::string command = "";
  std::string contents = "";
  std::vector<NOTE> fileNotes;

  //process command line arguments
	read_command_line(fileName, &priority, contents, command, argc, argv);

  // manage textfile if exsists or non-exisitent
  if (!file_exists(fileName))
	{
		createTextFile(fileName);
	}
  else
  {
    fileNotes = read_notes(fileName);
  }

  //process commands
  if (command == "add")
  {
    NOTE newNote;
    newNote.setContents(contents);
    newNote.setPriority(priority);
    fileNotes.push_back(newNote);
  }
  else if (command == "list")
  {
    if (contents.empty())
      printList(fileNotes);
    else
      std::cout << "ERROR: AN argument was included after the list command" << std::endl;
  }
  else if (command == "mark")
  {
    unsigned int tempLocation = stoi(contents) - 1;
    if (tempLocation < fileNotes.size())
    {
      NOTE tempMarkedNote;
      tempMarkedNote = fileNotes.at(tempLocation);
      tempMarkedNote.mark();
      fileNotes.erase(fileNotes.begin() + tempLocation);
      fileNotes.push_back(tempMarkedNote);
    }
  }
  else
  {
    std::cout << "ERROR: The command entered was invalid or included in an incorrect order." << std::endl;
  }

  write_notes(fileNotes, fileName);

	return EXIT_SUCCESS;
}
