#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>    // for strlen

#define port_no 8888
int check_input_for_int(char *buffer);

int main(int argc, char *argv[])
{
    int socket_description;
    struct sockaddr_in server;

    socket_description = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_description == -1)
    {
        puts("Error: Couldn't create socket for client");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port_no);

    if( connect(socket_description, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Error: Couldn't connect to server");
        return 1;
    }

    char* buffer_name  = (char *)malloc(100);
    //char* buffer_guess = (char *)malloc(10);
    char* buffer_guess = (char *)malloc(3);

    char server_reply[2000];

    if( recv(socket_description, server_reply, 2000, 0) < 0 )
    {
        puts("Recv failed");
        return 1;
    }
    puts(server_reply);
    /*-------------------Name call and send------*/

    scanf("%s", buffer_name );
    puts("Name is taken");

    if( send(socket_description, buffer_name, strlen(buffer_name), 0) < 0) puts("Error: Sending name failed");

    puts("Name send success");

    /*-----------------Guessing-------------*/
    bzero(&server_reply,2000);


    int *flag_game = (int *)malloc(1);
    int *guess = (int *)malloc(2);

    while(1)
    {
        recv(socket_description, flag_game, 1,0);
        if(*flag_game == 1){
            printf("Finish? Exiting Loop\n");
            break;
        }

        recv(socket_description, server_reply, 34, 0); //---------------recv

        puts(server_reply);

        //scanf("%s", buffer_guess ); strlen(buffer_guess)


        *guess = check_input_for_int(buffer_guess);

        //printf("Client random input: %d\n", *guess);

        if( send(socket_description, guess, 2, 0) < 0) puts("Error: Sending guess failed");

        bzero(&server_reply,2000);
        recv(socket_description, server_reply, 34, 0); //---------------recv
        puts(server_reply);
    }

    bzero(&server_reply,2000);
    recv(socket_description, server_reply, 34, 0); //---------------recv
    puts(server_reply);
    /*------------------Guessing Ends-----------*/


    puts("The End of client");
    close(socket_description);


}

/*
    This function takes user input and then check whether it is integer and
    it's a max 2 digit number

    @buffer = holds user input
    @other_part_of_string = for stroll, holds cutted string from the buffer
    @guess = int variable that holds converted number from user.

 */
int check_input_for_int(char *buffer)
{
    char *other_part_of_string;
    int guess;


    while(1)
    {

        //fgets(buffer, 3, stdin);
        scanf("%s",buffer );    //takes user input

        //printf("Found string: %s\n", buffer);

        if((guess = strtol(buffer, &other_part_of_string, 10)) != 0){   //converts str input to the int.

            //printf("Found random input: %d\n",guess);
            //printf("other part is %s\n",other_part_of_string );

            if(*other_part_of_string != '\0'){  //this is for if there is additional char character in the input after number.
                printf("Please enter only digits!\n");
            }else {
                if( guess > 100 || guess < 0){//check if it below 100 or above 0
                    printf("Please enter max. 2 digit number or min val. = 11\n");
                }else return guess;
            }

        }

    }

}
