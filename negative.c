#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int lireligne(int fd, char *buffer, int size) {
    ssize_t nbread = read(fd, buffer, size);
    if (nbread == -1) {
        return -1;
    }

    int i;
    for (i = 0; i < nbread; i++) {
        if (buffer[i] == '\n') {
            i++;
            break;
        }
    }
    lseek(fd, i - nbread, SEEK_CUR);
    return i;
}

int main(int argc, char **argv) {
    int fd_in;  // descripteur de fichier du fichier ouvert en lecture
    int fd_out; // descripteur de fichier du fichier ouvert en écriture
    int nbread;
    int nbreadfork;
    unsigned char *buffer = malloc(4096 * sizeof(unsigned char));    // buffer de lecture
    unsigned char *bufferfork = malloc(4096 * sizeof(unsigned char));

    if (argc < 3) {
        printf("%s\n", "Pas assez d'arguments");
        exit(1);
    }

    fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0) {
        perror(argv[1]);
        exit(1);
    }

    fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (fd_out < 0) {
        perror(argv[2]);
        exit(1);
    }

    for (int i = 0; i < 3; i++) { // Copie les 3 lignes de l'entête du PGM d'entré
        nbread = lireligne(fd_in, buffer, 4096); // Copie la ligne du PGM d'entré dans le buffer
        write(fd_out, buffer, nbread); // Ecrit le contenu du buffer dans le PGM de sortie
    }

    int a = fork();

    if (a != 0) {
        while (nbread != 0) { // Tant qu'on est pas arrivé à la fin du document
            nbread = lireligne(fd_in, buffer, 4096);
            for (int i = 0;
                 i < nbread; i++) { // Pour chaque case du buffer applique une opération inversant les couleurs
                buffer[i] = (unsigned char) (255 - buffer[i]);
            }
            write(fd_out, buffer, nbread); // Ecrit le contenu du buffer modifié dans le PGM de sortie
        }
    } else {
        do {
            nbreadfork = lireligne(fd_in, bufferfork, 4096);
            write(fd_out, bufferfork, nbreadfork);
        } while (nbreadfork != 0);
    }

    return 0;
}