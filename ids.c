/* ids.c
 * Roderick "Rance" White
 * ECE 2230 Fall 2020
 * MP1
 *
 * version 2.0
 *    Fix error in ids_record_fill.  Forgot to read in dest IP address
 *
 * Purpose: Holds various functions to be used by the main function in the lab1.c file.
 * These functions can create and destroy data lists, add and remove alerts from these
 * lists, return a specified alert to the main, and printing information.
 *
 *
 *
 *
 * Assumptions: Many details are incomplete.  The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * Bugs: 
 *
 * You must add comments for each function
 *
 * See the ECE 2230 programming guide
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ids.h"

/* Constructs the initial, empty id database */
struct ids_database *ids_construct(int size)
{
    
    /* In order to do this, I will have to allocate space for the struct*/
    struct ids_database *IDPtr;
    int i;

    //Allocate space for the new ids_database structure IDPtr
    IDPtr = (struct ids_database *) malloc(sizeof(struct ids_database));

    IDPtr->db_size = size;                                                                  //Put the size given by function into the IDPtr variable's db_size
    IDPtr->db_initsize = size;                                                              //Store the initial size given by function into the IDPtr variable's initsize
    IDPtr->db_entries = 0;                                                                  //The current number of entries in IDPtr is 0 as it is empty

    /*Space must be allocated for the array of alert_t pointers that alert_ptr will hold. */
    //Memory allocated is dependent on size of an alert_t structure pointer multiplied by the size.
    IDPtr->alert_ptr = (struct alert_t **) malloc(size * sizeof(struct alert_t *));

    //Loop to set each empty alert_t in IDPtr to NULL
    for(i=0; i<size; i++)
        IDPtr->alert_ptr[i] = NULL;

    return IDPtr;
}






/* Frees all of the alert_t memory blocks in the list, then the alert_t pointer, and finally the id database itself */ 
void ids_destruct(struct ids_database *list)
{
    int i;

    /* Loop to free all the alert_t memory blocks in the list and set them to NULL */
    for(i=0; i < list->db_entries; i++)
    {
        free(list->alert_ptr[i]);                                                           //Frees the information of the current alert_t pointer in the array
        list->alert_ptr[i] = NULL;                                                          //Set the current alert_t in the array to NULL
    }
    free(list->alert_ptr);                                                                  //Frees the entire array
    free(list);                                                                             //Frees the id database called list
}




/* Adds a new alert to the data structure */
int ids_add(struct ids_database *list, struct alert_t *rec_ptr)
{

    int i, IsDoubled=0;
    struct alert_t *PlaceholderPtr;

    /* Doubles the size of the array of alert_t pointers if the array is full */
    if(list->db_size <= list->db_entries)                                                               //If the list's array is full
    {
        list->db_size = list->db_size * 2;                                                              //Doubles the number in the db_size
        list->alert_ptr=realloc(list->alert_ptr, list->db_size * sizeof(struct alert_t *));             //Uses realloc to double the size of the array of alert_t pointers

        //Loop to set each new empty alert_t in the now larger list to NULL
        for(i=list->db_entries; i<list->db_size; i++)
        {
            list->alert_ptr[i] = NULL;
        }

        IsDoubled=1;
    }

    /* Loop to place rect_ptr in the list's alert_ptr array where its generator id is before a larger one. */
    for(i=0; i < list->db_entries; i++)                                                                 //Loop to count through list
    {

        if(rec_ptr->generator_id < list->alert_ptr[i]->generator_id)                                    //If the rect_ptr's generator id is less than the current alert_t's generator_id 
        {
            PlaceholderPtr = list->alert_ptr[i];                                                        //Placeholder to shift the array for space for rect_ptr
            list->alert_ptr[i] = rec_ptr;                                                               //Replaces current alert_t in alert_ptr with new rect_ptr
            rec_ptr = PlaceholderPtr;                                                                   //Puts placeholder ptr in rect_ptr so loop will continue to shift array
        }
    }

    list->db_entries = list->db_entries+1;                                                              //Increase the count of entries in list
    list->alert_ptr[i] = rec_ptr;                                                                       //Places current alert_t in rect_ptr at the end of the list's alert_ptr array

    return IsDoubled;
}









