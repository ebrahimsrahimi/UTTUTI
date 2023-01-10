#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define FIRSTPOST 1
#define NAMEOFACCOUNTFILE "accounts.txt"
#define NAMEOFPOSTS "posts.txt"
#define TRUE 1
#define ONE 1
#define TWO 2
#define NULLCHAR '\0'

typedef struct POST post;
typedef struct USER user;
typedef struct LIKERS liker;

struct POST
{
    char *userName;
    int postID;
    int like;
    char *text;
    post *nextPost;
    liker *likersHead;
};

struct USER
{
    char *userName;
    char *password;
    post *postsHead;
    user *nextUser;
    int numberOfPosts;
    int totalNumOfPosts;
};

struct LIKERS
{
    char *userName;
    liker *nextLiker;
};

user *newUser();

liker *newLiker();

void signup(user **headptr, user *new_user);

void login(user *head, char **loggedUser);

post *newPost(int postID, char *userName);

void funcPost(user *head, char *loggedUser);

void printPosts(post *postsHead);

void info(user *head, char *loggedUser);

void likePost(user *head, char *likeduser, int postID, char *loggedUser);

void find_user(user *head, char *userNameToFind);

void deletePost(user *head, char *loggedUser, int postID);

char *enter();

char *enterText();

int main()
{
    user *headOfUsers = NULL; /*initialize variables*/
    char *tempString = NULL;
    char *tempString2 = NULL;
    int tempInt = 0;
    char *loggedUser = NULL;
    char *hold = NULL;
    FILE *accounts = NULL;
    FILE *posts = NULL;
    user *currUser = NULL;
    post *currPost = NULL;
    printf("WELCOME TO UTTUTI\n");
    do
    {
        if (!loggedUser) /*if no one was login ask user to signup or login*/
        {
            printf("PLEASE LOGIN(DON'T HAVE AN ACCOUNT! SIGNUP FIRST)\n");
        }
        tempString = enter();
        /*determine command*/
        if (!strcmp("signup", tempString))
        {
            if (loggedUser)
            {
                printf("Please logout first\n");
            }
            else
            {
                signup(&headOfUsers, newUser());
            }
            fflush(stdin);
        }
        else if (!strcmp("login", tempString))
        {
            if (loggedUser)
            {
                printf("PLEASE LOGOUT FIRST!\n");
            }
            else
            {
                login(headOfUsers, &loggedUser);
            }
            fflush(stdin);
        }
        else if (!strcmp("post", tempString))
        {
            if (!loggedUser)
            {
                printf("PLEASE LOGIN FIRST!\n");
            }
            else
            {
                funcPost(headOfUsers, loggedUser);
            }
            fflush(stdin);
        }
        else if (!strcmp("info", tempString))
        {
            if (!loggedUser)
            {
                printf("PLEASE LOGIN FIRST!\n");
            }
            else
            {
                info(headOfUsers, loggedUser);
            }
            fflush(stdin);
        }
        else if (!strcmp("like", tempString))
        {
            if (!loggedUser)
            {
                printf("PLEASE LOGIN FIRST!\n");
            }
            else
            {
                tempString = enter();
                if (scanf("%d", &tempInt))
                {
                    likePost(headOfUsers, tempString, tempInt, loggedUser);
                }
                else
                {
                    printf("Please enter correct command\n");
                }
            }
            fflush(stdin);
        }
        else if (!strcmp("logout", tempString))
        {
            if (!loggedUser)
            {
                printf("PLEASE LOGIN FIRST THERE IS NO ACCOUNT FOR LOGOUT!\n");
            }
            else
            {
                hold = loggedUser;
                loggedUser = NULL;
                free(hold);
                printf("LOGOUT SECCESFULLY!\n");
            }
            fflush(stdin);
        }
        else if (!strcmp("find_user", tempString))
        {
            if (!loggedUser)
            {
                printf("PLEASE LOGIN FIRST!\n");
            }
            else
            {
                tempString = enter();
                find_user(headOfUsers, tempString);
            }
            fflush(stdin);
        }
        else if (!strcmp("EXIT", tempString))
        {
            printf("SEE YOU SOON!\n");
            return 0;
        }
        else if (!strcmp("delete", tempString))
        {
            if (!loggedUser)
            {
                printf("PLEASE LOGIN FIRST!\n");
            }
            else
            {
                if (scanf("%d", &tempInt))
                {
                    deletePost(headOfUsers, loggedUser, tempInt);
                }
                else
                {
                    printf("Please enter correct command\n");
                }
            }
            fflush(stdin);
        }
        else
        {
            printf("Please enter correct command\n");
            fflush(stdin);
        }
        accounts = fopen(NAMEOFACCOUNTFILE, "w"); /*write data in files*/
        posts = fopen(NAMEOFPOSTS, "w");
        currUser = headOfUsers;
        while (currUser)
        {
            fputs(currUser->userName, accounts);
            fputs(" ", accounts);
            fputs(currUser->password, accounts);
            fputs(" ", accounts);
            fprintf(accounts, "%d", currUser->totalNumOfPosts);
            fputs("\n", accounts);
            currPost = currUser->postsHead;
            while (currPost)
            {
                fputs(currPost->text, posts);
                fputs(" ", posts);
                fputs(currPost->userName, posts);
                fputs(" ", posts);
                fprintf(posts, "%d", currPost->like);
                fputs("\n", posts);
                currPost = currPost->nextPost;
            }
            currUser = currUser->nextUser;
        }
        fclose(accounts);
        fclose(posts);
    } while (TRUE);
    return 0;
}

