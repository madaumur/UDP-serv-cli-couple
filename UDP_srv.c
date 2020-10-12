/*  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   */
/*                  -- Bibliotèques & Constantes --             */
/*  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT_DEFAULT    8080 
#define SIZE            1024 

/*  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   */
/*                  -- Début de notre programme --              */
/*  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   */

int main ( int argc, char** argv )
{
/*  -   -   Définition du port à utiliser   -   -   -   -   -   */
    int     port;
    
    // Utilisation du port par défaut
    if ( argc != 2 )
    {
        printf  ( "Format d'utilisation: %s <port>\n", argv[0] );
        printf  ( "Port par défaut utilisé : 8080\n" );
        port = PORT_DEFAULT;
    }
    // Utilisation du port fourni à l'execution
    else 
    {
        port = atoi ( argv[1] );
    }


/*  -   -  Création du socket file descriptor   -   -   -   -   */
    int     sockfd;

    // Création de la socket
    if ( ( sockfd = socket ( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
    {
        perror  ( "Socket() error\n" );
        exit    ( EXIT_FAILURE ); 
    }


/*  -   -   Structures d'adresse et bind()  -   -   -   -   -   */ 
    // Création des struct sockaddr_in et mise à zéro de la zone mémoire
    struct  sockaddr_in servaddr, cliaddr; 
    memset  ( &servaddr, '\0', sizeof ( servaddr ) );

    // Définition des paramètres de la struct servaddr
    servaddr.sin_family         = AF_INET;
    servaddr.sin_port           = htons ( port );
    servaddr.sin_addr.s_addr    = inet_addr ( "127.0.0.1" );
    for ( int i = 0; i < 8; i++ ) 
    {
        servaddr.sin_zero[i] = 0;
    }

    // Fonction bind()
    if ( bind ( sockfd, ( struct sockaddr * ) &servaddr, sizeof ( servaddr ) ) < 0 )
    {
        perror  ( "Bind() error\n" );
        exit    ( EXIT_FAILURE ); 
    }


/*  -   -   -   Facultatif : Verifications  -   -   -   -   -   */
    /*
    printf  ( "Descripteur de la socket : %d\n", sockfd );
    printf  ( "Valeur du numéro de port : %d\n", ntohs ( servaddr.sin_port ) );
    */

/*  -   -   -   -   Ecoute su serveur   -   -   -   -   -   -   */
    char    buffer[SIZE];
    int     limit = 1;
    socklen_t addr_size = sizeof ( cliaddr );

    // Boucle infinie d'écriture
    while ( limit == 1 )
    {  
        if ( recvfrom ( sockfd, buffer, 1024, 0, ( struct sockaddr * ) &cliaddr, &addr_size ) < 0 )
        {
            perror  ( "Recvfrom() error\n" );          
        }
        else
        {
            printf  ( "-- msg: '%s'\n", buffer );
            // Cas d'arret : message 'quit' ou 'ERROR' - sortie du while{}
            if ( strcmp ( buffer, "quit" ) == 0 || strcmp ( buffer, "ERROR" ) == 0 )
            {
                printf  ( "Commande d'arret reçue.\n" );
                limit = 0;
            }
        }
    }

/*  -   -   -   -   Fermeture du socket     -   -   -   -   -   */
    printf  ( "Fermeture de la socket\n" );
    close   ( sockfd );

    return  ( 0 );
}