/* Returns a pointer to the alert_t at the index specified, if it is within the bounds of the list */ 
struct alert_t *ids_access(struct ids_database *list, int index)
{
    /* Return NULL if the index is out of bounds */
    if(list->db_entries < index+1)                                                          //If index would exceed number of entries
        return NULL;

    /* Return the alert_t memory at the index given. */
    return list->alert_ptr[index];
}









/* Returns a pointer to the alert_t at the index specified and removes it from the list, if it is within the bounds of the list */
struct alert_t *ids_remove(struct ids_database *list, int index)
{
    int i;
    struct alert_t *RemovedAlertT;


    /* Return NULL if the index is out of bounds */
    if(list->db_entries <= index)                                                                      //If index would exceed number of entries
        return NULL;

    RemovedAlertT=list->alert_ptr[index];                                                               //Places alert_t to be removed in separate structure

    list->db_entries=list->db_entries-1;                                                                //Lowers the number of entries

    /* Shift the array elements after the specified alert_t is removed */
    for(i=index; i < list->db_entries; i++)                                                           //Loops from the index given to the end
        list->alert_ptr[i] = list->alert_ptr[i+1];                                                      //Replace the current element with the element after

    /* The final element of the list must still be set to NULL */
    list->alert_ptr[i] = NULL; 

    /* Halves the size of the alert_ptr array if the number of entries is less than 20% of its size */
    if( (100 * list->db_entries)/list->db_size < 20)                                                    //If the number of entires is less than 20% of the size
    {
        if(list->db_size/2 >= list->db_initsize)                                                        //Continue if the result will not be less than the initial size from ids_construct
        {
            list->db_size = list->db_size/2;                                                            //Halves the number in the db_size
            list->alert_ptr=realloc(list->alert_ptr,list->db_size * sizeof(struct alert_t *));          //Using, the now halved db_size, realloc to half the memory allocated for the array of alert_t pointers
        }
    }
    return RemovedAlertT;
}






/* Indicates if the list is empty */
int ids_empty(struct ids_database *list)
{
    //If there are no entries in the list, that means it is empty
    if(list->db_entries == 0)
        return 1;

    //Otherwise return 0
    return 0;
}






/* This function returns the number of entries the list given is indicated to have. */
int ids_count(struct ids_database *list)
{
    return list->db_entries;
}


/* This function returns the size of the array in the database list. */
int ids_size(struct ids_database *list)
{
    return list->db_size;
}

/* Prompts user for alert record input starting with the source IP address.
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * Do not change the sequence of prompts as grading requires this exact
 * format
 */
void ids_record_fill(struct alert_t *rec)
{
    char line[MAXLINE];
    assert(rec != NULL);

    printf("Generator component:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->generator_id);
    printf("Signature:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->signature_id);
    printf("Revision:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->revision_id);
    printf("Dest IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->dest_ip_addr);
    printf("Source IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->src_ip_addr);
    printf("Destination port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->dest_port_num);
    printf("Source port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->src_port_num);
    printf("Time:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &rec->timestamp);
    printf("\n");
}

/* print the information for a particular alert record 
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * Do not change any of these lines and grading depends on this
 * format.
 */
void ids_print_rec(struct alert_t *rec)
{
    assert(rec != NULL);
    printf("[%d:%d:%d] (gen, sig, rev): ", rec->generator_id, rec->signature_id, 
            rec->revision_id);
    printf("Dest IP: %d, Src: %d, Dest port: %d,", rec->dest_ip_addr, 
            rec->src_ip_addr, rec->dest_port_num);
    printf(" Src: %d, Time: %d\n", rec->src_port_num, rec->timestamp); 
}


/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
