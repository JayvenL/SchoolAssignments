/********* main.c ********
    Student Name 	= Jayven Larsen
    Student Number	= 101260364
*/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a3_nodes.h"
#include "a3_functions.h"

int main()
{
    
    /* THIS CODE WILL LOAD THE DATABASE OF USERS FROM THE FILE
       AND GENERATE THE STARTING LINKED LIST.
    */
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
   
    

    int choice;
    int friend_choice;
    int number_post; 
    char username[16]; // holds password
    char password[15]; //holds username
    char friend_check[16]; //holds friends username for managing friends
    char post_friend_check [16]; // holds the friends username for posts
    char post_content[250]; //contains the post content
    user_t *username_manage = NULL; // is the user which is being managed by the program
    friend_t *check_friends = NULL; // pointer to loop through list of friends and check if friend is present
    bool valid_friend =  false; // repersented if the friend is in the friend list

    printf("\n*****************************************************\n");
    printf("            Welcome to Text-Based Facebook\n");
    printf("*****************************************************\n");

    while (1)
    {

        printf("\n*******************************\n");
        printf("        Main Menu:   ");
        printf("\n*******************************\n");
        printf("\n1. Register a new User");
        printf("\n2. Login with existing user's information ");
        printf("\n3. Exit ");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {

        case 1:
            printf("\nEnter user username: ");
            scanf("%s", username);
            user_t *check_existing_user = find_user(users,username);
            if (check_existing_user==NULL){ //checks if the username wanting to be added is already in the  list
            printf("\nEnter user password: ");
            scanf("%s", password);
            add_user(users, username, password); // adds new user
            printf("\n******** User Added ********\n");
            break;
            }else {
                printf("\n---------------------------------------");
                printf("\n  Username is already in the list");
                printf("\n---------------------------------------");
                break;

            }
        case 2: 

            printf("\nEnter user username: ");
            scanf("%s", username);
            printf("\nEnter user password: ");
            scanf("%s", password);
            username_manage = find_user(users, username); //checks user in the data base and assigns it to user_manage
            if (username_manage == NULL)
            {
                printf("\n-----------------------");
                printf("\n    User Not Found  ");
                printf("\n-----------------------");
            }
            else
            {

                if (strcmp(username_manage->password, password) != 0) // password checker
                {
                    printf("Incorrect password");
                }
                else if (strcmp(username_manage->password, password) == 0)
                {
                    break;
                }
            }

            break;
        case 3: // case 3 tearsdown the users and exits
            teardown(users);
            exit(1);
            break;
        default:
            printf("Incorrect option");
            break;
        }
        if (username_manage == NULL)
            continue;
        else
        {
            if (strcmp(username_manage->password, password) == 0)
            {
                break;
            }
        }
    }

    while (1)
    {
        printf("\n***********************************\n");
        printf("    Welcome %s  ", username);
        printf("\n***********************************\n");
        print_menu();

        scanf("%d", &choice);
        if (choice < 1 || choice > 6)
        {
            while (choice < 1 || choice > 6)
            {
                printf("This is not a valid option, Enter a new one: ");
                scanf("%d", &choice);
            }
        }

        switch (choice)
        {

        case 1: // case to change the users password
            while (1)
            {
                
                printf("\nEnter a new password that is up to 15 characters: ");
                scanf("%s", password);
                strcpy(username_manage->password, password);
                printf("\n****** Password changed! ******\n");
                break;
            }

            break;

        case 2: // MANAGER USE POSTS CASE

            if (username_manage->posts == NULL) 
            {
                printf("\n--------------------------------");
                printf("\n       %s's posts", username_manage->username);
                printf("\nNo posts available for %s", username_manage->username);
                printf("\n--------------------------------\n");
            }
            else
            {

                printf("\n--------------------------------");
                printf("\n       %s's posts", username_manage->username);
                display_user_posts(username_manage);
                printf("\n--------------------------------");
            }
            int post_choice;
            do
            {

                printf("\n1. Add new user post");
                printf("\n2. Remove a user post");
                printf("\n3. Return to main menu");
                printf("\n\nYour choice: ");
                scanf("%d", &post_choice);

                switch (post_choice) // choice for delete, add, return
                {
                case 1: //adds new post

                    printf("\nEnter your post content: ");
                    scanf(" %[^\n]s", post_content);
                    add_post(username_manage, post_content);
                    printf("\nPost has been added");
                    printf("\n-------------------------------------------");
                    printf("\n       %s's posts", username_manage->username);
                    display_user_posts(username_manage);
                    printf("-------------------------------------------");
                    break;

                case 2: // deletes selected post

                    printf("Which post do you want to delete: ");
                    scanf("%d", &number_post);
                    bool deleted_post = delete_post(username_manage, number_post);

                    if (deleted_post == false)
                    {
                        printf("\nInvalid post number");
                    }
                    display_user_posts(username_manage);
                    printf("-------------------------------------------");
                    break;

                default:
                    break;
                }

            } while (post_choice < 3);
            break;

        case 3: // users friends management

            do
            {
                printf("\n---------------------------------------------\n");
                printf("          %s's friends  ", username_manage->username);
                printf("\n---------------------------------------------\n");

                printf("\n1. Display all user's friends");
                printf("\n2. Add a new friend");
                printf("\n3. Delete a friend");
                printf("\n4. Display a friend's posts");
                printf("\n5. Return to main menu");
                printf("\n\nYour choice: ");
                scanf("%d", &friend_choice);

                switch (friend_choice) //swtich for adding , displaying, adding, deleting friends
                {
                case 1: 
                    printf("\nList of %s's friends:", username_manage->username);
                    if (username_manage->friends == NULL)
                    {
                        printf("\nNo friends available for %s", username_manage->username);
                    }
                    else
                    {
                        printf("List of %s's friends", username_manage->username);
                        display_user_friends(username_manage);
                    }
                    break;
                case 2:
                    printf("Enter a new friend's name: ");
                    scanf("%s", username);
                    user_t *friend_check = find_user(users, username);
                    if (friend_check == NULL)
                    {
                        printf("\n--------------------------\n");
                        printf("\n    User Not Found  ");
                        printf("\n--------------------------\n");
                        break;
                    }
                    add_friend(username_manage, username);
                    printf("Friend added to the list");

                    break;

                case 3:
                    if (username_manage->friends == NULL)
                    {
                        printf("\nNo friends available for %s", username_manage->username);
                    }
                    else
                    {
                        printf("\nList of %s's friends", username_manage->username);
                        display_user_friends(username_manage);
                        printf("\n\nEnter a friend's name to delete: ");
                        scanf("%s", username);
                        bool friend_deleted = delete_friend(username_manage, username);
                        if (friend_deleted == false)
                        {
                            printf("Invalid friend's name");
                            display_user_friends(username_manage);
                        }
                        else
                        {
                            display_user_friends(username_manage);
                        }
                    }

                case 4:
                    check_friends = username_manage->friends;
                    printf("Enter friend's name: ");
                    scanf("%s", post_friend_check);
                    while(check_friends !=NULL){ // goes through the friends to check if the friends name entered is a current friend
                        if (strcmp(check_friends->username,post_friend_check)==0){
                            user_t *temp = find_user(users,post_friend_check);
                            display_user_posts(temp); // displays the post of the friend 
                            valid_friend = true;
                        }
                        check_friends =check_friends->next;
                    }
                    if (valid_friend == false)
                    {
                        printf("\n--------------------------");
                        printf("\n    User Not in friends list  ");
                        printf("\n--------------------------\n");
                        //break;
                    }
                    //display_user_posts(friend_check_posts);
                    

                    break;

                default:
                    break;
                }

            } while (friend_choice < 5);

            break;

        case 4:

            display_all_posts(users);
            break;

        case 5: //case for exiting the program and tearing down the linked lists
            printf("\n********************************************");
            printf("\n   Thank you for using Text-Based Facebook");
            printf("\n                 Goodbye               ");
            printf("\n********************************************\n");
            teardown(users);
            exit(0);

            break;

        default:
            break;
        }
    }
}