user *newUser() /*prepare a new user*/
{
    user *new_user = (user *)malloc(sizeof(user));
    char *tempString;
    tempString = enter();
    new_user->userName = (char *)malloc(sizeof(char) * strlen(tempString));
    strcpy(new_user->userName, tempString);
    tempString = enter();
    new_user->password = (char *)malloc(sizeof(char) * strlen(tempString));
    strcpy(new_user->password, tempString);
    new_user->postsHead = NULL;
    new_user->nextUser = NULL;
    new_user->numberOfPosts = 0;
    new_user->totalNumOfPosts = 0;
    return new_user;
}

void signup(user **headptr, user *new_user)
{
    if (*headptr == NULL)
    {
        (*headptr) = new_user;
        return;
    }
    user *curr = *headptr;
    user *pre = NULL;
    while (curr) /*check the username*/
    {
        if (!strcmp(curr->userName, new_user->userName))
        {
            printf("THIS USERNAME ALREADY USE !\nPLEASE TRY AGAIN ANOTHER USERNAME\n");
            return;
        }
        pre = curr;
        curr = curr->nextUser;
    }
    pre->nextUser = new_user;
}

void login(user *head, char **loggedUser)
{
    user *curr = head;
    char *tempString;
    tempString = enter();
    while (curr != NULL)
    {
        if (!strcmp(curr->userName, tempString))
        {
            tempString = enter();
            if (!strcmp(curr->password, tempString)) /*check password*/
            {
                *loggedUser = malloc(sizeof(char) * strlen(tempString));
                strcpy(*loggedUser, curr->userName);
                return;
            }
            else
            {
                printf("Invalid Password ! \nPlease try again\n");
                return;
            }
        }
        curr = curr->nextUser;
    }
    printf("USER NOT FOUND!\n");
}

post *newPost(int postID, char *userName) /*prepare new post*/
{
    post *new_Post = (post *)malloc(sizeof(post));
    char *tempString = enterText();
    new_Post->text = malloc(sizeof(char) * strlen(tempString));
    strcpy(new_Post->text, tempString);
    new_Post->like = 0;
    new_Post->postID = postID;
    new_Post->userName = malloc(sizeof(char) * strlen(userName));
    strcpy(new_Post->userName, userName);
    new_Post->nextPost = NULL;
    new_Post->likersHead = NULL;
    return new_Post;
}

void funcPost(user *head, char *loggedUser) /*organaiz posts*/
{
    user *currUser = head;
    while (currUser != NULL)
    {
        if (!strcmp(currUser->userName, loggedUser))
        {
            post *currPost = currUser->postsHead;
            if (currUser->postsHead == NULL)
            {
                currUser->postsHead = newPost(FIRSTPOST, currUser->userName);
                currUser->numberOfPosts++;
                currUser->totalNumOfPosts++;
                return;
            }
            while (currPost != NULL)
            {
                if (currPost->nextPost == NULL)
                {
                    currPost->nextPost = newPost(++currUser->numberOfPosts, currUser->userName);
                    currUser->totalNumOfPosts++;
                    return;
                }
                currPost = currPost->nextPost;
            }
        }
        currUser = currUser->nextUser;
    }
    printf("USER NOT FOUND!\n");
}

void printPosts(post *postsHead)
{
    post *currPost = postsHead;
    while (currPost != NULL)
    {
        printf("user: %s\n", currPost->userName);
        printf("post_id: %d\n", currPost->postID);
        printf("like: %d\n", currPost->like);
        printf("post: %s\n", currPost->text);
        currPost = currPost->nextPost;
    }
}

