/* lab1.c
 * Roderick "Rance" White
 * roderiw
 * ECE 2230 Fall 2020
 * MP1
 *
 * NOTE:  You must update all of the following comments!
 *
 * Purpose: Allows the user to enter alerts into a data list and manipulate it using
 * listed the commands that will be listed upon being run, such as entering another alert
 * into the list, deleting the list, finding and/or removing alerts with similar generator
 * ids and destination ips, determining groups of alerts with similar generator ids, and
 * printing the data list. These are done using functions in another file.
 *
 * Assumptions: Many details are incomplete.  The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * The program accepts one command line arguement that is the size of the list.
 *
 * An outline for the interactive menu input is provided.  Details need to be
 * completed but the format of the commands and the prints found in 
 * ids_record_fill ids_print_rec should not be changed.
 *
 * Bugs: Assume that information entered by the user for alert_t's will not contain errors
 *
 * See the ECE 2230 programming guide
 *
 * NOTE: if it forbidden to access any of the members in the ids_database
 * structure.   The member names MUST NOT be found in this file or it is a 
 * design violation.  Instead you must utilize the ids_ fuctions found 
 * in the ids.h header file to access any details of the list.
 *
 * One of the requirements is to verify you program does not have any 
 * memory leaks or other errors that can be detected by valgrind.  Run with
 * your test scripts:
 *      valgrind --leak-check=full ./lab1 < your_test_script
 * 
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up 
 * your own style.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ids.h"

int main(int argc, char **argv)
{
    struct ids_database *IdList;

    struct alert_t *new_rec;
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int num_items;
    int ip_add;
    int index;
    int list_size;
    int j;
    int entrynum;

    if (argc != 2) {
        printf("Usage: ./lab1 list_size\n");
        exit(1);
    }
    list_size = atoi(argv[1]);
    if (list_size < 1) {
        printf("lab1 has invalid list size: %d\n", list_size);
        exit(2);
    }

    //Creates the initial list
    IdList = ids_construct(list_size); 

    printf("Welcome to lab1. Using list size: %d\n", list_size);
    printf("ALERT\nLISTGEN x, LISTIP x\nDELGEN x, DELIP x\nSCANGEN x\nPRINT\nQUIT\n");

    // remember fgets includes newline \n unless line too long
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s%d%s", command, &ip_add, junk);
        if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            ids_destruct(IdList);
            /* found exit */
            printf("cleanup\n");
            break;


        /* If "ALERT" is entered, a new alert will be added to the database, in order of the alert's generator ID */
        } else if (num_items == 1 && strcmp(command, "ALERT") == 0) {
            new_rec = (struct alert_t *) malloc(sizeof(struct alert_t));                //Malloc memory for new_rec
            ids_record_fill(new_rec);                                                   //Fill new_rec with information
 
            // you have to figure out what goes here
            // and call the correct printf command
            int add_return = ids_add(IdList, new_rec);                                  //Adds new_rec to the id list

            if (add_return == 1) {
                printf("\nAdded: %d and doubled list size\n",new_rec->generator_id);
            } else if (add_return == 0) {
                printf("\nAdded: %d\n",new_rec->generator_id);
            } else {
                printf("\n\nError with ids_add return value\n");
                exit(1);
            }
            new_rec = NULL;






        /* If "LISTGEN" is entered, this section will find and print the information of all alerts with identical generator ids to the one entered */
        } else if (num_items == 2 && strcmp(command, "LISTGEN") == 0) {

            index=0;                                                                    //Reset index to 0
            entrynum=ids_count(IdList);                                                 //Number of entries in the list
            new_rec = ids_access(IdList, 0);                                            //Accesses alert so we can find information

            //If new_rec is initially empty, then it is null.
            if (new_rec == NULL) {
                printf("Did not find alert: %d\n", ip_add);

            } else {
                //Loops through each alert to check if its generator id matches the gen-id entered
                for(j=0; j < entrynum; j++)
                {
                    new_rec = ids_access(IdList, j);                                    //Accesses alert so we can find information

                    //Does the alert's generator id match the inputted gen-id
                    if(new_rec->generator_id == ip_add)
                    {
                        ids_print_rec(new_rec);                                         //Prints the alert that matches
                        index=index+1;                                                  //Increases index by one
                    }
                }

                // Then print a summary stating how many were found
                // index: how many alerts match ip_add
                printf("Found %d matching generator alerts %d\n", index, ip_add);

                //If no matches have been found and index is 0
                if(index==0)
                    printf("Did not find alert: %d\n", ip_add);
            }
            new_rec = NULL;





        /* If "LISTIP" is entered, this section will find and print the information of all alerts with identical destination IP addresses ids the one entered */
        } else if (num_items == 2 && strcmp(command, "LISTIP") == 0) {

            index=0;                                                                    //Reset index to 0
            entrynum=ids_count(IdList);                                                 //Number of entries in the list
            new_rec = ids_access(IdList, 0);                                            //Accesses alert so we can find information

            //If new_rec is initially empty, then it is null.
            if (new_rec == NULL) {
                printf("Did not find destination IP: %d\n", ip_add);

            } else {
                //Loops through each alert to check if its generator id matches the ip address entered
                for(j=0; j < entrynum; j++)
                {
                    new_rec = ids_access(IdList, j);                                    //Accesses alert so we can find information

                    //Does the alert's destination id match the inputted ip address
                    if(new_rec->dest_ip_addr == ip_add)
                    {
                        ids_print_rec(new_rec);                                         //Prints the alert that matches
                        index=index+1;                                                  //Increases index by one
                    }
                }

                // Then print a summary stating how many were found
                // index: how many alerts match ip_add
                printf("Found %d alerts matching IP %d\n", index, ip_add);

                //If no matches have been found and index is 0
                if(index==0)
                    printf("Did not find destination IP: %d\n", ip_add);
            }
            new_rec = NULL;





        /* If "DELGEN" is entered, this section will find and delete all alerts from the list if they have an identical generator id to the one entered */
        } else if (num_items == 2 && strcmp(command, "DELGEN") == 0) {

            index=0;                                                                    //Reset index to 0
            entrynum=ids_count(IdList);                                                 //Number of entries in the list
            new_rec = ids_access(IdList, 0);                                            //Accesses alert so we can find information

            //If new_rec is initially empty, then it is null.
            if (new_rec == NULL) {
                printf("Did not remove alert type: %d\n", ip_add);

            } else {
                //Loops through each alert to check if its generator id matches the gen-id entered
                for(j=1; j < entrynum+1; j++)                                               //j must start at one in case the first item is deleted, since decreasing j before 0 is impossible
                {
                    new_rec = ids_access(IdList, j-1);                                      //Accesses alert so we can find information, using j-1 to get the correct position 

                    //Does the alert's generator id match the inputted gen-id
                    if(new_rec->generator_id == ip_add)
                    {
                        index=index+1;                                                      //Increases index by one
                        struct alert_t *RemovedId = ids_remove(IdList, j-1);                //Declares a memeory block to hold the returning value and retrieves the matching Id and deletes it from IdList
                        free(RemovedId);                                                    //Frees the memory block
                        j=j-1;                                                              //Since the list has been shifted down by one, we must lower j to shift what is being checked to the previous position
                        entrynum=entrynum-1;                                                //Lower entrynum, since the list is now smaller
                    }
                }
                // Then print a summary stating how many were found
                // index: how many alerts match ip_add
                printf("Removed %d matching generator alerts %d\n", index, ip_add);

                //If no matches have been found and index is 0
                if(index==0)
                    printf("Did not remove alert type: %d\n", ip_add);
            }
            new_rec = NULL;








        /* If "DELGIP" is entered, this section will find and delete all alerts from the list if they have an identical destination ip address to the one entered */
        } else if (num_items == 2 && strcmp(command, "DELIP") == 0) {
          
            index=0;                                                                    //Reset index to 0
            entrynum=ids_count(IdList);                                                 //Number of entries in the list
            new_rec = ids_access(IdList, 0);                                            //Accesses alert so we can find information

            //If new_rec is initially empty, then it is null.
            if (new_rec == NULL) {
                printf("Did not remove any alert with IP: %d\n", ip_add);

            } else {
                //Loops through each alert to check if its generator id matches the ip address entered
                for(j=1; j < entrynum+1; j++)                                               //j must start at one in case the first item is deleted, since decreasing j before 0 is impossible
                {
                    new_rec = ids_access(IdList, j-1);                                      //Accesses alert so we can find information, using j-1 to get the correct position 

                    //Does the alert's destination id match the inputted ip address
                    if(new_rec->dest_ip_addr == ip_add)
                    {
                        index=index+1;
                        struct alert_t *RemovedId = ids_remove(IdList, j-1);                //Declares a memory block to hold the returning value and retrieves the matching Id and deletes it from IdList
                        free(RemovedId);                                                    //Frees the memory block
                        j=j-1;                                                              //Since the list has been shifted down by one, we must lower j to shift what is being checked to the previous position
                        entrynum=entrynum-1;                                                //Lower entrynum, since the list is now smaller

                    }
                }

                // Then print a summary stating how many were found
                // index: how many alerts match ip_add
                printf("Removed %d alerts matching IP %d\n", index, ip_add);

                //If no matches have been found and index is 0
                if(index==0)
                    printf("Did not remove any alert with IP: %d\n", ip_add);
            }
            new_rec = NULL;



        /* If "SCANGEN" is entered, this section will loop through the list and find alerts with identical generator ids and group if the number exceeds an entered threshold, listing the groups and how many */
        } else if (num_items == 2 && strcmp(command, "SCANGEN") == 0) {
            // for the SCANGEN command the second argument is the threshold
            index = 1;
            int groups = 0;
            int groupid = 0, previd = 0;
            entrynum=ids_count(IdList);                                                 //Number of entries in the list

            new_rec = ids_access(IdList, 0);                                            //Accesses alert so we can find information

            //If new_rec is initially empty, then it is null.
            if (new_rec == NULL) {
                printf("No records with >= %d matches\n", ip_add);
            }
            else
            {
                groupid = new_rec->generator_id;

                //Loops through each alert to check if its generator id matches the ip address entered
                for(j=1; j < entrynum; j++)                                                 //Skips 0 as alert at 0 has already been checked
                {
                    previd = groupid;                                                       //Makes the new previous id into the old group id
                    new_rec = ids_access(IdList,j);                                         //Retrieves info on the next alert
                    groupid = new_rec->generator_id;                                        //Retrieve group id from alert

                    //If the current id and the previous one are the same, count up the index
                    if(groupid == previd)                                                   //Since they're already organized by gen id, compare if the next id matches the previous one.
                    {
                        index = index + 1;
                    }
                    //If the counting has ended and the index exceeds the threshold entered, then a message is printed informing that a set has been made, the generator id, and the size of the group
                    else if(index >= ip_add)
                    {
                        // for each group that is found print the number of matches
                        num_items = previd;                                                 //Must put the previous id for the ground into num_items, as that was how the printf was formatted
                        printf("A set with generator %d has %d alerts\n", num_items, index);
                        groups = groups + 1;
                        index = 1;
                    }
                    else
                        index = 1;
                }

                // after all sets have been found print how many sets
                if (groups > 0) {
                    printf("Found %d sets\n", groups);
                } else {
                    printf("No records with >= %d matches\n", ip_add);
                }
            }



        /* If "PRINT" is entered, this section will print all the alerts in the alert database list */
        } else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            int num_in_list = ids_count(IdList);                                        //Sends IdList to ids_count to find the number of entries
            int array_size = ids_size(IdList);                                          //Sends IdList to ids_size to find the size of list
            if (num_in_list == 0) {
                printf("List empty. DB size is %d\n", array_size);
            } else {
                printf("List has %d records. DB size is %d\n", num_in_list, array_size);
                int i;
                for (i = 0; i < num_in_list; i++) {
                    printf("%d: ", i+1);
                    // you must use the ids_print_rec function to format output 
                    new_rec = ids_access(IdList, i);                                    //Accesses alert so we can find information
                    ids_print_rec(new_rec);
                }
            }
            new_rec = NULL;
        } else {
            printf("# %s", line);
        }
    }
    exit(0);
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
