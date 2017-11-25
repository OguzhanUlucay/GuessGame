#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>    // for strlen

#define client_count 2

void game(int number_to_find, char *client_names[],int client_socket[]);
int find_random();

int main(int argc, char *argv[])
{
    int socket_description, client_socket[client_count], c, number_client = 0;


    struct sockaddr_in server, client;
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


    //puts("Socket is binded");
    //int temp = client_count;
    printf("hey\n" );
    printf("The game will be started, Waiting for players\n");

    listen(socket_description, client_count);


    c = sizeof(struct sockaddr_in);
    char *buffer_respond;
    buffer_respond = (char *)malloc(50);
    int i;


    while( client_socket[number_client] = accept(socket_description, (struct sockaddr*)&client, (socklen_t*)&c ) )
    {
        printf("Connection accepted\n");

        buffer_message = "Welcome to the number guessing game, please login to play \n\nEnter your name";
        send(client_socket[number_client], buffer_message, strlen(buffer_message), 0);

        bzero(buffer_respond, sizeof(buffer_respond));
        recv(client_socket[number_client], buffer_respond, sizeof(buffer_respond), 0);

        printf("%s logged in\n",buffer_respond );
        client_names[number_client] = (char *)malloc(50);
        strcpy(client_names[number_client], buffer_respond);

        if(number_client == (client_count-1)){
            printf("\ngame is starting\n\n");

            game(find_random(), client_names, client_socket);

            for (i = 0; i < number_client; i++) {
                bzero(client_names[i], sizeof(client_names[i]));
                close(client_socket[i]);
                //printf("The game will be started, Waiting for %d players", temp);

            }
            number_client = -1;
        }
        number_client++;
        //printf("number_client: %d\n",number_client);
    }

    // if(new_socket < 0)
    // {
    //     puts("Error: Client acception failed");
    //     return 1;
    // }


    //puts("Connection accepted");

    close(socket_description);

    return 0;
}

void game(int number_to_find, char *client_names[], int client_socket[])
{
    int client_guess = 150, number_client = 0;
    int flag_which_is_winner, round = 0, i;
    int *flag_game = (int *)malloc(1);
    *flag_game = 0;
    char *buffer_message, *respond, *buffer_message_low, *buffer_message_up;
    char *buffer_message_win, *buffer_message_end;
    respond = (char *)malloc(20);

    //printf("Entered to game. The Random Number is: %d\n",number_to_find);

    buffer_message_up  = "Too high, try a smaller number\n";
    buffer_message_low = "Too low, try a bigger number\n";
    buffer_message_win = "Well done, your answer is correct\n";
    buffer_message_end = "The game is over";

    while(client_guess != number_to_find)
    {
        round++;
        //printf("We are in the loop captain\n");
        printf("Round %d, %s's turn \n",round,client_names[number_client]);

        send(client_socket[number_client], flag_game, 1,0);

        buffer_message = "It's your turn \nguess a number:";
        send(client_socket[number_client], buffer_message, strlen(buffer_message), 0);
        recv(client_socket[number_client], &client_guess, 3, 0);

        //client_guess = atoi(respond);

        //("String taken input: %s\n", respond);
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


    for ( i = 0; i < client_count; i++) {

        if(flag_which_is_winner == i){
            printf("%s won the game\n\n",client_names[i]);
            send(client_socket[i], buffer_message_win, strlen(buffer_message_win), 0);
            usleep(500);
        }

        send(client_socket[i], flag_game, 1,0);
        usleep(500);
        send(client_socket[i], buffer_message_end, strlen(buffer_message_end), 0);
    }

    printf("Exiting loop captain\n");


}

int find_random()
{

    int number_to_find;
    time_t t;

    srand((unsigned) time(&t));

    number_to_find = rand()%100+1;

    return number_to_find;
}
