#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>    /* Internet domain header */
#include <arpa/inet.h>     /* only needed on mac */

// change this value to customize the port per student (step 2)
#define PORT 54321
#define LEGO_PIECES 10
#define MAX_BUF 128
#define MAX_QUEUE 2

/*
 * Write msg to players with socket descriptors player1 and player2
 */
void write_to_players(char *msg, int player1, int player2) {
    // you must complete this function
    if (player1 != -1) {
        write(player1, msg, strlen(msg));
    }
    if (player2 != -1) {
        write(player2, msg, strlen(msg));
    }
}

/* Read and return a valid move from socket.
 * A valid move is a text integer between 1 and 3 followed by a \r\n.
 *
 * Hint: read from the socket into a buffer, loop over the buffer
 *   until you find \r\n and then replace the \r with \0 to make a
 *   string. Then use strtol to convert to an integer. If the result
 *   isn't in range, write a message to the socket and repeat.
 */
int read_a_move(int socket) {
    // prompt player for a move
    write(socket, "Enter a move (1-3): ", 20);

    char buffer[MAX_BUF];
    buffer[0] = '\0';
    int num_chars = read(socket, buffer, MAX_BUF - 1);
    while(strstr(buffer, "\r\n") == NULL) {
        num_chars += read(socket, &buffer[num_chars], MAX_BUF - 1 - num_chars);
        buffer[num_chars] = '\0';

        if (num_chars == 0) {
            fprintf(stderr, "Client disconnected.\n");
            return -1;
        }
        if (num_chars == -1) {
            perror("read");
            return -1;
        }
        if (num_chars >= MAX_BUF - 1) {
            fprintf(stderr, "Client sent too much data, buffer too full, exiting...\n");
            return -1;
        }
    }

    char *endptr;
    int move = strtol(buffer, &endptr, 10);
    if (move < 1 || move > 3) {
        write(socket, "Invalid move, try again.\r\n", 26);
        return read_a_move(socket);
    }
    return move;
}


int main() {
    // create socket
    int listen_soc = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_soc == -1) {
        perror("server: socket");
        exit(1);
    }

    // initialize server address
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    memset(&server.sin_zero, 0, 8);
    server.sin_addr.s_addr = INADDR_ANY;

    printf("Server is listening on %d\n", PORT);

    // This sets an option on the socket so that its port can be reused right
    // away. Since you are likely to run, stop, edit, compile and rerun your
    // server fairly quickly, this will mean you can reuse the same port.
    int on = 1;
    int status = setsockopt(listen_soc, SOL_SOCKET, SO_REUSEADDR,
                            (const char *) &on, sizeof(on));
    if (status == -1) {
        perror("setsockopt -- REUSEADDR");
    }

    // Bind socket to an address
    if (bind(listen_soc, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
      perror("server: bind");
      close(listen_soc);
      exit(1);
    }

    // Set up a queue in the kernel to hold pending connections.
    if (listen(listen_soc, MAX_QUEUE) < 0) {
        // listen failed
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    unsigned int client_len = sizeof(struct sockaddr_in);
    client_addr.sin_family = AF_INET;

    int fd_p1 = accept(listen_soc, (struct sockaddr *)&client_addr, &client_len);
    if (fd_p1 == -1) {
        perror("accept");
        return -1;
    }
    write_to_players("You are player 1, waiting for player 2...", fd_p1, -1);

    int fd_p2 = accept(listen_soc, (struct sockaddr *)&client_addr, &client_len);
    if (fd_p2 == -1) {
        perror("accept");
        return -1;
    }
    write_to_players("You are player 2, starting game...\r\n\r\n", -1, fd_p2);
    write_to_players("Welcome to the chocolate lego block game.\r\n", fd_p1, fd_p2);
    write_to_players("There are 10 lego pieces stacked vertically on a chocolate bar.\r\n", fd_p1, fd_p2);
    write_to_players("Each player takes turns to take 1-3 items from the stack (lego or chocolate).\r\n", fd_p1, fd_p2);
    write_to_players("The player who takes the last item -- the chocolate bar -- WINS!.\r\n", fd_p1, fd_p2);

    int num_pieces = LEGO_PIECES;
    char msg[MAX_BUF];
    int player = 2;

    // Game loop
    while(num_pieces >= 0){
        player = (player == 1) ? 2 : 1;

        sprintf(msg, "There are %d lego pieces left.\r\n", num_pieces);
        write_to_players(msg, fd_p1, fd_p2);
        sprintf(msg, "Player %d, take 1-3 pieces.\r\n", player);
        write_to_players(msg, fd_p1, fd_p2);

        int move = read_a_move(player == 1 ? fd_p1 : fd_p2);
        if (move == -1) {
            exit(0);
        }
        num_pieces -= move;
    }

    sprintf(msg, "Player %d has taken the chocolate bar, they win!\r\n", player);
    write_to_players(msg, fd_p1, fd_p2);

    return 0;
}
