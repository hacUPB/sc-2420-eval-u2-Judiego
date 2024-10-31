#include <stdio.h> 
#include <stdbool.h>
#include <SDL.h>
#include "Constants.h"

// Variables globales para la ventana y el renderizador
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool game_is_running = true; // Para saber si el juego sigue corriendo o no

int last_frame_time = 0; //controlar el tiempo entre frames
int gravity = 300;       //Gravedad que afecta al jugador
int jump_force = 400;    //Fuerza con la que el jugador salta
bool is_jumping = false; //saber si el jugador está en el aire

//Estructura del jugador
struct Player {
    float x, y;          // Posición 
    float width, height; // Tamaño del jugador
    int vel_y;           // Velocidad vertical del jugador
} player;

//Estructura del obstáculo
struct Obstacle {
    float x, y;          // Posición del obstáculo
    float width, height; // Tamaño del obstáculo
    float vel_x;         // Velocidad con la que se mueve hacia la izquierda
} obstacle;

//Inicializa la ventana 
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("Error inicializando SDL \n");
        return 0;
    }

    window = SDL_CreateWindow(
        "Geometry Dash",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Error creando la ventana \n");
        return 0;
    }

    // dibujo
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error creando el renderizador \n");
        return 0;
    }

    return 1; // Todo bien
}

void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) { // Verifica si hay eventos en la cola
        switch (event.type) {
        case SDL_QUIT: //Si cierran la ventana
            game_is_running = false;
            break;
        case SDL_KEYDOWN: // Si se presiona una tecla
            if (event.key.keysym.sym == SDLK_ESCAPE) { //Salir con la tecla Escape
                game_is_running = false;
            }
            if (event.key.keysym.sym == SDLK_SPACE && !is_jumping) { //Saltar con espacio
                player.vel_y = -jump_force; //Salto hacia arriba
                is_jumping = true;          //El jugador está saltando
            }
            break;
        }
    }
}

//Configuración inicial del jugador y el obstáculo
void setup() {
    player.x = 50;
    player.y = WINDOW_HEIGHT - GROUND_HEIGHT - 50;
    player.width = 50;
    player.height = 50;
    player.vel_y = 0;

    obstacle.x = WINDOW_WIDTH;
    obstacle.y = WINDOW_HEIGHT - GROUND_HEIGHT - 50;
    obstacle.width = 50;
    obstacle.height = 50;
    obstacle.vel_x = -200; //Se mueve a la izquierda
}

//Actualiza la posición del jugador y del obstáculo
void update() {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();

    //Gravedad afecta al jugador
    player.vel_y += gravity * delta_time;
    player.y += player.vel_y * delta_time;

    //Si el jugador toca el suelo, deja de caer
    if (player.y >= WINDOW_HEIGHT - GROUND_HEIGHT - player.height) {
        player.y = WINDOW_HEIGHT - GROUND_HEIGHT - player.height;
        player.vel_y = 0;
        is_jumping = false; // Ya no está en el aire
    }

    //obstáculo se mueve hacia la izquierda
    obstacle.x += obstacle.vel_x * delta_time;

    //Si el obstáculo sale de la pantalla, aparece a la derecha
    if (obstacle.x + obstacle.width < 0) {
        obstacle.x = WINDOW_WIDTH;
    }

    //Colisión entre jugador y el obstáculo
    if (player.x < obstacle.x + obstacle.width &&
        player.x + player.width > obstacle.x &&
        player.y < obstacle.y + obstacle.height &&
        player.y + player.height > obstacle.y) {
        // Si chocan termina el juego
        game_is_running = false;
        printf("perdiste.\n");
    }
}

//Dibuja el jugador y el obstáculo
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fondo
    SDL_RenderClear(renderer);

    // Dibujar jugador
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect player_rect = { (int)player.x, (int)player.y, (int)player.width, (int)player.height };
    SDL_RenderFillRect(renderer, &player_rect);

    // Dibuja obstáculo 
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle_rect = { (int)obstacle.x, (int)obstacle.y, (int)obstacle.width, (int)obstacle.height };
    SDL_RenderFillRect(renderer, &obstacle_rect);

    SDL_RenderPresent(renderer); 
}

// Destruye la ventana 
void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Función principal
int main(int argc, char* args[]) {
    game_is_running = initialize_window(); // Inicializa la ventana

    setup(); 

    while (game_is_running) {
        process_input(); // Maneja el teclado
        update();        // Actualiza la lógica del juego
        render();        // Dibuja los objetos
    }

    destroy_window(); // Cierra el juego y limpia la memoria
    return 0;
}

