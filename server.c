#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>    // for strlen

#define client_count 5

void game(int number_to_find, char *client_names[],int client_socket[]);
int find_random();

int main(int argc, char *argv[])
{
    struct sockaddr_in server, client;
    int socket_description, client_socket[client_count], c, number_client = 0;
    char *buffer_message, *client_names[client_count];


    socket_description = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_description == -1)
    {
        puts("Could not create socket");
        return 1;
    }

    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port        = htons(8888);

    if( bind(socket_description, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Error: Binding to port failed");
        return 1;
    }


    printf("\nThe game will be started, Waiting for %d players\n", client_count);

    listen(socket_description, client_count);


    c = sizeof(struct sockaddr_in);
    char *buffer_respond;
    buffer_respond = (char *)malloc(50);
    int i;

    /*
        @number_client = index for clients array.
     */
    while( client_socket[number_client] = accept(socket_description, (struct sockaddr*)&client, (socklen_t*)&c ) )
    {
        printf("Connection accepted\n");

        buffer_message = "Welcome to the number guessing game, please login to play \n\nEnter your name: ";
        send(client_socket[number_client], buffer_message, strlen(buffer_message), 0);

        bzero(buffer_respond, sizeof(buffer_respond));  //Erase buffer
        recv(client_socket[number_client], buffer_respond, sizeof(buffer_respond), 0);  //Take client's name.

        printf("%s logged in\n",buffer_respond );

        client_names[number_client] = (char *)malloc(50);
        strcpy(client_names[number_client], buffer_respond);    //Assign client's name.

        if(number_client == (client_count-1)){  //Start if client number is compelete.
            printf("\nThe game is starting\n\n");

            game(find_random(), client_names, client_socket);   //Calls method that includes Game functionality.

            /*Erases the ALL clients names and closes client sockets */
            for (i = 0; i < number_client; i++) {
                bzero(client_names[i], sizeof(client_names[i]));
                close(client_socket[i]);

            }
            printf("\nThe game will be started, Waiting for %d players\n\n", client_count);

            number_client = -1;
        }
        number_client++;
    }

    close(socket_description);

    return 0;
}

/*
    Main part of the code. Procces guess of the clients and compare with random generated number.
    If any client's correctly guess the number than 2. loop starts and tells to the clients that game
    is finished with sending a flag(flag_game) which holds if game is finished.

    @flag_game = Holds if game is finished or not. This var. is send to the client.


*/
void game(int number_to_find, char *client_names[], int client_socket[])
{
    int client_guess = 150, number_client = 0;
    int flag_which_is_winner, round = 0, i;
    int *flag_game = (int *)malloc(1);
    *flag_game = 0;
    char *buffer_message, *respond, *buffer_message_low, *buffer_message_up;
    char *buffer_message_win, *buffer_message_end;
    respond = (char *)malloc(20);


    buffer_message_up  = "Too high, try a smaller number\n";
    buffer_message_low = "Too low, try a bigger number\n";
    buffer_message_win = "Well done, your answer is correct\n";
    buffer_message_end = "\nThe game is over";

    while(client_guess != number_to_find)
    {

        if(number_client == 0){round++;printf("\n");}

        printf("Round %d, %s's turn \n",round,client_names[number_client]);

        send(client_socket[number_client], flag_game, 1,0);

        buffer_message = "It's your turn \nguess a number:";
        send(client_socket[number_client], buffer_message, strlen(buffer_message), 0);
        recv(client_socket[number_client], &client_guess, 3, 0);    //Takes client's guess


        printf("%s guessed: %d\n", client_names[number_client], client_guess);


        if(client_guess > number_to_find)
        {
            send(client_socket[number_client], buffer_message_up, strlen(buffer_message_up), 0);

        }
        else if(client_guess < number_to_find)
        {
            send(client_socket[number_client], buffer_message_low, strlen(buffer_message_low),0);

        }

        bzero(respond, sizeof(respond));
        flag_which_is_winner = number_client;
        number_client = (number_client+1)%client_count;


    }

    *flag_game = 1;

    for ( i = 0; i < client_count; i++)
    {

        if(flag_which_is_winner == i){
            printf("\n%s won the game\n\n",client_names[i]);
            send(client_socket[i], buffer_message_win, strlen(buffer_message_win), 0);  //Tells client that it won.
            usleep(500);
        }

        send(client_socket[i], flag_game, 1,0); //Tells client to break it's loop. Flag for telling game is finished.
        usleep(500);
        send(client_socket[i], buffer_message_end, strlen(buffer_message_end), 0);  //Tells client that game is over.
    }

}
/* Find random number by time seed. */
int find_random()
{

    int number_to_find;
    time_t t;

    srand((unsigned) time(&t));

    number_to_find = rand()%100+1;

    return number_to_find;
}
