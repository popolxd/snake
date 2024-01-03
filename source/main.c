#include <raylib.h>
#include <stdlib.h>

int ONE_SQUARE_SIZE = 100;
int INVERSE_SPEED = 20;

typedef struct {
    int apple_pos[2];
    int snake_size;
    int** snake_body_positions;
    int grid_width;
    int grid_height;
    int direction[2];

} main_struct;

void init_snake(main_struct* snake);
void free_snake(main_struct* snake);
int check_if_died(main_struct* snake);
void rellocate_apple(main_struct* snake);

void move(main_struct* snake, int current_frame) {
    if (IsKeyPressed(KEY_W) && !(snake->direction[0] == 0 && snake->direction[1] == 1)) {
        snake->direction[0] = 0;
        snake->direction[1] = -1;
    } else if (IsKeyPressed(KEY_S) && !(snake->direction[0] == 0 && snake->direction[1] == -1)) {
        snake->direction[0] = 0;
        snake->direction[1] = 1;
    } else if (IsKeyPressed(KEY_A) && !(snake->direction[0] == 1 && snake->direction[1] == 0)) {
        snake->direction[0] = -1;
        snake->direction[1] = 0;
    } else if (IsKeyPressed(KEY_D) && !(snake->direction[0] == -1 && snake->direction[1] == 0)) {
        snake->direction[0] = 1;
        snake->direction[1] = 0;
    }

    if (current_frame % INVERSE_SPEED == 0 && !(snake->direction[0] == 0 && snake->direction[1] == 0)) {

        int tail_pos[] = {snake->snake_body_positions[snake->snake_size - 1][0], snake->snake_body_positions[snake->snake_size - 1][1]};
        
        for (int i = (snake->snake_size - 1); i > 0; i--) {
            snake->snake_body_positions[i][0] = snake->snake_body_positions[i - 1][0];
            snake->snake_body_positions[i][1] = snake->snake_body_positions[i - 1][1];
        }

        snake->snake_body_positions[0][0] += snake->direction[0];
        snake->snake_body_positions[0][1] += snake->direction[1];

        if (check_if_died(snake)) {
            free_snake(snake);
            init_snake(snake);
        };

        if (snake->snake_body_positions[0][0] == snake->apple_pos[0] && snake->snake_body_positions[0][1] == snake->apple_pos[1]) {
            snake->snake_body_positions[snake->snake_size][0] = tail_pos[0];
            snake->snake_body_positions[snake->snake_size][1] = tail_pos[1];
            snake->snake_size++;

            rellocate_apple(snake);
        }
    }



}

int check_if_died(main_struct* snake) {
    if (snake->snake_body_positions[0][0] < 0 || snake->snake_body_positions[0][0] >= snake->grid_width
    || snake->snake_body_positions[0][1] < 0 || snake->snake_body_positions[0][1] >= snake->grid_height) {
        return 1;
    }
    for (int i = 1; i < snake->snake_size; i++) {
        if (snake->snake_body_positions[0][0] == snake->snake_body_positions[i][0]
        && snake->snake_body_positions[0][1] == snake->snake_body_positions[i][1]) {
            return 1;
        }
    }

    return 0;
}

void init_snake(main_struct* snake) {
    snake->apple_pos[0] = 5;
    snake->apple_pos[1] = 5;
    snake->snake_size = 3;
    snake->grid_width = 10;
    snake->grid_height = 10;
    snake->direction[0] = 0;
    snake->direction[1] = 0;

    snake->snake_body_positions = malloc(snake->grid_width * snake->grid_height * sizeof(int*));
    for (int i = 0; i < snake->grid_width * snake->grid_height; i++) {
        snake->snake_body_positions[i] = malloc(2 * sizeof(int));
        snake->snake_body_positions[i][0] = 0;
        snake->snake_body_positions[i][1] = 3 - i;
    }
}

void free_snake(main_struct* snake) {
    for (int i = 0; i < snake->grid_width * snake->grid_height; i++) {
        free(snake->snake_body_positions[i]);
    }

    free(snake->snake_body_positions);
}

void rellocate_apple(main_struct* snake) {
    while (1) {
        snake->apple_pos[0] = GetRandomValue(0, 9);
        snake->apple_pos[1] = GetRandomValue(0, 9);

        int is_free = 1;

        for (int i = 0; i < snake->snake_size; i++) {
            if (snake->snake_body_positions[i][0] == snake->apple_pos[0] && snake->snake_body_positions[i][1] == snake->apple_pos[1]) {
                is_free = 0;
                break;
            }
        }

        if (is_free) {
            break;
        }
    }
}

int main() {

    InitWindow(1000, 1000, "Snake");
    // int display = GetCurrentMonitor();
    // int SCREEN_WIDTH = GetMonitorWidth(display);
    // int SCREEN_HEIGHT = GetMonitorHeight(display);
    // SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    main_struct snake;
    init_snake(&snake);

    SetTargetFPS(60);

    int current_frame = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){0, 0, 0, 255});

        current_frame += 1;
        move(&snake, current_frame);

        if (current_frame == INVERSE_SPEED) current_frame = 0;

        for (int i = 0; i < snake.grid_height; i++) {
            for (int j = 0; j < snake.grid_width; j++) {

                DrawRectangle(j * ONE_SQUARE_SIZE, i * ONE_SQUARE_SIZE, ONE_SQUARE_SIZE - 1, ONE_SQUARE_SIZE - 1, (Color){50, 50, 50, 250});
            }
        }

        for (int i = 0; i < snake.snake_size; i++) {

            DrawRectangle(snake.snake_body_positions[i][0] * ONE_SQUARE_SIZE, snake.snake_body_positions[i][1] * ONE_SQUARE_SIZE, ONE_SQUARE_SIZE - 1, ONE_SQUARE_SIZE - 1, (Color){20, 20, 220, 255});
        }

        DrawRectangle(snake.apple_pos[0] * ONE_SQUARE_SIZE, snake.apple_pos[1] * ONE_SQUARE_SIZE, ONE_SQUARE_SIZE - 1, ONE_SQUARE_SIZE - 1, (Color){220, 20, 20, 255});

        EndDrawing();
    }

    CloseWindow();
    free_snake(&snake);

    return 0;
}