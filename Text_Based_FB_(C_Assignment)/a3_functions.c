/********* definitions.c ********
    Student Name 	= Jayven Larsen
    Student Number	= 101260364
*/

#include "a3_functions.h"




/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, const char *password)
{
   user_t *new_user = malloc(sizeof(user_t));
   assert(new_user != NULL);
   user_t *current = users;
   strcpy(new_user->username, username);
   strcpy(new_user->password, password);
   new_user->friends = NULL;
   new_user->posts = NULL;

   if (users == NULL || strcmp(new_user->username, users->username) < 0) // checks if the users linked list is empty
   {
      new_user->next = users;
      return new_user; // if empty it assigns the new user to the head and returns it
   }

   while (current->next != NULL && strcmp(new_user->username, current->next->username) > 0) //goes through the linked list in alphabetical order
   {
      current = current->next; // moving the current user in the linked-list.
   }
   new_user->next = current->next; //Push method
   current->next = new_user;
   

   return users;
}
/*
   Function that searches if the user is available in the database
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username)
{
   for (user_t *temp = users; temp != NULL; temp = temp->next) // loops through the user linked lists
   {
      assert(temp != NULL);
      // printf("\n%s",temp->username);
      if (strcmp(username, temp->username) == 0) //checks to see if the username is in the linked list
      {
         return temp;
      }
   }
   return NULL;
}

/*
   Function that creates a new friend's node.
   Return the newly created node.
*/
friend_t *create_friend(const char *username)
{
   friend_t *new_friend = malloc(sizeof(friend_t)); 
   assert(new_friend != NULL);

   strcpy(new_friend->username, username);

   return new_friend;
}

/*
   Function that links a friend to a user. The friend's name should be added into
   a sorted (ascending order) linked list.
*/
void add_friend(user_t *user, const char *friend)
{
   friend_t *new_friend = create_friend(friend);
   if (user->friends == NULL) //if the friend linked list is empty, it assigns the new friend to the head
   {
      user->friends = new_friend;
   }
   else
   {

      strcpy(new_friend->username, friend);
      new_friend->next = user->friends;
      user->friends = new_friend;
   }
   user->friends->posts = new_friend->posts; //links the users posts to new friends posts
}

/*
   Function that removes a friend from a user's friend list.
   Return true of the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name)
{
   bool check;
   friend_t *to_remove = user->friends;
   friend_t *temp = NULL;
   friend_t *name_check = user->friends;
   if (user->friends == NULL)
   {
      return false;
   }

   while (name_check != NULL)
   {
      check = (strcmp(to_remove->username, friend_name) == 0); // checks if the friend_name is in the users friend's list
      name_check = name_check->next; 
   }
   if (check == false)
      return false;
   else
   {

      for (; to_remove != NULL; temp = to_remove, to_remove = to_remove->next) // goes through the friend linked list
      {
         if (strcmp(to_remove->username, friend_name) == 0) // if the friend name matches in the list
         {
            if (temp != NULL) // checks position
            {
               temp->next = to_remove->next;
            }
            else
            {
               user->friends = to_remove->next;
            }
         }
      }
      free(to_remove);
      return true;
   }
}

/*
   Function that creates a new user's post.
   Return the newly created post.
*/
post_t *create_post(const char *text)
{
   post_t *new_post = malloc(sizeof(post_t));

   strcpy(new_post->content, text);

   return new_post;
}

/*
   Function that adds a post to a user's timeline. New posts should be added following
   the stack convention (LIFO) (i.e., to the beginning of the Posts linked list).
*/
void add_post(user_t *user, const char *text)
{

   post_t *new_post = create_post(text);
   if (user->posts == NULL)
   {
      user->posts = new_post;
   }
   else
   {

      strcpy(new_post->content, text);
      new_post->next = user->posts;
      user->posts = new_post;
   }
}

