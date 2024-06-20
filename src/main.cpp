#include <iostream>
#include <cstdlib> // Para usar rand()
#include <ctime>   // Para inicializar la semilla de rand()
#include <unistd.h> // Para usleep() en sistemas POSIX
#include <termios.h> // Para configuración de terminal POSIX
#include <fcntl.h> // Para operaciones de control de archivo POSIX

using namespace std;

// Definiciones de constantes
const int WIDTH = 80;    // Ancho del tablero
const int HEIGHT = 20;   // Alto del tablero
const int PADDLE_SIZE = 4; // Tamaño de las paletas
const int BALL_DELAY = 80000; // Retardo de movimiento de la bola (microsegundos)

// Clase Paddle (paleta)
class Paddle {
public:
    Paddle(int startX, int startY); // Constructor
    void moveUp();
    void moveDown();
    int getX() const;
    int getY() const;
    static const int SIZE = PADDLE_SIZE; // Tamaño de la paleta
    
private:
    int x;
    int y;
};

// Constructor de Paddle
Paddle::Paddle(int startX, int startY) : x(startX), y(startY) {}

// Métodos de Paddle
void Paddle::moveUp() {
    y--;
    if (y < 1) y = 1; // Limita el movimiento hacia arriba
}

void Paddle::moveDown() {
    y++;
    if (y > HEIGHT - SIZE - 1) y = HEIGHT - SIZE - 1; // Limita el movimiento hacia abajo
}

int Paddle::getX() const {
    return x;
}

int Paddle::getY() const {
    return y;
}

// Clase Ball (pelota)
class Ball {
public:
    Ball(int startX, int startY); // Constructor
    void move();
    int getX() const;
    int getY() const;
    
private:
    int x;
    int y;
    int dx;
    int dy;
};

// Constructor de Ball
Ball::Ball(int startX, int startY) : x(startX), y(startY) {
    // Inicializa la dirección de la pelota de manera aleatoria
    srand(time(0));
    dx = (rand() % 2 == 0) ? -1 : 1; // Dirección horizontal (-1 izquierda, 1 derecha)
    dy = (rand() % 2 == 0) ? -1 : 1; // Dirección vertical (-1 arriba, 1 abajo)
}

// Método para mover la pelota
void Ball::move() {
    x += dx;
    y += dy;

    // Colisión con los bordes verticales
    if (y <= 0 || y >= HEIGHT - 1) {
        dy = -dy; // Invierte la dirección vertical
    }
}

int Ball::getX() const {
    return x;
}

int Ball::getY() const {
    return y;
}

// Función para verificar si hay entrada del teclado en sistemas POSIX
bool key_pressed() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

// Función principal (main)
int main() {
    // Inicialización de las paletas y la pelota
    Paddle leftPaddle(1, HEIGHT / 2 - Paddle::SIZE / 2);
    Paddle rightPaddle(WIDTH - 2, HEIGHT / 2 - Paddle::SIZE / 2);
    Ball ball(WIDTH / 2, HEIGHT / 2);

    // Bucle principal del juego
    while (true) {
        // Borrado del tablero (limpia la pantalla)
        system("clear"); // Cambia a "cls" si estás en Windows

        // Movimiento de las paletas
        char input;
        if (key_pressed()) {
            input = getchar();
            if (input == 'w')
                leftPaddle.moveUp();
            else if (input == 's')
                leftPaddle.moveDown();
            else if (input == 'i')
                rightPaddle.moveUp();
            else if (input == 'k')
                rightPaddle.moveDown();
        }

        // Movimiento de la pelota
        ball.move();

        // Dibujo del tablero
        for (int i = 0; i < WIDTH; i++)
            cout << "-";
        cout << endl;

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0 || j == WIDTH - 1)
                    cout << "|"; // Paredes laterales
                else if (i >= leftPaddle.getY() && i < leftPaddle.getY() + Paddle::SIZE && j == leftPaddle.getX())
                    cout << "#"; // Paleta izquierda
                else if (i >= rightPaddle.getY() && i < rightPaddle.getY() + Paddle::SIZE && j == rightPaddle.getX())
                    cout << "#"; // Paleta derecha
                else if (i == ball.getY() && j == ball.getX())
                    cout << "O"; // Pelota
                else
                    cout << " "; // Espacio vacío dentro del tablero
            }
            cout << endl;
        }

        for (int i = 0; i < WIDTH; i++)
            cout << "-";
        cout << endl;

        // Retardo para el movimiento de la pelota
        usleep(BALL_DELAY);
    }

    return 0;
}

tar SFML-2.6.1-windows-vc17-64-bit-xf tar.gz
