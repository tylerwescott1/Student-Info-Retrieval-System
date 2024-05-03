/*
HEADER COMMENT 
Full name: Tyler Wescott
NSHE: 5006959108
Goal of the program/output: 
Goal of main is to read data from NSHE.txt and populate hash maps using this data, then prompt user for
names and NSHE ids of students to search and display proper error message or correct student data

Goal of hashMap.cpp is to create constructor, destructor, copyHashMap, deallocateHashMap, operator[], operator=, and
resize functions by following steps outlined in the PDF provided. 
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include "hashMap.h"
#include "hashMap.cpp"

using namespace std;

struct studentRecord 
{
    string first, last, id;
    double gpa, credits;
    string university;
};


//You will need to read from a txt file with a set of lines where each line contains
//first last id gpa credits university
int main()
{
    hashMap<string, studentRecord> nshe;
    //maps a student’s first name, last name, and student id (concatenated into one string) to the student’s record.
    //For example if a student is called John Doe with id 1234567, you use nshe["John"+ "Doe"+ "1234567"] as the key
    //to look up a student record

    hashMap<string, bool> studentExists;
    //maps a student’s first and last name (concatenated into one string) to a boolean that determines if this student
    //exists or not, for example if a student has a name Jane Roe, then you use studentExists["Jane"+ "Roe"] as the
    //key to look up if the student exists in the NSHE system

    hashMap<string, int> failedSearches;
    //maps an existing student first and last name to a counter that denotes the amount of failed student id searches
    //on the student

    ifstream readFile("NSHE.txt");
    string fileReadString, firstLastId, firstandLast;


    while (getline(readFile, fileReadString))
    {
        //get and parse the line
        stringstream ss(fileReadString);
        studentRecord studentRecObj;

        //read into struct
        if (ss >> studentRecObj.first >> studentRecObj.last >> studentRecObj.id >> studentRecObj.gpa >> studentRecObj.credits >> studentRecObj.university)
        {
            firstLastId = studentRecObj.first + studentRecObj.last + studentRecObj.id;
            nshe[firstLastId] = studentRecObj;      //populate nshe hash map

            firstandLast = studentRecObj.first + studentRecObj.last;
            studentExists[firstandLast] = 1;        //populate studentExists hash map with bool
        }
        else { break; }     //if string stream doesn't work for some reason
    }

    /*
    Once you populate the nshe and studentExists map, you implement the following algorithm:
    1. Prompt the user for a student’s full name and student id. If the student’s name does not exist, output student does not exist, and go to the menu portion (step 5)
    2. If the student exists, but the amount of times this student was searched with their nshe id is 3 or larger, output too many failed nshe searches on this student and go to the menu portion (step 5)
    3. If the student exists, search for the student’s name concatenated with their student id, if record is not found, output NSHE ID does not match student’s record, updated the failed searches amount for this student and go to the menu portion (step 5)
    4. If the student record could be found, output the record and go to the menu portion (step 5)
    5. Prompt the user if they wish to continue, if yes go to step 1 otherwise end the program
    */

    bool continueBool = 1;      //if continue game is true ('y')
    while (continueBool == 1)
    {
        string studentFirstName, studentLastName, studentNSHEid, studentFirstAndLast, studentFirstLastNSHE;
        int failedSearchCount = 0;
        char continueChar;

        //Prompt the user for a student’s full name and student id.
        cout << "Enter student's full name and nshe id: ";

        if (cin >> studentFirstName >> studentLastName >> studentNSHEid)
        {
            //cout name and id of student for code grade auto check
            cout << studentFirstName << " " << studentLastName << " " << studentNSHEid << endl << endl;

            studentFirstAndLast = studentFirstName + studentLastName;
            studentFirstLastNSHE = studentFirstAndLast + studentNSHEid;

            //If the student’s name does not exist, output student does not exist
            if (studentExists[studentFirstAndLast] == 0)    //if the value in studentExists is returned as false
                cout << "Student does not exist" << endl << endl;

            //If the student exists
            if (studentExists[studentFirstAndLast] == 1)
            {
                failedSearchCount = failedSearches[studentFirstAndLast];    //get number of failed searches
                //but # of times this student was searched with their nshe id >= 3, output too many failed nshe searches
                if (failedSearchCount >= 3)
                    cout << "Too many failed nshe searches on this student" << endl << endl;

                if (failedSearchCount < 3)
                {
                    studentRecord studentFound;
                    studentFound = nshe[studentFirstLastNSHE];  //save the found student data into a studentRecord obj
                    
                    //search for the student’s name concatenated with their student id
                    //if record is not found, output NSHE ID does not match student’s record
                    if (studentFound.id != studentNSHEid)       //if the provided nshe doesn't match/provided incorrectly
                    {
                        cout << "NSHE ID does not match student's record" << endl << endl;
                        //updated the failed searches amount for this student
                        failedSearches[studentFirstAndLast] += 1;
                    }
                    //If the student record could be found, output the record
                    if (studentFound.id == studentNSHEid)       //otherwise, all info provided correctly and student exists so print info
                    {
                        cout << studentFound.last << ", " << studentFound.first << endl;
                        cout << "Credits: " << studentFound.credits << " GPA: " << studentFound.gpa << endl;
                        cout << studentFound.university << endl << endl;
                    }
                }
            }
        }  

        //Prompt the user if they wish to continue, if yes go to step 1 otherwise end the program
        cout << "Do you wish to continue? (Y/N): ";
        cin >> continueChar;
        cout << continueChar << endl << endl;   //also cout this for code grade auto check

        if (continueChar == 'y')
            continueBool = 1;
        if (continueChar == 'n')
            continueBool = 0;
    }
    
    return 0;
}