/*
   Function that removes a post from a user's list of posts.
   Return true if the post was deleted and false otherwise.
*/
_Bool delete_post(user_t *user, int number)
{
   post_t *temp = user->posts;
   if (user->posts == NULL)
   {
      return false;
   }

   if (number - 1 == 0) // if the first post if picked, it deletes the head and reassigns the rest
   {
      user->posts = temp->next;
      free(temp);
      return true;
   }

   int count = 1;

   while (temp != NULL && count < number - 1) // traverses the linked list until the number is hit
   {
      temp = temp->next;
      count++;
   }
   if (temp == NULL || temp->next == NULL) // checks for current and next temp position
   {
      return false;
   }
   post_t *temp2 = temp->next->next;

   free(temp->next);
   temp->next = temp2;
   printf("\nPost %d was Deleted\n", number);
   return true;
}

/*
   Function that  displays a specific user's posts
*/
void display_user_posts(user_t *user)
{
   int count = 1;
   for (post_t *temp = user->posts; temp != NULL; temp = temp->next, count++) // loops throught the posts list and prints each post
   {
      printf("\n%d- %s %s\n", count, user->username, temp->content);
   }
}

/*
   Function that displays a specific user's friends
*/
void display_user_friends(user_t *user)
{
   int count = 1;

   for (friend_t *temp = user->friends; temp != NULL; temp = temp->next, count++)
   {
      printf("\n%d- %s", count, temp->username);
   }
}
/*
   Function that displays all the posts of 2 users at a time from the database.
   After displaying 2 users' posts, it prompts if you want to display
   posts of the next 2 users.
   If there are no more post or the user types “n” or “N”, the function returns.
*/
void display_all_posts(user_t *users)
{

   int count = 0;
   char choice;

   while (users->posts != NULL)
   {

      display_user_posts(users);
      display_user_posts(users->next);
      printf("Do you want to display next 2 users posts? (Y/N): ");
      scanf(" %c", &choice);

      // count++;
      if (choice == 'N' || choice == 'n')
      {
         break;
      }
      if (choice == 'y' || choice == 'Y')
      {
         if (users->next == NULL || users->next->next == NULL)
         {
            printf("There are no more posts");
            break;
         }
         else
            users = users->next->next;
      }
   }
}

/*
   Fucntion that free all users from the database before quitting the application.
*/
void teardown(user_t *users)
{
   user_t *temp_users;
   post_t *temp_post;
   friend_t *temp_friend;

while (users->posts != NULL)
   {
      temp_post = users->posts;
      users->posts = users->posts->next;
      free(temp_post);
   }
while (users->friends != NULL)
   {
      temp_friend = users->friends;
      users->friends = users->friends->next;
      free(temp_friend);
   }

while (users != NULL)
   {
      temp_users = users;
      users = users->next;
      free(temp_users);
   }
}


/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_menu()
{

   printf("\n1. Manage a user's profile (change password)");
   printf("\n2. Manage a user's posts (display,add,remove)");
   printf("\n3. Manage a user's friends (display,add,remove)");
   printf("\n4. Display ALL posts");
   printf("\n5. Exit\n");

   printf("\nEnter choice: ");
}

/*
   Function that reads users from the text file.
  
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
   user_t *users = NULL;
   char buffer[500];
   fgets(buffer, sizeof(buffer), file); // Read and discard the header line
   int count = 0;
   for (int i = 0; i < num_users; i++)
   {
      fgets(buffer, sizeof(buffer), file);
      buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

      char *token = strtok(buffer, ",");
      char *token2 = strtok(NULL, ",");
      users = add_user(users, token, token2);
      char *username = token;

      token = strtok(NULL, ",");

      user_t *current_user = users;
      for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
         ;

      while (token != NULL && strcmp(token, ",") != 0 && count < 3)
      {
         if (strcmp(token, " ") != 0)
         {
            add_friend(current_user, token);
         }
         token = strtok(NULL, ",");
         count++;
      }
      count = 0;

      // token = strtok(NULL, ",");
      while (token != NULL && strcmp(token, ",") != 0)
      {
         add_post(current_user, token);
         token = strtok(NULL, ",");
      }
   }
   return users;
}