void info(user *head, char *loggedUser)
{
    if (!loggedUser)
    {
        printf("Please login first!");
        return;
    }
    user *currUser = head;
    while (currUser)
    {
        if (!strcmp(currUser->userName, loggedUser))
        {
            printf("Username : %s\n", currUser->userName);
            printf("Password :%s\n", currUser->password);
            printPosts(currUser->postsHead);
            return;
        }
        currUser = currUser->nextUser;
    }
    printf("USER NOT FOUND!\n");
}

void likePost(user *head, char *likeduser, int postID, char *loggedUser)
{
    user *currUser = head;
    while (currUser)
    {
        if (!strcmp(currUser->userName, likeduser))
        {
            post *currPost = currUser->postsHead;
            while (currPost != NULL)
            {
                if (currPost->postID == postID)
                {
                    if (!currPost->likersHead)
                    {
                        currPost->like++;
                        currPost->likersHead = newLiker(loggedUser);
                        return;
                    }
                    liker *currLiker = currPost->likersHead;
                    while (currLiker)
                    {
                        if (!strcmp(currLiker->userName, loggedUser))
                        {
                            printf("YOU HAVE BEEN LIKE THIS POST!\n");
                            return;
                        }
                        currLiker = currLiker->nextLiker;
                    }
                    currLiker = currPost->likersHead;
                    while (currLiker->nextLiker)
                    {
                        currLiker = currLiker->nextLiker;
                    }
                    currLiker->nextLiker = newLiker(loggedUser);
                    currPost->like++;
                    return;
                }
                currPost = currPost->nextPost;
            }
            printf("POST NOT FOUND\n");
            return;
        }
        currUser = currUser->nextUser;
    }
    printf("USER NOT FOUND!\n");
}

void find_user(user *head, char *userNameToFind)
{
    user *curr = head;
    while (curr)
    {
        if (!strcmp(curr->userName, userNameToFind))
        {
            printPosts(curr->postsHead);
            return;
        }
        curr = curr->nextUser;
    }
    printf("USER NOT FOUND!\n");
}

liker *newLiker(char *loggedUser) /*prepare new Liker*/
{
    liker *newLiker = (liker *)malloc(sizeof(liker));
    newLiker->userName = (char *)malloc(sizeof(char) * strlen(loggedUser));
    strcpy(newLiker->userName, loggedUser);
    newLiker->nextLiker = NULL;
}

char *enter() /*get input from stdin*/
{
    char *tempstring = (char *)malloc(sizeof(char));
    char tempchar;
    int counter = ONE;
    while ((tempchar = getchar()) != ' ' && tempchar != '\n')
    {
        counter++;
        tempstring = realloc(tempstring, sizeof(char) * counter);
        *(tempstring + counter - TWO) = tempchar;
    }
    *(tempstring + counter - ONE) = NULLCHAR;
    return tempstring;
}

char *enterText() /*get post's text from stdin*/
{
    char *tempstring = (char *)malloc(sizeof(char));
    char tempchar;
    int counter = ONE;
    while ((tempchar = getchar()) != '\n')
    {
        counter++;
        tempstring = realloc(tempstring, sizeof(char) * counter);
        *(tempstring + counter - TWO) = tempchar;
    }
    *(tempstring + counter - ONE) = NULLCHAR;
    return tempstring;
}

void deletePost(user *head, char *loggedUser, int postID)
{
    user *currUser = head;
    post *hold = NULL;
    post *currPost = NULL;
    post *prePost = NULL;
    while (currUser)
    {
        if (!strcmp(currUser->userName, loggedUser))
        {
            currPost = currUser->postsHead;
            while (currPost)
            {
                if (currUser->postsHead->postID == postID)
                {
                    hold = currPost;
                    if (!currPost->nextPost)
                    {
                        currPost = NULL;
                        free(hold);
                        currUser->totalNumOfPosts--;
                        return;
                    }
                    currUser->postsHead = currPost->nextPost;
                    free(hold);
                    currUser->totalNumOfPosts--;
                    return;
                }
                if (currPost->postID == postID)
                {
                    hold = currPost;
                    if (!currPost->nextPost)
                    {
                        prePost->nextPost = NULL;
                        free(hold);
                        currUser->totalNumOfPosts--;
                        return;
                    }
                    prePost->nextPost = currPost->nextPost;
                    free(hold);
                    currUser->totalNumOfPosts--;
                    return;
                }
                prePost = currPost;
                currPost = currPost->nextPost;
            }
            printf("THIS POST NOT EXIST!\n");
            return;
        }
        currUser = currUser->nextUser;
    }
}